/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include <stdlib.h>
#include <esp_types.h>
#include "sdkconfig.h"
#include "soc/soc.h"
#include "pmu_param.h"
#include "soc/pmu_icg_mapping.h"
#include "esp_private/esp_pmu.h"
#include "soc/clk_tree_defs.h"
#include "hal/efuse_ll.h"
#include "hal/efuse_hal.h"
#include "esp_hw_log.h"

static __attribute__((unused)) const char *TAG = "pmu_param";

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)   (sizeof(a) / sizeof((a)[0]))
#endif

#define PMU_HP_ACTIVE_POWER_CONFIG_DEFAULT() { \
    .dig_power = {          \
        .mem_dslp      = 0, \
        .mem_pd_en     = 0, \
        .cnnt_pd_en    = 0, \
        .top_pd_en     = 0, \
        .dcdc_switch_pd_en = 0 \
    }, \
    .clk_power = {          \
        .i2c_iso_en    = 0, \
        .i2c_retention = 0, \
        .xpd_pll_i2c   = 0xf, \
        .xpd_pll       = 0xf  \
    }, \
    .xtal = {               \
        .xpd_xtal      = 1  \
    } \
}

#define PMU_HP_SLEEP_POWER_CONFIG_DEFAULT() { \
    .dig_power = {          \
        .mem_dslp      = 0, \
        .mem_pd_en     = 0, \
        .cnnt_pd_en    = 0, \
        .top_pd_en     = 0, \
        .dcdc_switch_pd_en = 1 \
    }, \
    .clk_power = {          \
        .i2c_iso_en    = 1, \
        .i2c_retention = 1, \
        .xpd_pll_i2c   = 1, \
        .xpd_pll       = 0  \
    }, \
    .xtal = {               \
        .xpd_xtal      = 0  \
    } \
}

const pmu_hp_system_power_param_t * pmu_hp_system_power_param_default(pmu_hp_mode_t mode)
{
    static const pmu_hp_system_power_param_t hp_power[] = {
        PMU_HP_ACTIVE_POWER_CONFIG_DEFAULT(),
        {{}, {}, {}}, // No Modem
        PMU_HP_SLEEP_POWER_CONFIG_DEFAULT()
    };
    assert(mode < ARRAY_SIZE(hp_power));
    return &hp_power[mode];
}

#define PMU_HP_ACTIVE_CLOCK_CONFIG_DEFAULT() {    \
    .icg_func   = 0xffffffff,                     \
    .icg_apb    = 0xffffffff,                     \
    .icg_modem  = 0,                              \
    .sysclk     = {                               \
        .dig_sysclk_nodiv = 0,                    \
        .icg_sysclk_en    = 1,                    \
        .sysclk_slp_sel   = 0,                    \
        .icg_slp_sel      = 0,                    \
        .dig_sysclk_sel   = SOC_CPU_CLK_SRC_XTAL  \
    } \
}

#define PMU_HP_SLEEP_CLOCK_CONFIG_DEFAULT() {     \
    .icg_func   = 0,                              \
    .icg_apb    = 0,                              \
    .icg_modem  = 0,                              \
    .sysclk     = {                               \
        .dig_sysclk_nodiv = 0,                    \
        .icg_sysclk_en    = 0,                    \
        .sysclk_slp_sel   = 1,                    \
        .icg_slp_sel      = 1,                    \
        .dig_sysclk_sel   = SOC_CPU_CLK_SRC_XTAL  \
    } \
}

const pmu_hp_system_clock_param_t * pmu_hp_system_clock_param_default(pmu_hp_mode_t mode)
{
    static const pmu_hp_system_clock_param_t hp_clock[] = {
        PMU_HP_ACTIVE_CLOCK_CONFIG_DEFAULT(),
        {0, 0, 0, {}}, // No Modem
        PMU_HP_SLEEP_CLOCK_CONFIG_DEFAULT()
    };
    assert(mode < ARRAY_SIZE(hp_clock));
    return &hp_clock[mode];
}

#define PMU_HP_ACTIVE_DIGITAL_CONFIG_DEFAULT() { \
    .syscntl = {                \
        .uart_wakeup_en  = 0,   \
        .lp_pad_hold_all = 0,   \
        .hp_pad_hold_all = 0,   \
        .dig_pad_slp_sel = 0,   \
        .dig_pause_wdt   = 0,   \
        .dig_cpu_stall   = 0    \
    } \
}

#define PMU_HP_SLEEP_DIGITAL_CONFIG_DEFAULT() { \
    .syscntl = {                \
        .uart_wakeup_en  = 1,   \
        .lp_pad_hold_all = 0,   \
        .hp_pad_hold_all = 0,   \
        .dig_pad_slp_sel = 0,   \
        .dig_pause_wdt   = 1,   \
        .dig_cpu_stall   = 1    \
    } \
}

