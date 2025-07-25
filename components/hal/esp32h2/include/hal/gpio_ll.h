/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*******************************************************************************
 * NOTICE
 * The hal is not public api, don't use in application code.
 * See readme.md in soc/include/hal/readme.md
 ******************************************************************************/

// The LL layer for ESP32-H2 GPIO register operations

#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include "soc/soc.h"
#include "soc/gpio_periph.h"
#include "soc/gpio_struct.h"
#include "soc/lp_aon_struct.h"
#include "soc/pmu_struct.h"
#include "soc/usb_serial_jtag_reg.h"
#include "soc/pcr_struct.h"
#include "soc/clk_tree_defs.h"
#include "hal/gpio_types.h"
#include "hal/misc.h"
#include "hal/assert.h"

#ifdef __cplusplus
extern "C" {
#endif

// Get GPIO hardware instance with giving gpio num
#define GPIO_LL_GET_HW(num) (((num) == 0) ? (&GPIO) : NULL)

#define GPIO_LL_PRO_CPU_INTR_ENA      (BIT(0))
#define GPIO_LL_PRO_CPU_NMI_INTR_ENA  (BIT(1))

#define GPIO_LL_INTR_SOURCE0   ETS_GPIO_INTR_SOURCE

/**
 * @brief Get the configuration for an IO
 *
 * @param hw Peripheral GPIO hardware instance address.
 * @param gpio_num GPIO number
 * @param[out] io_config Pointer to the structure that saves the specific IO configuration
 */
static inline void gpio_ll_get_io_config(gpio_dev_t *hw, uint32_t gpio_num, gpio_io_config_t *io_config)
{
    uint32_t bit_mask = 1 << gpio_num;
    uint32_t iomux_reg_val = REG_READ(IO_MUX_GPIO0_REG + (gpio_num * 4));
    io_config->pu = (iomux_reg_val & FUN_PU_M) >> FUN_PU_S;
    io_config->pd = (iomux_reg_val & FUN_PD_M) >> FUN_PD_S;
    io_config->ie = (iomux_reg_val & FUN_IE_M) >> FUN_IE_S;
    io_config->oe = (hw->enable.val & bit_mask) >> gpio_num;
    io_config->oe_ctrl_by_periph = !(hw->func_out_sel_cfg[gpio_num].oen_sel);
    io_config->oe_inv = hw->func_out_sel_cfg[gpio_num].oen_inv_sel;
    io_config->od = hw->pin[gpio_num].pad_driver;
    io_config->drv = (gpio_drive_cap_t)((iomux_reg_val & FUN_DRV_M) >> FUN_DRV_S);
    io_config->fun_sel = (iomux_reg_val & MCU_SEL_M) >> MCU_SEL_S;
    io_config->sig_out = HAL_FORCE_READ_U32_REG_FIELD(hw->func_out_sel_cfg[gpio_num], out_sel);
    io_config->slp_sel = (iomux_reg_val & SLP_SEL_M) >> SLP_SEL_S;
}

/**
  * @brief Enable pull-up on GPIO.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number
  */
static inline void gpio_ll_pullup_en(gpio_dev_t *hw, uint32_t gpio_num)
{
    REG_SET_BIT(IO_MUX_GPIO0_REG + (gpio_num * 4), FUN_PU);
}

/**
  * @brief Disable pull-up on GPIO.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number
  */
__attribute__((always_inline))
static inline void gpio_ll_pullup_dis(gpio_dev_t *hw, uint32_t gpio_num)
{
    // The pull-up value of the USB pins are controlled by the pins’ pull-up value together with USB pull-up value
    // USB DP pin is default to PU enabled
    // Note that esp32h2 has supported USB_EXCHG_PINS feature. If this efuse is burnt, the gpio pin
    // which should be checked is USB_INT_PHY0_DM_GPIO_NUM instead.
    // TODO: read the specific efuse with efuse_ll.h
    if (gpio_num == USB_INT_PHY0_DP_GPIO_NUM) {
        SET_PERI_REG_MASK(USB_SERIAL_JTAG_CONF0_REG, USB_SERIAL_JTAG_PAD_PULL_OVERRIDE);
        CLEAR_PERI_REG_MASK(USB_SERIAL_JTAG_CONF0_REG, USB_SERIAL_JTAG_DP_PULLUP);
    }
    REG_CLR_BIT(IO_MUX_GPIO0_REG + (gpio_num * 4), FUN_PU);
}

/**
  * @brief Enable pull-down on GPIO.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number
  */
static inline void gpio_ll_pulldown_en(gpio_dev_t *hw, uint32_t gpio_num)
{
    REG_SET_BIT(IO_MUX_GPIO0_REG + (gpio_num * 4), FUN_PD);
}

/**
  * @brief Disable pull-down on GPIO.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number
  */
__attribute__((always_inline))
static inline void gpio_ll_pulldown_dis(gpio_dev_t *hw, uint32_t gpio_num)
{

    REG_CLR_BIT(IO_MUX_GPIO0_REG + (gpio_num * 4), FUN_PD);
}

/**
 * @brief  GPIO set interrupt trigger type
 *
 * @param  hw Peripheral GPIO hardware instance address.
 * @param  gpio_num GPIO number. If you want to set the trigger type of e.g. of GPIO16, gpio_num should be GPIO_NUM_16 (16);
 * @param  intr_type Interrupt type, select from gpio_int_type_t
 */
static inline void gpio_ll_set_intr_type(gpio_dev_t *hw, uint32_t gpio_num, gpio_int_type_t intr_type)
{
    hw->pin[gpio_num].int_type = intr_type;
}

/**
  * @brief Get GPIO interrupt status
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param core_id interrupt core id
  * @param status interrupt status
  */
__attribute__((always_inline))
static inline void gpio_ll_get_intr_status(gpio_dev_t *hw, uint32_t core_id, uint32_t *status)
{
    (void)core_id;
    *status = hw->pcpu_int.procpu_int;
}

/**
  * @brief Get GPIO interrupt status high
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param core_id interrupt core id
  * @param status interrupt status high
  */
__attribute__((always_inline))
static inline void gpio_ll_get_intr_status_high(gpio_dev_t *hw, uint32_t core_id, uint32_t *status)
{
    *status = 0; // Less than 32 GPIOs in ESP32-H2
}

/**
  * @brief Clear GPIO interrupt status
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param mask interrupt status clear mask
  */
__attribute__((always_inline))
static inline void gpio_ll_clear_intr_status(gpio_dev_t *hw, uint32_t mask)
{
    hw->status_w1tc.status_w1tc = mask;
}

/**
  * @brief Clear GPIO interrupt status high
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param mask interrupt status high clear mask
  */
__attribute__((always_inline))
static inline void gpio_ll_clear_intr_status_high(gpio_dev_t *hw, uint32_t mask)
{
    // Less than 32 GPIOs on ESP32-H2 Do nothing.
}

/**
 * @brief  Enable GPIO module interrupt signal
 *
 * @param  hw Peripheral GPIO hardware instance address.
 * @param  core_id Interrupt enabled CPU to corresponding ID
 * @param  gpio_num GPIO number. If you want to enable the interrupt of e.g. GPIO16, gpio_num should be GPIO_NUM_16 (16);
 */
__attribute__((always_inline))
static inline void gpio_ll_intr_enable_on_core(gpio_dev_t *hw, uint32_t core_id, uint32_t gpio_num)
{
    HAL_ASSERT(core_id == 0 && "target SoC only has a single core");
    GPIO.pin[gpio_num].int_ena = GPIO_LL_PRO_CPU_INTR_ENA;     //enable pro cpu intr
}

/**
 * @brief  Disable GPIO module interrupt signal
 *
 * @param  hw Peripheral GPIO hardware instance address.
 * @param  gpio_num GPIO number. If you want to disable the interrupt of e.g. GPIO16, gpio_num should be GPIO_NUM_16 (16);
 */
__attribute__((always_inline))
static inline void gpio_ll_intr_disable(gpio_dev_t *hw, uint32_t gpio_num)
{
    hw->pin[gpio_num].int_ena = 0;                             //disable GPIO intr
}

/**
  * @brief Disable input mode on GPIO.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number
  */
__attribute__((always_inline))
static inline void gpio_ll_input_disable(gpio_dev_t *hw, uint32_t gpio_num)
{
    PIN_INPUT_DISABLE(IO_MUX_GPIO0_REG + (gpio_num * 4));
}

/**
  * @brief Enable input mode on GPIO.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number
  */
__attribute__((always_inline))
static inline void gpio_ll_input_enable(gpio_dev_t *hw, uint32_t gpio_num)
{
    PIN_INPUT_ENABLE(IO_MUX_GPIO0_REG + (gpio_num * 4));
}

/**
 * @brief Enable GPIO pin filter
 *
 * @param hw Peripheral GPIO hardware instance address.
 * @param gpio_num GPIO number of the pad.
 */
static inline void gpio_ll_pin_filter_enable(gpio_dev_t *hw, uint32_t gpio_num)
{
    PIN_FILTER_EN(IO_MUX_GPIO0_REG + (gpio_num * 4));
}

/**
 * @brief Disable GPIO pin filter
 *
 * @param hw Peripheral GPIO hardware instance address.
 * @param gpio_num GPIO number of the pad.
 */
static inline void gpio_ll_pin_filter_disable(gpio_dev_t *hw, uint32_t gpio_num)
{
    PIN_FILTER_DIS(IO_MUX_GPIO0_REG + (gpio_num * 4));
}

/**
  * @brief Select gpio hysteresis control by efuse.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number
  */
static inline void gpio_ll_pin_input_hysteresis_ctrl_sel_efuse(gpio_dev_t *hw, uint32_t gpio_num)
{
    PIN_HYS_EN_SEL_EFUSE(IO_MUX_GPIO0_REG + (gpio_num * 4));
}

/**
  * @brief Select gpio hysteresis control by software.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number
  */
static inline void gpio_ll_pin_input_hysteresis_ctrl_sel_soft(gpio_dev_t *hw, uint32_t gpio_num)
{
    PIN_HYS_EN_SEL_SOFT(IO_MUX_GPIO0_REG + (gpio_num * 4));
}

/**
  * @brief Enable gpio hysteresis if controlled by software.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number
  */
static inline void gpio_ll_pin_input_hysteresis_enable(gpio_dev_t *hw, uint32_t gpio_num)
{
    PIN_HYS_SOFT_ENABLE(IO_MUX_GPIO0_REG + (gpio_num * 4));
}

/**
  * @brief Disable gpio hysteresis if controlled by software.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number
  */
static inline void gpio_ll_pin_input_hysteresis_disable(gpio_dev_t *hw, uint32_t gpio_num)
{
    PIN_HYS_SOFT_DISABLE(IO_MUX_GPIO0_REG + (gpio_num * 4));
}

/**
  * @brief Disable output mode on GPIO.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number
  */
__attribute__((always_inline))
static inline void gpio_ll_output_disable(gpio_dev_t *hw, uint32_t gpio_num)
{
    hw->enable_w1tc.enable_w1tc = (0x1 << gpio_num);
}

/**
  * @brief Enable output mode on GPIO.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number
  */
__attribute__((always_inline))
static inline void gpio_ll_output_enable(gpio_dev_t *hw, uint32_t gpio_num)
{
    hw->enable_w1ts.enable_w1ts = (0x1 << gpio_num);
}

/**
  * @brief Disable open-drain mode on GPIO.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number
  */
static inline void gpio_ll_od_disable(gpio_dev_t *hw, uint32_t gpio_num)
{
    hw->pin[gpio_num].pad_driver = 0;
}

/**
  * @brief Enable open-drain mode on GPIO.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number
  */
static inline void gpio_ll_od_enable(gpio_dev_t *hw, uint32_t gpio_num)
{
    hw->pin[gpio_num].pad_driver = 1;
}

/**
 * @brief  GPIO set output level
 *
 * @param  hw Peripheral GPIO hardware instance address.
 * @param  gpio_num GPIO number. If you want to set the output level of e.g. GPIO16, gpio_num should be GPIO_NUM_16 (16);
 * @param  level Output level. 0: low ; 1: high
 */
__attribute__((always_inline))
static inline void gpio_ll_set_level(gpio_dev_t *hw, uint32_t gpio_num, uint32_t level)
{
    if (level) {
        hw->out_w1ts.val = 1 << gpio_num;
    } else {
        hw->out_w1tc.val = 1 << gpio_num;
    }
}

/**
 * @brief  GPIO get input level
 *
 * @warning If the pad is not configured for input (or input and output) the returned value is always 0.
 *
 * @param  hw Peripheral GPIO hardware instance address.
 * @param  gpio_num GPIO number. If you want to get the logic level of e.g. pin GPIO16, gpio_num should be GPIO_NUM_16 (16);
 *
 * @return
 *     - 0 the GPIO input level is 0
 *     - 1 the GPIO input level is 1
 */
__attribute__((always_inline))
static inline int gpio_ll_get_level(gpio_dev_t *hw, uint32_t gpio_num)
{
    return (hw->in.in_data_next >> gpio_num) & 0x1;
}

/**
 * @brief Enable GPIO wake-up function.
 *
 * @param hw Peripheral GPIO hardware instance address.
 * @param gpio_num GPIO number.
 * @param intr_type GPIO wake-up type. Only GPIO_INTR_LOW_LEVEL or GPIO_INTR_HIGH_LEVEL can be used.
 */
static inline void gpio_ll_wakeup_enable(gpio_dev_t *hw, uint32_t gpio_num)
{
    hw->pin[gpio_num].wakeup_enable = 1;
}

/**
 * @brief Disable GPIO wake-up function.
 *
 * @param hw Peripheral GPIO hardware instance address.
 * @param gpio_num GPIO number
 */
static inline void gpio_ll_wakeup_disable(gpio_dev_t *hw, uint32_t gpio_num)
{
    hw->pin[gpio_num].wakeup_enable = 0;
}

/**
  * @brief Set GPIO pad drive capability
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number, only support output GPIOs
  * @param strength Drive capability of the pad
  */
static inline void gpio_ll_set_drive_capability(gpio_dev_t *hw, uint32_t gpio_num, gpio_drive_cap_t strength)
{
    SET_PERI_REG_BITS(IO_MUX_GPIO0_REG + (gpio_num * 4), FUN_DRV_V, strength, FUN_DRV_S);
}

/**
  * @brief Get GPIO pad drive capability
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number, only support output GPIOs
  * @param strength Pointer to accept drive capability of the pad
  */
static inline void gpio_ll_get_drive_capability(gpio_dev_t *hw, uint32_t gpio_num, gpio_drive_cap_t *strength)
{
    *strength = (gpio_drive_cap_t)GET_PERI_REG_BITS2(IO_MUX_GPIO0_REG + (gpio_num * 4), FUN_DRV_V, FUN_DRV_S);
}

/**
  * @brief Enable gpio pad hold function.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number, only support output GPIOs
  */
__attribute__((always_inline))
static inline void gpio_ll_hold_en(gpio_dev_t *hw, uint32_t gpio_num)
{
    LP_AON.gpio_hold0.gpio_hold0 |= GPIO_HOLD_MASK[gpio_num];
}

/**
  * @brief Disable gpio pad hold function.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number, only support output GPIOs
  */
__attribute__((always_inline))
static inline void gpio_ll_hold_dis(gpio_dev_t *hw, uint32_t gpio_num)
{
    LP_AON.gpio_hold0.gpio_hold0 &= ~GPIO_HOLD_MASK[gpio_num];
}

/**
  * @brief Get digital gpio pad hold status.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number, only support output GPIOs
  *
  * @note caller must ensure that gpio_num is a digital io pad
  *
  * @return
  *     - true  digital gpio pad is held
  *     - false digital gpio pad is unheld
  */
__attribute__((always_inline))
static inline bool gpio_ll_is_digital_io_hold(gpio_dev_t *hw, uint32_t gpio_num)
{
    return !!(LP_AON.gpio_hold0.gpio_hold0 & BIT(gpio_num));
}

/**
  * @brief Configure peripheral signal input whether to bypass GPIO matrix.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param signal_idx Peripheral signal id to input. One of the ``*_IN_IDX`` signals in ``soc/gpio_sig_map.h``.
  * @param from_gpio_matrix True if not to bypass GPIO matrix, otherwise False.
  */
__attribute__((always_inline))
static inline void gpio_ll_set_input_signal_from(gpio_dev_t *hw, uint32_t signal_idx, bool from_gpio_matrix)
{
    hw->func_in_sel_cfg[signal_idx].sig_in_sel = from_gpio_matrix;
}

/**
 * @brief Connect a GPIO input with a peripheral signal, which tagged as input attribute.
 *
 * @note There's no limitation on the number of signals that a GPIO can combine with.
 *
 * @param signal_idx Peripheral signal index (tagged as input attribute)
 * @param gpio_num GPIO number, especially, `GPIO_MATRIX_CONST_ZERO_INPUT` means connect logic 0 to signal
 *                                          `GPIO_MATRIX_CONST_ONE_INPUT` means connect logic 1 to signal
 * @param in_inv True if the GPIO input needs to be inverted, otherwise False.
 */
static inline void gpio_ll_set_input_signal_matrix_source(gpio_dev_t *hw, uint32_t signal_idx, uint32_t gpio_num, bool in_inv)
{
    hw->func_in_sel_cfg[signal_idx].in_sel = gpio_num;
    hw->func_in_sel_cfg[signal_idx].in_inv_sel = in_inv;
    gpio_ll_set_input_signal_from(hw, signal_idx, true);
}

/**
 * @brief Get the GPIO number that is routed to the input peripheral signal through GPIO matrix.
 *
 * @param hw Peripheral GPIO hardware instance address.
 * @param in_sig_idx Peripheral signal index (tagged as input attribute).
 *
 * @return
 *    - -1     Signal bypassed GPIO matrix
 *    - Others GPIO number
 */
static inline int gpio_ll_get_in_signal_connected_io(gpio_dev_t *hw, uint32_t in_sig_idx)
{
    gpio_func_in_sel_cfg_reg_t reg;
    reg.val = hw->func_in_sel_cfg[in_sig_idx].val;
    return (reg.sig_in_sel ? reg.in_sel : -1);
}

/**
  * @brief Configure the source of output enable signal for the GPIO pin.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number of the pad.
  * @param ctrl_by_periph True if use output enable signal from peripheral, false if force the output enable signal to be sourced from bit n of GPIO_ENABLE_REG
  * @param oen_inv True if the output enable needs to be inverted, otherwise False.
  */
static inline void gpio_ll_set_output_enable_ctrl(gpio_dev_t *hw, uint8_t gpio_num, bool ctrl_by_periph, bool oen_inv)
{
    hw->func_out_sel_cfg[gpio_num].oen_inv_sel = oen_inv;       // control valid only when using gpio matrix to route signal to the IO
    hw->func_out_sel_cfg[gpio_num].oen_sel = !ctrl_by_periph;
}

/**
 * @brief Connect a peripheral signal which tagged as output attribute with a GPIO.
 *
 * @note There's no limitation on the number of signals that a GPIO can combine with.
 *
 * @param gpio_num GPIO number
 * @param signal_idx Peripheral signal index (tagged as output attribute). Particularly, `SIG_GPIO_OUT_IDX` means disconnect GPIO and other peripherals. Only the GPIO driver can control the output level.
 * @param out_inv True if the signal output needs to be inverted, otherwise False.
 */
static inline void gpio_ll_set_output_signal_matrix_source(gpio_dev_t *hw, uint32_t gpio_num, uint32_t signal_idx, bool out_inv)
{
    HAL_FORCE_MODIFY_U32_REG_FIELD(hw->func_out_sel_cfg[gpio_num], out_sel, signal_idx);
    hw->func_out_sel_cfg[gpio_num].out_inv_sel = out_inv;
}

/**
 * @brief  Select a function for the pin in the IOMUX
 *
 * @param  hw Peripheral GPIO hardware instance address.
 * @param  gpio_num GPIO number
 * @param  func Function to assign to the pin
 */
__attribute__((always_inline))
static inline void gpio_ll_func_sel(gpio_dev_t *hw, uint8_t gpio_num, uint32_t func)
{
    // Disable USB Serial JTAG if pins 26 or pins 27 needs to select an IOMUX function
    if (gpio_num == USB_INT_PHY0_DM_GPIO_NUM || gpio_num == USB_INT_PHY0_DP_GPIO_NUM) {
        CLEAR_PERI_REG_MASK(USB_SERIAL_JTAG_CONF0_REG, USB_SERIAL_JTAG_USB_PAD_ENABLE);
    }
    PIN_FUNC_SELECT(IO_MUX_GPIO0_REG + (gpio_num * 4), func);
}

/**
 * @brief  Control the pin in the IOMUX
 *
 * @param  bmap   write mask of control value
 * @param  val    Control value
 * @param  shift  write mask shift of control value
 */
__attribute__((always_inline))
static inline void gpio_ll_set_pin_ctrl(uint32_t val, uint32_t bmap, uint32_t shift)
{
    SET_PERI_REG_BITS(PIN_CTRL, bmap, val, shift);
}

/**
 * @brief Set clock source of IO MUX module
 *
 * @param src IO MUX clock source (only a subset of soc_module_clk_t values are valid)
 */
static inline void gpio_ll_iomux_set_clk_src(soc_module_clk_t src)
{
    switch (src) {
    case SOC_MOD_CLK_XTAL:
        PCR.iomux_clk_conf.iomux_func_clk_sel = 0;
        break;
    case SOC_MOD_CLK_PLL_F48M:
        PCR.iomux_clk_conf.iomux_func_clk_sel = 2;
        break;
    default:
        // Unsupported IO_MUX clock source
        HAL_ASSERT(false);
    }
}

/**
  * @brief Force hold all digital(VDDPST2) and lp(VDDPST1) io pads.
  * @note GPIO force hold, whether the chip in sleep mode or wakeup mode.
  */
static inline void gpio_ll_force_hold_all(void)
{
    // WT flags, they get self-cleared after the configuration is done
    PMU.imm.pad_hold_all.tie_high_hp_pad_hold_all = 1;
    PMU.imm.pad_hold_all.tie_high_lp_pad_hold_all = 1;
}

/**
  * @brief Force unhold all digital(VDDPST2) and lp(VDDPST1) io pads.
  * @note GPIO force unhold, whether the chip in sleep mode or wakeup mode.
  */
static inline void gpio_ll_force_unhold_all(void)
{
    // WT flags, they get self-cleared after the configuration is done
    PMU.imm.pad_hold_all.tie_low_hp_pad_hold_all = 1;
    PMU.imm.pad_hold_all.tie_low_lp_pad_hold_all = 1;
}

/**
  * @brief Enable GPIO pin to use sleep mode pin functions during light sleep.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number
  */
__attribute__((always_inline))
static inline void gpio_ll_sleep_sel_en(gpio_dev_t *hw, uint32_t gpio_num)
{
    PIN_SLP_SEL_ENABLE(IO_MUX_GPIO0_REG + (gpio_num * 4));
}

/**
  * @brief Disable GPIO pin to use sleep mode pin functions during light sleep.
  * Pin functions remains the same in both normal execution and in light-sleep mode.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number
  */
__attribute__((always_inline))
static inline void gpio_ll_sleep_sel_dis(gpio_dev_t *hw, uint32_t gpio_num)
{
    PIN_SLP_SEL_DISABLE(IO_MUX_GPIO0_REG + (gpio_num * 4));
}

/**
  * @brief Disable GPIO pull-up in sleep mode.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number
  */
__attribute__((always_inline))
static inline void gpio_ll_sleep_pullup_dis(gpio_dev_t *hw, uint32_t gpio_num)
{
    PIN_SLP_PULLUP_DISABLE(IO_MUX_GPIO0_REG + (gpio_num * 4));
}

/**
  * @brief Enable GPIO pull-up in sleep mode.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number
  */
__attribute__((always_inline))
static inline void gpio_ll_sleep_pullup_en(gpio_dev_t *hw, uint32_t gpio_num)
{
    PIN_SLP_PULLUP_ENABLE(IO_MUX_GPIO0_REG + (gpio_num * 4));
}

/**
  * @brief Enable GPIO pull-down in sleep mode.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number
  */
__attribute__((always_inline))
static inline void gpio_ll_sleep_pulldown_en(gpio_dev_t *hw, uint32_t gpio_num)
{
    PIN_SLP_PULLDOWN_ENABLE(IO_MUX_GPIO0_REG + (gpio_num * 4));
}

/**
  * @brief Disable GPIO pull-down in sleep mode.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number
  */
__attribute__((always_inline))
static inline void gpio_ll_sleep_pulldown_dis(gpio_dev_t *hw, uint32_t gpio_num)
{
    PIN_SLP_PULLDOWN_DISABLE(IO_MUX_GPIO0_REG + (gpio_num * 4));
}

/**
  * @brief Disable GPIO input in sleep mode.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number
  */
__attribute__((always_inline))
static inline void gpio_ll_sleep_input_disable(gpio_dev_t *hw, uint32_t gpio_num)
{
    PIN_SLP_INPUT_DISABLE(IO_MUX_GPIO0_REG + (gpio_num * 4));
}

/**
  * @brief Enable GPIO input in sleep mode.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number
  */
__attribute__((always_inline))
static inline void gpio_ll_sleep_input_enable(gpio_dev_t *hw, uint32_t gpio_num)
{
    PIN_SLP_INPUT_ENABLE(IO_MUX_GPIO0_REG + (gpio_num * 4));
}

/**
  * @brief Disable GPIO output in sleep mode.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number
  */
__attribute__((always_inline))
static inline void gpio_ll_sleep_output_disable(gpio_dev_t *hw, uint32_t gpio_num)
{
    PIN_SLP_OUTPUT_DISABLE(IO_MUX_GPIO0_REG + (gpio_num * 4));
}

/**
  * @brief Enable GPIO output in sleep mode.
  *
  * @param hw Peripheral GPIO hardware instance address.
  * @param gpio_num GPIO number
  */
__attribute__((always_inline))
static inline void gpio_ll_sleep_output_enable(gpio_dev_t *hw, uint32_t gpio_num)
{
    PIN_SLP_OUTPUT_ENABLE(IO_MUX_GPIO0_REG + (gpio_num * 4));
}

/**
 * @brief Enable GPIO deep-sleep wake-up function.
 *
 * @param hw Peripheral GPIO hardware instance address.
 * @param gpio_num GPIO number.
 * @param intr_type GPIO wake-up type. Only GPIO_INTR_LOW_LEVEL or GPIO_INTR_HIGH_LEVEL can be used.
 */
static inline void gpio_ll_deepsleep_wakeup_enable(gpio_dev_t *hw, uint32_t gpio_num, gpio_int_type_t intr_type)
{
    HAL_ASSERT((gpio_num >= GPIO_NUM_7 && gpio_num <= GPIO_NUM_14) &&
               "only gpio7~14 support deep sleep wake-up function");

    LP_AON.ext_wakeup_cntl.ext_wakeup_filter = 1;

    uint32_t wakeup_sel_mask = HAL_FORCE_READ_U32_REG_FIELD(LP_AON.ext_wakeup_cntl, ext_wakeup_sel);
    wakeup_sel_mask |= BIT(gpio_num - 7);
    HAL_FORCE_MODIFY_U32_REG_FIELD(LP_AON.ext_wakeup_cntl, ext_wakeup_sel, wakeup_sel_mask);

    bool trigger_level = (intr_type == GPIO_INTR_LOW_LEVEL) ? 0 : 1;
    uint32_t wakeup_level_mask = HAL_FORCE_READ_U32_REG_FIELD(LP_AON.ext_wakeup_cntl, ext_wakeup_lv);
    if (trigger_level) {
        wakeup_level_mask |= BIT(gpio_num - 7);
    } else {
        wakeup_level_mask &= ~BIT(gpio_num - 7);
    }
    HAL_FORCE_MODIFY_U32_REG_FIELD(LP_AON.ext_wakeup_cntl, ext_wakeup_lv, wakeup_level_mask);
}

/**
 * @brief Disable GPIO deep-sleep wake-up function.
 *
 * @param hw Peripheral GPIO hardware instance address.
 * @param gpio_num GPIO number
 */
static inline void gpio_ll_deepsleep_wakeup_disable(gpio_dev_t *hw, uint32_t gpio_num)
{
    HAL_ASSERT((gpio_num >= GPIO_NUM_7 && gpio_num <= GPIO_NUM_14) &&
               "only gpio7~14 support deep sleep wake-up function");

    uint32_t wakeup_sel_mask = HAL_FORCE_READ_U32_REG_FIELD(LP_AON.ext_wakeup_cntl, ext_wakeup_sel);
    wakeup_sel_mask &= ~BIT(gpio_num - 7);
    HAL_FORCE_MODIFY_U32_REG_FIELD(LP_AON.ext_wakeup_cntl, ext_wakeup_sel, wakeup_sel_mask);
}

/**
 * @brief Get the status of whether an IO is used for deep-sleep wake-up.
 *
 * @param hw Peripheral GPIO hardware instance address.
 * @param gpio_num GPIO number
 * @return True if the pin is enabled to wake up from deep-sleep
 */
static inline bool gpio_ll_deepsleep_wakeup_is_enabled(gpio_dev_t *hw, uint32_t gpio_num)
{
    HAL_ASSERT((gpio_num >= GPIO_NUM_7 && gpio_num <= GPIO_NUM_14) &&
               "only gpio7~14 support deep sleep wake-up function");

    uint32_t wakeup_sel_mask = HAL_FORCE_READ_U32_REG_FIELD(LP_AON.ext_wakeup_cntl, ext_wakeup_sel);
    return wakeup_sel_mask & BIT(gpio_num - 7);
}

#ifdef __cplusplus
}
#endif
