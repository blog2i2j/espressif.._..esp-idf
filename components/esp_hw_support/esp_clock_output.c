/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <sys/queue.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_private/gpio.h"
#include "esp_clock_output.h"
#include "esp_check.h"
#include "esp_rom_gpio.h"
#include "soc/clkout_channel.h"
#include "hal/gpio_hal.h"
#include "hal/clk_tree_hal.h"
#include "hal/clk_tree_ll.h"
#include "soc/soc_caps.h"
#include "soc/io_mux_reg.h"
#include "esp_private/gpio.h"
#include "esp_private/critical_section.h"

typedef struct clkout_channel_handle {
    bool is_mapped;
    soc_clkout_sig_id_t mapped_clock;
    clock_out_channel_t channel_id;
    uint8_t ref_cnt;
    uint64_t mapped_io_bmap;
    portMUX_TYPE clkout_channel_lock;
} clkout_channel_handle_t;

typedef struct esp_clock_output_mapping {
    gpio_num_t mapped_io;
    clkout_channel_handle_t* clkout_channel_hdl;
    uint8_t ref_cnt;
    portMUX_TYPE clkout_mapping_lock;
    SLIST_ENTRY(esp_clock_output_mapping) next;
} esp_clock_output_mapping_t;

static const char *TAG = "esp_clock_output";

static SLIST_HEAD(esp_clock_output_mapping_head, esp_clock_output_mapping) s_mapping_list = SLIST_HEAD_INITIALIZER(s_mapping_list_head);
static portMUX_TYPE __attribute__((unused)) s_mapping_list_lock = portMUX_INITIALIZER_UNLOCKED;
static portMUX_TYPE __attribute__((unused)) s_clkout_lock = portMUX_INITIALIZER_UNLOCKED;

static clkout_channel_handle_t s_clkout_handle[CLKOUT_CHANNEL_MAX] = {
    [0 ... CLKOUT_CHANNEL_MAX - 1] = {
        .is_mapped = false,
        .ref_cnt = 0,
        .mapped_io_bmap = 0,
        .clkout_channel_lock = portMUX_INITIALIZER_UNLOCKED,
    }
};

static clkout_channel_handle_t* clkout_channel_alloc(soc_clkout_sig_id_t clk_sig, gpio_num_t gpio_num)
{
    clkout_channel_handle_t *allocated_channel = NULL;

#if SOC_GPIO_CLOCKOUT_BY_IO_MUX
    int32_t channel_num = IONUM_TO_CLKOUT_CHANNEL(gpio_num);
    if (channel_num < 0) {
        return NULL;
    }
    esp_os_enter_critical(&s_clkout_handle[channel_num].clkout_channel_lock);
    if (!s_clkout_handle[channel_num].is_mapped) {
        s_clkout_handle[channel_num].is_mapped = true;
        allocated_channel = &s_clkout_handle[channel_num];
    } else if ((s_clkout_handle[channel_num].mapped_io_bmap & BIT(gpio_num)) &&
               (s_clkout_handle[channel_num].mapped_clock == clk_sig)) {
        allocated_channel = &s_clkout_handle[channel_num];
    }
    if (allocated_channel != NULL) {
        allocated_channel->channel_id = (clock_out_channel_t)channel_num;
    }
    esp_os_exit_critical(&s_clkout_handle[channel_num].clkout_channel_lock);
#elif SOC_GPIO_CLOCKOUT_BY_GPIO_MATRIX
    for (uint32_t channel = 0; channel < CLKOUT_CHANNEL_MAX; channel++) {
        esp_os_enter_critical(&s_clkout_handle[channel].clkout_channel_lock);
        if (!s_clkout_handle[channel].is_mapped) {
            s_clkout_handle[channel].is_mapped = true;
            allocated_channel = &s_clkout_handle[channel];
            allocated_channel->channel_id = (clock_out_channel_t)channel;
            esp_os_exit_critical(&s_clkout_handle[channel].clkout_channel_lock);
            break;
        } else if (s_clkout_handle[channel].mapped_clock == clk_sig) {
            allocated_channel = &s_clkout_handle[channel];
            esp_os_exit_critical(&s_clkout_handle[channel].clkout_channel_lock);
            break;
        }
        esp_os_exit_critical(&s_clkout_handle[channel].clkout_channel_lock);
    }
#endif

    if (allocated_channel != NULL) {
        esp_os_enter_critical(&allocated_channel->clkout_channel_lock);
        allocated_channel->mapped_io_bmap |= BIT(gpio_num);
        allocated_channel->mapped_clock = clk_sig;
        allocated_channel->ref_cnt++;
        if (allocated_channel->ref_cnt == 1) {
            esp_os_enter_critical(&s_clkout_lock);
#if SOC_CLOCKOUT_HAS_SOURCE_GATE
            clk_ll_enable_clkout_source(clk_sig, true);
#endif
            clk_hal_clock_output_setup(clk_sig, allocated_channel->channel_id);
            esp_os_exit_critical(&s_clkout_lock);
        }
        esp_os_exit_critical(&allocated_channel->clkout_channel_lock);
    }

    return allocated_channel;
}

