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

/** Group: Memory data */

/** Group: Clock gate register */
/** Type of clk register
 *  Key Manager clock gate control register
 */
typedef union {
    struct {
        /** reg_cg_force_on : R/W; bitpos: [0]; default: 1;
         *  Write 1 to force on register clock gate.
         */
        uint32_t reg_cg_force_on:1;
        /** mem_cg_force_on : R/W; bitpos: [1]; default: 0;
         *  Write 1 to force on memory clock gate.
         */
        uint32_t mem_cg_force_on:1;
        uint32_t reserved_2:30;
    };
    uint32_t val;
} keymng_clk_reg_t;


/** Group: Interrupt registers */
/** Type of int_raw register
 *  Key Manager interrupt raw register, valid in level.
 */
typedef union {
    struct {
        /** prep_done_int_raw : RO/WTC/SS; bitpos: [0]; default: 0;
         *  The raw interrupt status bit  for the km_prep_done_int interrupt
         */
        uint32_t prep_done_int_raw:1;
        /** proc_done_int_raw : RO/WTC/SS; bitpos: [1]; default: 0;
         *  The raw interrupt status bit  for the km_proc_done_int interrupt
         */
        uint32_t proc_done_int_raw:1;
        /** post_done_int_raw : RO/WTC/SS; bitpos: [2]; default: 0;
         *  The raw interrupt status bit  for the km_post_done_int interrupt
         */
        uint32_t post_done_int_raw:1;
        uint32_t reserved_3:29;
    };
    uint32_t val;
} keymng_int_raw_reg_t;

/** Type of int_st register
 *  Key Manager interrupt status register.
 */
typedef union {
    struct {
        /** prep_done_int_st : RO; bitpos: [0]; default: 0;
         *  The masked interrupt status bit  for the km_prep_done_int interrupt
         */
        uint32_t prep_done_int_st:1;
        /** proc_done_int_st : RO; bitpos: [1]; default: 0;
         *  The masked interrupt status bit  for the km_proc_done_int interrupt
         */
        uint32_t proc_done_int_st:1;
        /** post_done_int_st : RO; bitpos: [2]; default: 0;
         *  The masked interrupt status bit  for the km_post_done_int interrupt
         */
        uint32_t post_done_int_st:1;
        uint32_t reserved_3:29;
    };
    uint32_t val;
} keymng_int_st_reg_t;

/** Type of int_ena register
 *  Key Manager interrupt enable register.
 */
typedef union {
    struct {
        /** prep_done_int_ena : R/W; bitpos: [0]; default: 0;
         *  The interrupt enable bit  for the km_prep_done_int interrupt
         */
        uint32_t prep_done_int_ena:1;
        /** proc_done_int_ena : R/W; bitpos: [1]; default: 0;
         *  The interrupt enable bit  for the km_proc_done_int interrupt
         */
        uint32_t proc_done_int_ena:1;
        /** post_done_int_ena : R/W; bitpos: [2]; default: 0;
         *  The interrupt enable bit  for the km_post_done_int interrupt
         */
        uint32_t post_done_int_ena:1;
        uint32_t reserved_3:29;
    };
    uint32_t val;
} keymng_int_ena_reg_t;

/** Type of int_clr register
 *  Key Manager interrupt clear register.
 */
typedef union {
    struct {
        /** prep_done_int_clr : WT; bitpos: [0]; default: 0;
         *  Set this bit to clear the km_prep_done_int interrupt
         */
        uint32_t prep_done_int_clr:1;
        /** proc_done_int_clr : WT; bitpos: [1]; default: 0;
         *  Set this bit to clear the km_proc_done_int interrupt
         */
        uint32_t proc_done_int_clr:1;
        /** post_done_int_clr : WT; bitpos: [2]; default: 0;
         *  Set this bit to clear the km_post_done_int interrupt
         */
        uint32_t post_done_int_clr:1;
        uint32_t reserved_3:29;
    };
    uint32_t val;
} keymng_int_clr_reg_t;


/** Group: Static configuration registers */
/** Type of static register
 *  Key Manager static configuration register
 */
