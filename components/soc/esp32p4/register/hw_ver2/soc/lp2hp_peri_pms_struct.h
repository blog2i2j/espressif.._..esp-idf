/**
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 *  SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/** Group: TEE LP2HP PMS DATE REG */
/** Type of lp2hp_pms_date register
 *  NA
 */
typedef union {
    struct {
        /** tee_date : R/W; bitpos: [31:0]; default: 2363943;
         *  NA
         */
        uint32_t tee_date:32;
    };
    uint32_t val;
} tee_lp2hp_pms_date_reg_t;


/** Group: TEE PMS CLK EN REG */
/** Type of pms_clk_en register
 *  NA
 */
typedef union {
    struct {
        /** reg_clk_en : R/W; bitpos: [0]; default: 1;
         *  NA
         */
        uint32_t reg_clk_en:1;
        uint32_t reserved_1:31;
    };
    uint32_t val;
} tee_pms_clk_en_reg_t;


/** Group: TEE LP MM PMS REG0 REG */
/** Type of lp_mm_pms_reg0 register
 *  NA
 */
typedef union {
    struct {
        /** reg_lp_mm_psram_allow : R/W; bitpos: [0]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_psram_allow:1;
        /** reg_lp_mm_flash_allow : R/W; bitpos: [1]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_flash_allow:1;
        /** reg_lp_mm_l2mem_allow : R/W; bitpos: [2]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_l2mem_allow:1;
        /** reg_lp_mm_l2rom_allow : R/W; bitpos: [3]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_l2rom_allow:1;
        uint32_t reserved_4:2;
        /** reg_lp_mm_trace0_allow : R/W; bitpos: [6]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_trace0_allow:1;
        /** reg_lp_mm_trace1_allow : R/W; bitpos: [7]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_trace1_allow:1;
        /** reg_lp_mm_cpu_bus_mon_allow : R/W; bitpos: [8]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_cpu_bus_mon_allow:1;
        /** reg_lp_mm_l2mem_mon_allow : R/W; bitpos: [9]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_l2mem_mon_allow:1;
        /** reg_lp_mm_tcm_mon_allow : R/W; bitpos: [10]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_tcm_mon_allow:1;
        /** reg_lp_mm_cache_allow : R/W; bitpos: [11]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_cache_allow:1;
        uint32_t reserved_12:20;
    };
    uint32_t val;
} tee_lp_mm_pms_reg0_reg_t;


/** Group: TEE LP MM PMS REG1 REG */
/** Type of lp_mm_pms_reg1 register
 *  NA
 */
typedef union {
    struct {
        /** reg_lp_mm_hp_usbotg_allow : R/W; bitpos: [0]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_usbotg_allow:1;
        /** reg_lp_mm_hp_usbotg11_allow : R/W; bitpos: [1]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_usbotg11_allow:1;
        /** reg_lp_mm_hp_usbotg11_wrap_allow : R/W; bitpos: [2]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_usbotg11_wrap_allow:1;
        /** reg_lp_mm_hp_gdma_allow : R/W; bitpos: [3]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_gdma_allow:1;
        /** reg_lp_mm_hp_regdma_allow : R/W; bitpos: [4]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_regdma_allow:1;
        /** reg_lp_mm_hp_sdmmc_allow : R/W; bitpos: [5]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_sdmmc_allow:1;
        /** reg_lp_mm_hp_ahb_pdma_allow : R/W; bitpos: [6]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_ahb_pdma_allow:1;
        /** reg_lp_mm_hp_jpeg_allow : R/W; bitpos: [7]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_jpeg_allow:1;
        /** reg_lp_mm_hp_ppa_allow : R/W; bitpos: [8]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_ppa_allow:1;
        /** reg_lp_mm_hp_dma2d_allow : R/W; bitpos: [9]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_dma2d_allow:1;
        /** reg_lp_mm_hp_key_manager_allow : R/W; bitpos: [10]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_key_manager_allow:1;
        /** reg_lp_mm_hp_axi_pdma_allow : R/W; bitpos: [11]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_axi_pdma_allow:1;
        /** reg_lp_mm_hp_flash_allow : R/W; bitpos: [12]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_flash_allow:1;
        /** reg_lp_mm_hp_psram_allow : R/W; bitpos: [13]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_psram_allow:1;
        /** reg_lp_mm_hp_crypto_allow : R/W; bitpos: [14]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_crypto_allow:1;
        /** reg_lp_mm_hp_gmac_allow : R/W; bitpos: [15]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_gmac_allow:1;
        /** reg_lp_mm_hp_usb_phy_allow : R/W; bitpos: [16]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_usb_phy_allow:1;
        /** reg_lp_mm_hp_pvt_allow : R/W; bitpos: [17]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_pvt_allow:1;
        /** reg_lp_mm_hp_csi_host_allow : R/W; bitpos: [18]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_csi_host_allow:1;
        /** reg_lp_mm_hp_dsi_host_allow : R/W; bitpos: [19]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_dsi_host_allow:1;
        /** reg_lp_mm_hp_isp_allow : R/W; bitpos: [20]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_isp_allow:1;
        /** reg_lp_mm_hp_h264_core_allow : R/W; bitpos: [21]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_h264_core_allow:1;
        /** reg_lp_mm_hp_rmt_allow : R/W; bitpos: [22]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_rmt_allow:1;
        /** reg_lp_mm_hp_bitsrambler_allow : R/W; bitpos: [23]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_bitsrambler_allow:1;
        /** reg_lp_mm_hp_axi_icm_allow : R/W; bitpos: [24]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_axi_icm_allow:1;
        /** reg_lp_mm_hp_peri_pms_allow : R/W; bitpos: [25]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_peri_pms_allow:1;
        /** reg_lp_mm_lp2hp_peri_pms_allow : R/W; bitpos: [26]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_lp2hp_peri_pms_allow:1;
        /** reg_lp_mm_dma_pms_allow : R/W; bitpos: [27]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_dma_pms_allow:1;
        /** reg_lp_mm_hp_h264_dma2d_allow : R/W; bitpos: [28]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_h264_dma2d_allow:1;
        /** reg_lp_mm_axi_perf_mon_allow : R/W; bitpos: [29]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_axi_perf_mon_allow:1;
        uint32_t reserved_30:2;
    };
    uint32_t val;
} tee_lp_mm_pms_reg1_reg_t;


/** Group: TEE LP MM PMS REG2 REG */
/** Type of lp_mm_pms_reg2 register
 *  NA
 */
typedef union {
    struct {
        /** reg_lp_mm_hp_mcpwm0_allow : R/W; bitpos: [0]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_mcpwm0_allow:1;
        /** reg_lp_mm_hp_mcpwm1_allow : R/W; bitpos: [1]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_mcpwm1_allow:1;
        /** reg_lp_mm_hp_timer_group0_allow : R/W; bitpos: [2]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_timer_group0_allow:1;
        /** reg_lp_mm_hp_timer_group1_allow : R/W; bitpos: [3]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_timer_group1_allow:1;
        /** reg_lp_mm_hp_i2c0_allow : R/W; bitpos: [4]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_i2c0_allow:1;
        /** reg_lp_mm_hp_i2c1_allow : R/W; bitpos: [5]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_i2c1_allow:1;
        /** reg_lp_mm_hp_i2s0_allow : R/W; bitpos: [6]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_i2s0_allow:1;
        /** reg_lp_mm_hp_i2s1_allow : R/W; bitpos: [7]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_i2s1_allow:1;
        /** reg_lp_mm_hp_i2s2_allow : R/W; bitpos: [8]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_i2s2_allow:1;
        /** reg_lp_mm_hp_pcnt_allow : R/W; bitpos: [9]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_pcnt_allow:1;
        /** reg_lp_mm_hp_uart0_allow : R/W; bitpos: [10]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_uart0_allow:1;
        /** reg_lp_mm_hp_uart1_allow : R/W; bitpos: [11]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_uart1_allow:1;
        /** reg_lp_mm_hp_uart2_allow : R/W; bitpos: [12]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_uart2_allow:1;
        /** reg_lp_mm_hp_uart3_allow : R/W; bitpos: [13]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_uart3_allow:1;
        /** reg_lp_mm_hp_uart4_allow : R/W; bitpos: [14]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_uart4_allow:1;
        /** reg_lp_mm_hp_parlio_allow : R/W; bitpos: [15]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_parlio_allow:1;
        /** reg_lp_mm_hp_gpspi2_allow : R/W; bitpos: [16]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_gpspi2_allow:1;
        /** reg_lp_mm_hp_gpspi3_allow : R/W; bitpos: [17]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_gpspi3_allow:1;
        /** reg_lp_mm_hp_usbdevice_allow : R/W; bitpos: [18]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_usbdevice_allow:1;
        /** reg_lp_mm_hp_ledc_allow : R/W; bitpos: [19]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_ledc_allow:1;
        uint32_t reserved_20:1;
        /** reg_lp_mm_hp_etm_allow : R/W; bitpos: [21]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_etm_allow:1;
        /** reg_lp_mm_hp_intrmtx_allow : R/W; bitpos: [22]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_intrmtx_allow:1;
        /** reg_lp_mm_hp_twai0_allow : R/W; bitpos: [23]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_twai0_allow:1;
        /** reg_lp_mm_hp_twai1_allow : R/W; bitpos: [24]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_twai1_allow:1;
        /** reg_lp_mm_hp_twai2_allow : R/W; bitpos: [25]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_twai2_allow:1;
        /** reg_lp_mm_hp_i3c_mst_allow : R/W; bitpos: [26]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_i3c_mst_allow:1;
        /** reg_lp_mm_hp_i3c_slv_allow : R/W; bitpos: [27]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_i3c_slv_allow:1;
        /** reg_lp_mm_hp_lcdcam_allow : R/W; bitpos: [28]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_lcdcam_allow:1;
        uint32_t reserved_29:1;
        /** reg_lp_mm_hp_adc_allow : R/W; bitpos: [30]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_adc_allow:1;
        /** reg_lp_mm_hp_uhci_allow : R/W; bitpos: [31]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_uhci_allow:1;
    };
    uint32_t val;
} tee_lp_mm_pms_reg2_reg_t;


/** Group: TEE LP MM PMS REG3 REG */
/** Type of lp_mm_pms_reg3 register
 *  NA
 */
typedef union {
    struct {
        /** reg_lp_mm_hp_gpio_allow : R/W; bitpos: [0]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_gpio_allow:1;
        /** reg_lp_mm_hp_iomux_allow : R/W; bitpos: [1]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_iomux_allow:1;
        /** reg_lp_mm_hp_systimer_allow : R/W; bitpos: [2]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_systimer_allow:1;
        /** reg_lp_mm_hp_sys_reg_allow : R/W; bitpos: [3]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_sys_reg_allow:1;
        /** reg_lp_mm_hp_clkrst_allow : R/W; bitpos: [4]; default: 1;
         *  NA
         */
        uint32_t reg_lp_mm_hp_clkrst_allow:1;
        uint32_t reserved_5:27;
    };
    uint32_t val;
} tee_lp_mm_pms_reg3_reg_t;


typedef struct {
    volatile tee_lp2hp_pms_date_reg_t lp2hp_pms_date;
    volatile tee_pms_clk_en_reg_t pms_clk_en;
    volatile tee_lp_mm_pms_reg0_reg_t lp_mm_pms_reg0;
    uint32_t reserved_00c[9];
    volatile tee_lp_mm_pms_reg1_reg_t lp_mm_pms_reg1;
    uint32_t reserved_034[28];
    volatile tee_lp_mm_pms_reg2_reg_t lp_mm_pms_reg2;
    uint32_t reserved_0a8[29];
    volatile tee_lp_mm_pms_reg3_reg_t lp_mm_pms_reg3;
} tee_dev_t;

extern tee_dev_t LP2HP_PERI_PMS;

#ifndef __cplusplus
_Static_assert(sizeof(tee_dev_t) == 0x120, "Invalid size of tee_dev_t structure");
#endif

#ifdef __cplusplus
}
#endif
