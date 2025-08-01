/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * Background
 *
 * This file is for the MSPI related, but not Flash driver related registers, these registers:
 * - may influence both Flash and PSRAM
 * - not related or directly related to Flash controller driver
 *
 * Some hints for naming convention:
 * - For MSPI timing tuning related registers, the LL should start with `mspi_timing_ll_`
 * - For others, the LL should start with `mspi_ll_`
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "soc/soc.h"
#include "soc/clk_tree_defs.h"
#include "soc/pcr_struct.h"
#include "hal/misc.h"
#include "hal/assert.h"

//TODO: [ESP32H4] IDF-12390 inherited from verification branch, need check

#ifdef __cplusplus
extern "C" {
#endif

/************************** MSPI pll clock configurations **************************/
/*
 * @brief Select FLASH clock source
 *
 * @param mspi_id      mspi_id
 * @param clk_src      clock source, see valid sources in type `soc_periph_flash_clk_src_t`
 */
__attribute__((always_inline))
static inline void _mspi_timing_ll_set_flash_clk_src(uint32_t mspi_id, soc_periph_flash_clk_src_t clk_src)
{
    HAL_ASSERT(mspi_id == 0);
    switch (clk_src) {
    case FLASH_CLK_SRC_XTAL:
        PCR.mspi_clk_conf.mspi_func_clk_sel = 0;
        break;
    case FLASH_CLK_SRC_RC_FAST:
        PCR.mspi_clk_conf.mspi_func_clk_sel = 1;
        break;
    // case FLASH_CLK_SRC_PLL_F64M:
    //     PCR.mspi_clk_conf.mspi_func_clk_sel = 2;
    //     break;
    // TODO: [ESP32H4] IDF-13632, support 64M
    case FLASH_CLK_SRC_PLL_F48M:
        PCR.mspi_clk_conf.mspi_func_clk_sel = 3;
        break;
    default:
        HAL_ASSERT(false);
    }
}

#ifdef __cplusplus
}
#endif