static esp_clock_output_mapping_t* clkout_mapping_alloc(clkout_channel_handle_t* channel_hdl, gpio_num_t gpio_num)
{
    esp_clock_output_mapping_t *allocated_mapping = NULL;

    esp_os_enter_critical(&s_mapping_list_lock);
    esp_clock_output_mapping_t *hdl;
    SLIST_FOREACH(hdl, &s_mapping_list, next) {
        if ((hdl->clkout_channel_hdl == channel_hdl) && (hdl->mapped_io == gpio_num)) {
            allocated_mapping = hdl;
        }
    }
    esp_os_exit_critical(&s_mapping_list_lock);

    if (allocated_mapping == NULL) {
        allocated_mapping = (esp_clock_output_mapping_t *)malloc(sizeof(esp_clock_output_mapping_t));
        if (!allocated_mapping) {
            return NULL;
        }
        allocated_mapping->mapped_io = gpio_num;
        allocated_mapping->clkout_channel_hdl = channel_hdl;
        allocated_mapping->ref_cnt = 0;
        portMUX_INITIALIZE(&allocated_mapping->clkout_mapping_lock);
        esp_os_enter_critical(&s_mapping_list_lock);
        SLIST_INSERT_HEAD(&s_mapping_list, allocated_mapping, next);
        esp_os_exit_critical(&s_mapping_list_lock);
    }

    esp_os_enter_critical(&allocated_mapping->clkout_mapping_lock);
    allocated_mapping->ref_cnt++;
    if (allocated_mapping->ref_cnt == 1) {
#if SOC_GPIO_CLOCKOUT_BY_IO_MUX
        gpio_iomux_output(gpio_num, CLKOUT_CHANNEL_TO_IOMUX_FUNC(allocated_mapping->clkout_channel_hdl->channel_id));
#elif SOC_GPIO_CLOCKOUT_BY_GPIO_MATRIX
        gpio_set_pull_mode(gpio_num, GPIO_FLOATING);
        gpio_func_sel(gpio_num, PIN_FUNC_GPIO);
        esp_rom_gpio_connect_out_signal(gpio_num, CLKOUT_CHANNEL_TO_GPIO_SIG_ID(allocated_mapping->clkout_channel_hdl->channel_id), false, false);
#endif
    }
    esp_os_exit_critical(&allocated_mapping->clkout_mapping_lock);
    return allocated_mapping;
}

static void clkout_channel_free(clkout_channel_handle_t *channel_hdl)
{
    esp_os_enter_critical(&channel_hdl->clkout_channel_lock);
    if (--channel_hdl->ref_cnt == 0) {
        esp_os_enter_critical(&s_clkout_lock);
#if SOC_CLOCKOUT_HAS_SOURCE_GATE
        clk_ll_enable_clkout_source(channel_hdl->mapped_clock, false);
#endif
        clk_hal_clock_output_teardown(channel_hdl->channel_id);
        esp_os_exit_critical(&s_clkout_lock);
        channel_hdl->mapped_clock = CLKOUT_SIG_INVALID;
        channel_hdl->is_mapped = false;
    }
    esp_os_exit_critical(&channel_hdl->clkout_channel_lock);
}

static void clkout_mapping_free(esp_clock_output_mapping_t *mapping_hdl)
{
    esp_os_enter_critical(&mapping_hdl->clkout_mapping_lock);
    clkout_channel_free(mapping_hdl->clkout_channel_hdl);
    bool do_free_mapping_hdl = false;
    if (--mapping_hdl->ref_cnt == 0) {
        gpio_output_disable(mapping_hdl->mapped_io);

        esp_os_enter_critical(&mapping_hdl->clkout_channel_hdl->clkout_channel_lock);
        mapping_hdl->clkout_channel_hdl->mapped_io_bmap &= ~BIT(mapping_hdl->mapped_io);
        esp_os_exit_critical(&mapping_hdl->clkout_channel_hdl->clkout_channel_lock);

        esp_os_enter_critical(&s_mapping_list_lock);
        SLIST_REMOVE(&s_mapping_list, mapping_hdl, esp_clock_output_mapping, next);
        esp_os_exit_critical(&s_mapping_list_lock);
        do_free_mapping_hdl = true;
    }
    esp_os_exit_critical(&mapping_hdl->clkout_mapping_lock);

    if (do_free_mapping_hdl) {
        free(mapping_hdl);
    }
}