typedef union {
    struct {
        /** use_efuse_key : R/W; bitpos: [4:0]; default: 0;
         *  Set each bit to choose efuse key instead of key manager deployed key. Each bit
         *  stands for a key type:bit 4 for psram_key; bit 3 for ds_key; bit 2 for hmac_key;
         *  bit 1 for flash_key; bit 0 for ecdsa_key
         */
        uint32_t use_efuse_key:5;
        /** rnd_switch_cycle : R/W; bitpos: [9:5]; default: 15;
         *  The core clock cycle number to sample one rng input data. Please set it bigger than
         *  the clock cycle ratio: T_rng/T_km
         */
        uint32_t rnd_switch_cycle:5;
        /** use_sw_init_key : R/W; bitpos: [10]; default: 0;
         *  Set this bit to use software written init key instead of efuse_init_key.
         */
        uint32_t use_sw_init_key:1;
        /** flash_key_len : R/W; bitpos: [11]; default: 0;
         *  Set this bit to choose flash crypt using xts-aes-256 or xts-aes-128. 1: use
         *  xts-aes-256. 0: use xts-aes-128.
         */
        uint32_t flash_key_len:1;
        /** psram_key_len : R/W; bitpos: [12]; default: 0;
         *  Set this bit to choose psram crypt using xts-aes-256 or xts-aes-128. 1: use
         *  xts-aes-256. 0: use xts-aes-128.
         */
        uint32_t psram_key_len:1;
        uint32_t reserved_13:19;
    };
    uint32_t val;
} keymng_static_reg_t;

/** Type of lock register
 *  Key Manager static configuration locker register
 */
typedef union {
    struct {
        /** use_efuse_key_lock : R/W1; bitpos: [4:0]; default: 0;
         *  Write 1 to lock reg_use_efuse_key. Each bit locks the corresponding bit of
         *  reg_use_efuse_key.
         */
        uint32_t use_efuse_key_lock:5;
        /** rnd_switch_cycle_lock : R/W1; bitpos: [5]; default: 0;
         *  Write 1 to lock reg_rnd_switch_cycle.
         */
        uint32_t rnd_switch_cycle_lock:1;
        /** use_sw_init_key_lock : R/W1; bitpos: [6]; default: 0;
         *  Write 1 to lock reg_use_sw_init_key.
         */
        uint32_t use_sw_init_key_lock:1;
        /** flash_key_len_lock : R/W1; bitpos: [7]; default: 0;
         *  Write 1 to lock reg_flash_key_len.
         */
        uint32_t flash_key_len_lock:1;
        /** psram_key_len_lock : R/W1; bitpos: [8]; default: 0;
         *  Write 1 to lock reg_psram_key_len.
         */
        uint32_t psram_key_len_lock:1;
        uint32_t reserved_9:23;
    };
    uint32_t val;
} keymng_lock_reg_t;


/** Group: Configuration registers */
/** Type of conf register
 *  Key Manager configuration register
 */
typedef union {
    struct {
        /** kgen_mode : R/W; bitpos: [2:0]; default: 0;
         *  Set this field to choose the key generator deployment mode. 0: random mode. 1: AES
         *  mode. 2: ECDH0 mode. 3: ECDH1 mode. 4: recover mode. 5: export mode. 6-7: reserved.
         */
        uint32_t kgen_mode:3;
        /** key_purpose : R/W; bitpos: [6:3]; default: 0;
         *  Set this field to choose the key purpose. 1: ecdsa_key_192. 2: ecdsa_key_256. 3:
         *  flash_256_1_key. 4: flash_256_2_key. 5: flash_128_key. 6: hmac_key. 7: ds_key. 8:
         *  psram_256_1_key. 9: psram_256_2_key. 10: psram_128_key. 11: ecdsa_key_384_l. 12:
         *  ecdsa_key_384_h. Others: reserved.
         */
        uint32_t key_purpose:4;
        uint32_t reserved_7:25;
    };
    uint32_t val;
} keymng_conf_reg_t;


/** Group: Control registers */
/** Type of start register
 *  Key Manager control register
 */
typedef union {
    struct {
        /** start : WT; bitpos: [0]; default: 0;
         *  Write 1 to continue Key Manager operation at LOAD/GAIN state.
         */
        uint32_t start:1;
        /** continue : WT; bitpos: [1]; default: 0;
         *  Write 1 to start Key Manager at IDLE state.
         */
        uint32_t conti:1;
        uint32_t reserved_2:30;
    };
    uint32_t val;
} keymng_start_reg_t;


/** Group: State registers */
/** Type of state register
 *  Key Manager state register
 */
typedef union {
    struct {
        /** state : RO; bitpos: [1:0]; default: 0;
         *  The state of Key Manager. 0: IDLE. 1: LOAD. 2: GAIN. 3: BUSY.
         */
        uint32_t state:2;
        uint32_t reserved_2:30;
    };
    uint32_t val;
} keymng_state_reg_t;