const pmu_hp_system_digital_param_t * pmu_hp_system_digital_param_default(pmu_hp_mode_t mode)
{
    static const pmu_hp_system_digital_param_t hp_digital[] = {
        PMU_HP_ACTIVE_DIGITAL_CONFIG_DEFAULT(),
        {{}}, // No Modem
        PMU_HP_SLEEP_DIGITAL_CONFIG_DEFAULT()
    };
    assert(mode < ARRAY_SIZE(hp_digital));
    return &hp_digital[mode];
}

#define PMU_HP_ACTIVE_ANALOG_CONFIG_DEFAULT() { \
    .bias = {                   \
        .dcm_vset        = 27,  \
        .dcm_mode        = 1,   \
        .xpd_bias        = 1,   \
        .dbg_atten       = 0x0, \
        .pd_cur          = 0,   \
        .bias_sleep      = 0    \
    }, \
    .regulator0 = {             \
        .lp_dbias_vol    = 0xd, \
        .hp_dbias_vol    = 0x1c,\
        .dbias_sel       = 1,   \
        .dbias_init      = 1,   \
        .slp_mem_xpd     = 0,   \
        .slp_logic_xpd   = 0,   \
        .slp_mem_dbias   = 1, /* slp_mem_dbias bit[3] controls ext_ldo_1 xpd for rev1.0*/ \
        .slp_logic_dbias = 0,   \
    }, \
    .regulator1 = {             \
        .drv_b           = 0x0 \
    } \
}

#define PMU_HP_SLEEP_ANALOG_CONFIG_DEFAULT() { \
    .bias = {                   \
        .dcm_vset        = 0,   \
        .dcm_mode        = 0,   \
        .xpd_bias        = 0,   \
        .dbg_atten       = 0x0, \
        .pd_cur          = 1,   \
        .bias_sleep      = 1    \
    }, \
    .regulator0 = {             \
        .slp_mem_xpd     = 0,   \
        .slp_logic_xpd   = 0,   \
        .slp_mem_dbias   = 1, /* slp_mem_dbias bit[3] controls ext_ldo_1 xpd for rev1.0*/ \
        .slp_logic_dbias = 0,   \
    }, \
    .regulator1 = {             \
        .drv_b           = 0x0 \
    } \
}

const pmu_hp_system_analog_param_t * pmu_hp_system_analog_param_default(pmu_hp_mode_t mode)
{
    static const pmu_hp_system_analog_param_t hp_analog[] = {
        PMU_HP_ACTIVE_ANALOG_CONFIG_DEFAULT(),
        {{}, {}, {}}, // No Modem
        PMU_HP_SLEEP_ANALOG_CONFIG_DEFAULT()
    };
    assert(mode < ARRAY_SIZE(hp_analog));
    return &hp_analog[mode];
}

#define PMU_HP_RETENTION_REGDMA_CONFIG(dir, entry)  ((((dir)<<2) | (entry & 0x3)) & 0x7)

#define PMU_HP_ACTIVE_RETENTION_CONFIG_DEFAULT() {  \
    .retention = {                                  \
        .hp_sleep2active_backup_modem_clk_code = 2, \
        .hp_modem2active_backup_modem_clk_code = 2, \
        .hp_active_retention_mode       = 0, \
        .hp_sleep2active_retention_en   = 0, \
        .hp_modem2active_retention_en   = 0, \
        .hp_sleep2active_backup_clk_sel = 0, \
        .hp_modem2active_backup_clk_sel = 0, \
        .hp_sleep2active_backup_mode    = PMU_HP_RETENTION_REGDMA_CONFIG(0, 0), \
        .hp_modem2active_backup_mode    = PMU_HP_RETENTION_REGDMA_CONFIG(0, 2), \
        .hp_sleep2active_backup_en      = 0, \
        .hp_modem2active_backup_en      = 0, \
    }, \
    .backup_clk = 0xffffffff,   \
}

#define PMU_HP_SLEEP_RETENTION_CONFIG_DEFAULT() {   \
    .retention = {                                  \
        .hp_modem2sleep_backup_modem_clk_code  = 0, \
        .hp_active2sleep_backup_modem_clk_code = 2, \
        .hp_sleep_retention_mode        = 0, \
        .hp_modem2sleep_retention_en    = 0, \
        .hp_active2sleep_retention_en   = 0, \
        .hp_modem2sleep_backup_clk_sel  = 0, \
        .hp_active2sleep_backup_clk_sel = 0, \
        .hp_modem2sleep_backup_mode     = PMU_HP_RETENTION_REGDMA_CONFIG(1, 1), \
        .hp_active2sleep_backup_mode    = PMU_HP_RETENTION_REGDMA_CONFIG(1, 0), \
        .hp_modem2sleep_backup_en       = 0, \
        .hp_active2sleep_backup_en      = 0, \
    }, \
    .backup_clk = 0xffffffff,   \
}

const pmu_hp_system_retention_param_t * pmu_hp_system_retention_param_default(pmu_hp_mode_t mode)
{
    static const pmu_hp_system_retention_param_t hp_retention[] = {
        PMU_HP_ACTIVE_RETENTION_CONFIG_DEFAULT(),
        {{}, 0}, // No Modem
        PMU_HP_SLEEP_RETENTION_CONFIG_DEFAULT()
    };
    assert(mode < ARRAY_SIZE(hp_retention));
    return &hp_retention[mode];
}