esp_err_t esp_clock_output_start(soc_clkout_sig_id_t clk_sig, gpio_num_t gpio_num, esp_clock_output_mapping_handle_t *clkout_mapping_ret_hdl)
{
    ESP_RETURN_ON_FALSE((clkout_mapping_ret_hdl != NULL), ESP_ERR_INVALID_ARG, TAG, "Clock out mapping handle passed in is invalid");
#if SOC_GPIO_CLOCKOUT_BY_GPIO_MATRIX
    ESP_RETURN_ON_FALSE(GPIO_IS_VALID_GPIO(gpio_num), ESP_ERR_INVALID_ARG, TAG, "%s", "Output GPIO number error");
#else
    ESP_RETURN_ON_FALSE(IS_VALID_CLKOUT_IO(gpio_num), ESP_ERR_INVALID_ARG, TAG, "%s", "Output GPIO number error");
#endif

    esp_clock_output_mapping_t *hdl;
    SLIST_FOREACH(hdl, &s_mapping_list, next) {
        ESP_RETURN_ON_FALSE(!((hdl->mapped_io == gpio_num) && (hdl->clkout_channel_hdl->mapped_clock != clk_sig)), ESP_ERR_INVALID_ARG, TAG, "Selected io is already mapped by another signal");
    }

    clkout_channel_handle_t *channel_hdl = clkout_channel_alloc(clk_sig, gpio_num);
#if SOC_GPIO_CLOCKOUT_BY_IO_MUX
    ESP_RETURN_ON_FALSE((channel_hdl != NULL), ESP_ERR_INVALID_ARG, TAG, "Selected clock out IO is already mapped to other internal clock source");
#elif SOC_GPIO_CLOCKOUT_BY_GPIO_MATRIX
    ESP_RETURN_ON_FALSE((channel_hdl != NULL), ESP_FAIL, TAG, "Maximum support for %d output clock signals, no available clock_out channel for assignment", CLKOUT_CHANNEL_MAX);
#endif

    *clkout_mapping_ret_hdl = clkout_mapping_alloc(channel_hdl, gpio_num);

    return ESP_OK;
}

esp_err_t esp_clock_output_stop(esp_clock_output_mapping_handle_t clkout_mapping_hdl)
{
    ESP_RETURN_ON_FALSE((clkout_mapping_hdl != NULL), ESP_ERR_INVALID_ARG, TAG, "Clock out mapping handle passed in is invalid");
    ESP_RETURN_ON_FALSE(clkout_mapping_hdl->ref_cnt > 0, ESP_ERR_INVALID_STATE, TAG, "%s", "Clock outputting is already disabled");
    clkout_mapping_free(clkout_mapping_hdl);
    return ESP_OK;
}

#if SOC_CLOCKOUT_SUPPORT_CHANNEL_DIVIDER
esp_err_t esp_clock_output_set_divider(esp_clock_output_mapping_handle_t clkout_mapping_hdl, uint32_t div_num)
{
    ESP_RETURN_ON_FALSE(((div_num > 0) && (div_num <= 256)), ESP_ERR_INVALID_ARG, TAG, "Divider number must be in the range of [1,  256]");
    ESP_RETURN_ON_FALSE((clkout_mapping_hdl != NULL), ESP_ERR_INVALID_ARG, TAG, "Clock out mapping handle passed in is invalid");
    esp_os_enter_critical(&clkout_mapping_hdl->clkout_mapping_lock);
    clk_hal_clock_output_set_divider(clkout_mapping_hdl->clkout_channel_hdl->channel_id, div_num);
    esp_os_exit_critical(&clkout_mapping_hdl->clkout_mapping_lock);
    return ESP_OK;
}
#endif

#if CONFIG_IDF_TARGET_ESP32
// Due to a hardware bug, PIN_CTRL cannot select 0xf output, whereas 0xf is the default value.
__attribute__((constructor))
static void esp_clock_output_pin_ctrl_init(void)
{
    gpio_ll_set_pin_ctrl(0, CLK_OUT1, CLK_OUT1_S);
    gpio_ll_set_pin_ctrl(0, CLK_OUT2, CLK_OUT2_S);
    gpio_ll_set_pin_ctrl(0, CLK_OUT3, CLK_OUT3_S);
}
#endif