/** Group: Result registers */
/** Type of result register
 *  Key Manager operation result register
 */
typedef union {
    struct {
        /** proc_result : RO/SS; bitpos: [0]; default: 0;
         *  The procedure result bit of Key Manager, only valid when Key Manager procedure is
         *  done. 1: Key Manager procedure succeeded. 0: Key Manager procedure failed.
         */
        uint32_t proc_result:1;
        uint32_t reserved_1:31;
    };
    uint32_t val;
} keymng_result_reg_t;

/** Type of key_vld register
 *  Key Manager key status register
 */
typedef union {
    struct {
        /** key_ecdsa_192_vld : RO; bitpos: [0]; default: 0;
         *  The status bit for key_ecdsa_192.   1: The key has been deployed correctly. 0: The
         *  key has not been deployed yet.
         */
        uint32_t key_ecdsa_192_vld:1;
        /** key_ecdsa_256_vld : RO; bitpos: [1]; default: 0;
         *  The status bit for key_ecdsa_256.   1: The key has been deployed correctly. 0: The
         *  key has not been deployed yet.
         */
        uint32_t key_ecdsa_256_vld:1;
        /** key_flash_vld : RO; bitpos: [2]; default: 0;
         *  The status bit for key_flash.     1: The key has been deployed correctly. 0: The
         *  key has not been deployed yet.
         */
        uint32_t key_flash_vld:1;
        /** key_hmac_vld : RO; bitpos: [3]; default: 0;
         *  The status bit for key_hmac.    1: The key has been deployed correctly. 0: The key
         *  has not been deployed yet.
         */
        uint32_t key_hmac_vld:1;
        /** key_ds_vld : RO; bitpos: [4]; default: 0;
         *  The status bit for key_ds.         1: The key has been deployed correctly. 0: The
         *  key has not been deployed yet.
         */
        uint32_t key_ds_vld:1;
        /** key_psram_vld : RO; bitpos: [5]; default: 0;
         *  The status bit for key_psram.   1: The key has been deployed correctly. 0: The key
         *  has not been deployed yet.
         */
        uint32_t key_psram_vld:1;
        /** key_ecdsa_384_vld : RO; bitpos: [6]; default: 0;
         *  The status bit for key_ecdsa_384.   1: The key has been deployed correctly. 0: The
         *  key has not been deployed yet.
         */
        uint32_t key_ecdsa_384_vld:1;
        uint32_t reserved_7:25;
    };
    uint32_t val;
} keymng_key_vld_reg_t;

/** Type of huk_vld register
 *  Key Manager HUK status register
 */
typedef union {
    struct {
        /** huk_valid : RO; bitpos: [0]; default: 0;
         *  The HUK status. 0: HUK is not valid. 1: HUK is valid.
         */
        uint32_t huk_valid:1;
        uint32_t reserved_1:31;
    };
    uint32_t val;
} keymng_huk_vld_reg_t;


/** Group: Version register */
/** Type of date register
 *  Version control register
 */
typedef union {
    struct {
        /** date : R/W; bitpos: [27:0]; default: 37781824;
         *  Key Manager version control register.
         */
        uint32_t date:28;
        uint32_t reserved_28:4;
    };
    uint32_t val;
} keymng_date_reg_t;


typedef struct {
    uint32_t reserved_000;
    volatile keymng_clk_reg_t clk;
    volatile keymng_int_raw_reg_t int_raw;
    volatile keymng_int_st_reg_t int_st;
    volatile keymng_int_ena_reg_t int_ena;
    volatile keymng_int_clr_reg_t int_clr;
    volatile keymng_static_reg_t static_conf;
    volatile keymng_lock_reg_t lock;
    volatile keymng_conf_reg_t conf;
    volatile keymng_start_reg_t start;
    volatile keymng_state_reg_t state;
    volatile keymng_result_reg_t result;
    volatile keymng_key_vld_reg_t key_vld;
    volatile keymng_huk_vld_reg_t huk_vld;
    uint32_t reserved_038[49];
    volatile keymng_date_reg_t date;
    volatile uint32_t assist_info[16];
    volatile uint32_t public_info[16];
    volatile uint32_t sw_init_key[8];
} keymng_dev_t;

extern keymng_dev_t KEYMNG;

#ifndef __cplusplus
_Static_assert(sizeof(keymng_dev_t) == 0x1a0, "Invalid size of keymng_dev_t structure");
#endif

#ifdef __cplusplus
}
#endif