/** LP system default parameter */
#define PMU_LP_ACTIVE_POWER_CONFIG_DEFAULT() { \
    .dig_power = {              \
        .lp_pad_slp_sel = 0,    \
        .bod_source_sel = 0,    \
        .vddbat_mode    = 0,    \
        .mem_dslp       = 0,    \
        .peri_pd_en     = 0,    \
    }, \
    .clk_power = {              \
        .xpd_lppll      = 0,    \
        .xpd_xtal32k    = 1,    \
        .xpd_rc32k      = 1,    \
        .xpd_fosc       = 1,    \
        .pd_osc         = 0     \
    } \
}

#define PMU_LP_SLEEP_POWER_CONFIG_DEFAULT() { \
    .dig_power = {              \
        .lp_pad_slp_sel = 0,    \
        .bod_source_sel = 0,    \
        .vddbat_mode    = 0,    \
        .mem_dslp       = 0,    \
        .peri_pd_en     = 0,    \
    }, \
    .clk_power = {              \
        .xpd_lppll      = 0,    \
        .xpd_xtal32k    = 0,    \
        .xpd_rc32k      = 0,    \
        .xpd_fosc       = 0,    \
        .pd_osc         = 0     \
    }, \
    .xtal = {                   \
        .xpd_xtal       = 0     \
    } \
}

const pmu_lp_system_power_param_t * pmu_lp_system_power_param_default(pmu_lp_mode_t mode)
{
    static const pmu_lp_system_power_param_t lp_power[] = {
        PMU_LP_ACTIVE_POWER_CONFIG_DEFAULT(),
        PMU_LP_SLEEP_POWER_CONFIG_DEFAULT()
    };
    assert(mode < ARRAY_SIZE(lp_power));
    return &lp_power[mode];
}

#define PMU_LP_ACTIVE_ANALOG_CONFIG_DEFAULT() { \
    .regulator0 = {         \
        .slp_xpd    = 0,    \
        .slp_dbias  = 0  \
    }, \
    .regulator1 = {         \
        .drv_b      = 0x0     \
    } \
}

#define PMU_LP_SLEEP_ANALOG_CONFIG_DEFAULT() { \
    .bias = {               \
        .xpd_bias   = 0,    \
        .dbg_atten  = 0,  \
        .pd_cur     = 1,    \
        .bias_sleep = 1,    \
    }, \
    .regulator0 = {         \
        .slp_xpd    = 0,    \
        .slp_dbias  = 0  \
    }, \
    .regulator1 = {         \
        .drv_b      = 0x0     \
    } \
}

const pmu_lp_system_analog_param_t * pmu_lp_system_analog_param_default(pmu_lp_mode_t mode)
{
    static const pmu_lp_system_analog_param_t lp_analog[] = {
        PMU_LP_ACTIVE_ANALOG_CONFIG_DEFAULT(),
        PMU_LP_SLEEP_ANALOG_CONFIG_DEFAULT()
    };
    assert(mode < ARRAY_SIZE(lp_analog));
    return &lp_analog[mode];
}

uint32_t get_act_hp_dbias(void)
{
    /* hp_cali_dbias is read from efuse to ensure that the hp_active_voltage is close to 1.15V
    */
    uint32_t hp_cali_dbias = HP_CALI_ACTIVE_DBIAS_DEFAULT;
    uint32_t blk_version = efuse_hal_blk_version();
    uint32_t hp_cali_dbias_efuse = 0;
    if (blk_version >= 2) {
        hp_cali_dbias_efuse = efuse_ll_get_active_hp_dbias();
    }
    if (hp_cali_dbias_efuse > 0) {
        hp_cali_dbias = hp_cali_dbias_efuse + 16;
        if (hp_cali_dbias > 31) {
            hp_cali_dbias = 31;
        }
    } else {
        ESP_HW_LOGD(TAG, "hp_cali_dbias not burnt in efuse, use default.");
    }
    return hp_cali_dbias;
}

uint32_t get_act_lp_dbias(void)
{
    /* lp_cali_dbias is read from efuse to ensure that the lp_active_voltage is close to 1.15V
    */
    uint32_t lp_cali_dbias = LP_CALI_ACTIVE_DBIAS_DEFAULT;
    uint32_t blk_version = efuse_hal_blk_version();
    uint32_t lp_cali_dbias_efuse = 0;
    if (blk_version >= 2) {
        lp_cali_dbias_efuse = efuse_ll_get_active_lp_dbias();
    }
    if (lp_cali_dbias_efuse > 0) {
        //efuse dbias need to add 4 to near to dcdc voltage
        lp_cali_dbias = lp_cali_dbias_efuse + 16 + 4;
        if (lp_cali_dbias > 31) {
            lp_cali_dbias = 31;
        }
    } else {
        ESP_HW_LOGD(TAG, "lp_cali_dbias not burnt in efuse, use default.");
    }
    return lp_cali_dbias;
}
