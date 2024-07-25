/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     test_aes.c
 * @brief    the test function for the aes driver
 * @version  V1.0
 * @date     20. July 2016
 ******************************************************************************/
#include <stdio.h>
#include "soc.h"
#include "drv/aes.h"
#include "dtest.h"
#include "test_driver_config.h"

#define CONFIG_AES_BLOCK_MODE   1
struct aes_test {
    aes_crypto_mode_e crypto_mode;
    aes_endian_mode_e endian;
    uint8_t *in;
    uint8_t *key;
    uint8_t *iv;
    uint8_t *expect_out;
    aes_mode_e  aes_mode;
    aes_key_len_bits_e bits;
};

typedef struct aes_test aes_test_t;

aes_handle_t aes = NULL;
static uint8_t aes_int_flag = 1;

static void aes_interrupt_handler(int32_t idx, aes_event_e event)
{
    aes_int_flag = 0;
}

static uint8_t in[16] = "Hello, World!";
static uint8_t key[32] = "Demo-Key";
static uint8_t iv[16] = {0};
static uint8_t expect_out[][16] = {
    {0x3, 0x64, 0xf, 0x7a, 0x1, 0xef, 0xd6, 0xa7, 0x73, 0x48, 0x35, 0x5b, 0xbf, 0x5f, 0xa9, 0xfe },
    {0xdb, 0xc0, 0x1d, 0xb9, 0x2d, 0xdc, 0x2c, 0xf2, 0x4b, 0x3a, 0x79, 0x27, 0x89, 0x7f, 0xd5, 0x71 },
    {0x7b, 0xab, 0x3a, 0xb2, 0xf3, 0x3c, 0xcf, 0xca, 0x20, 0x7a, 0xc9, 0x63, 0x7f, 0x02, 0x49, 0x55 },
    {0x3, 0x64, 0xf, 0x7a, 0x1, 0xef, 0xd6, 0xa7, 0x73, 0x48, 0x35, 0x5b, 0xbf, 0x5f, 0xa9, 0xfe },
    {0xdb, 0xc0, 0x1d, 0xb9, 0x2d, 0xdc, 0x2c, 0xf2, 0x4b, 0x3a, 0x79, 0x27, 0x89, 0x7f, 0xd5, 0x71 },
    {0x7b, 0xab, 0x3a, 0xb2, 0xf3, 0x3c, 0xcf, 0xca, 0x20, 0x7a, 0xc9, 0x63, 0x7f, 0x02, 0x49, 0x55 },
    {0x5f, 0xa1, 0x05, 0xb1, 0x41, 0xb0, 0x8c, 0x7c, 0x7b, 0x7f, 0x39, 0x5a, 0x7c, 0x46, 0x33, 0x3d },
    {0x92, 0xb6, 0x1c, 0xbc, 0x7b, 0xa6, 0xd5, 0xa9, 0x8e, 0x8b, 0xe4, 0x8d, 0xf0, 0xd8, 0xb4, 0xee },
    {0x11, 0xcd, 0x23, 0xaa, 0x7c, 0x60, 0x78, 0x7b, 0x87, 0x98, 0x40, 0xb4, 0x96, 0xd8, 0x0d, 0x32 }
};

static aes_test_t aes_cbc_256_cases[] = {
#if AES_LITTLE_ENDIAN
    {AES_CRYPTO_MODE_ENCRYPT, AES_ENDIAN_LITTLE, in, key, iv, expect_out[2]},
    {AES_CRYPTO_MODE_DECRYPT, AES_ENDIAN_LITTLE, expect_out[2], key, iv, in},
#endif
#if AES_BIG_ENDIAN
    {AES_CRYPTO_MODE_ENCRYPT, AES_ENDIAN_BIG, in, key, iv, expect_out[8]},
    {AES_CRYPTO_MODE_DECRYPT, AES_ENDIAN_BIG, expect_out[8], key, iv, in},
#endif
};

static aes_test_t aes_cbc_192_cases[] = {
#if AES_LITTLE_ENDIAN
    {AES_CRYPTO_MODE_ENCRYPT, AES_ENDIAN_LITTLE, in, key, iv, expect_out[1]},
    {AES_CRYPTO_MODE_DECRYPT, AES_ENDIAN_LITTLE, expect_out[1], key, iv, in},
#endif
#if AES_BIG_ENDIAN
    {AES_CRYPTO_MODE_ENCRYPT, AES_ENDIAN_BIG, in, key, iv, expect_out[7]},
    {AES_CRYPTO_MODE_DECRYPT, AES_ENDIAN_BIG, expect_out[7], key, iv, in},
#endif
};


static aes_test_t aes_cbc_128_cases[] = {
#if AES_LITTLE_ENDIAN
    {AES_CRYPTO_MODE_ENCRYPT, AES_ENDIAN_LITTLE, in, key, iv, expect_out[0]},
    {AES_CRYPTO_MODE_DECRYPT, AES_ENDIAN_LITTLE, expect_out[0], key, iv, in},
#endif
#if AES_BIG_ENDIAN
    {AES_CRYPTO_MODE_ENCRYPT, AES_ENDIAN_BIG, in, key, iv, expect_out[6]},
    {AES_CRYPTO_MODE_DECRYPT, AES_ENDIAN_BIG, expect_out[6], key, iv, in},
#endif
};


static aes_test_t aes_ecb_256_cases[] = {
#if AES_LITTLE_ENDIAN
    {AES_CRYPTO_MODE_ENCRYPT, AES_ENDIAN_LITTLE, in, key, iv, expect_out[5]},
    {AES_CRYPTO_MODE_DECRYPT, AES_ENDIAN_LITTLE, expect_out[5], key, iv, in},
#endif
#if AES_BIG_ENDIAN
    {AES_CRYPTO_MODE_ENCRYPT, AES_ENDIAN_BIG, in, key, iv, expect_out[8]},
    {AES_CRYPTO_MODE_DECRYPT, AES_ENDIAN_BIG, expect_out[8], key, iv, in},
#endif
};

static aes_test_t aes_ecb_192_cases[] = {
#if AES_LITTLE_ENDIAN
    {AES_CRYPTO_MODE_ENCRYPT, AES_ENDIAN_LITTLE, in, key, iv, expect_out[4]},
    {AES_CRYPTO_MODE_DECRYPT, AES_ENDIAN_LITTLE, expect_out[4], key, iv, in},
#endif
#if AES_BIG_ENDIAN
    {AES_CRYPTO_MODE_ENCRYPT, AES_ENDIAN_BIG, in, key, iv, expect_out[7]},
    {AES_CRYPTO_MODE_DECRYPT, AES_ENDIAN_BIG, expect_out[7], key, iv, in},
#endif
};

static aes_test_t aes_ecb_128_cases[] = {
#if AES_LITTLE_ENDIAN
    {AES_CRYPTO_MODE_ENCRYPT, AES_ENDIAN_LITTLE, in, key, iv, expect_out[3]},
    {AES_CRYPTO_MODE_DECRYPT, AES_ENDIAN_LITTLE, expect_out[3], key, iv, in},
#endif
#if AES_BIG_ENDIAN
    {AES_CRYPTO_MODE_ENCRYPT, AES_ENDIAN_BIG, in, key, iv, expect_out[6]},
    {AES_CRYPTO_MODE_DECRYPT, AES_ENDIAN_BIG, expect_out[6], key, iv, in},
#endif
};

typedef struct {
    aes_endian_mode_e endian;
    aes_mode_e  aes_mode;
    aes_key_len_bits_e bits;
    uint32_t expect_out;
} aes_config_test_t;
static aes_config_test_t csi_aes_config_cases[] = {
    {AES_ENDIAN_BIG + 1, AES_MODE_ECB, AES_KEY_LEN_BITS_128, (CSI_DRV_ERRNO_AES_BASE | AES_ERROR_ENDIAN)},
    {AES_ENDIAN_LITTLE, AES_MODE_CTR + 1, AES_KEY_LEN_BITS_128, (CSI_DRV_ERRNO_AES_BASE | AES_ERROR_MODE)},
    {AES_ENDIAN_LITTLE, AES_MODE_ECB, AES_KEY_LEN_BITS_256 + 1, (CSI_DRV_ERRNO_AES_BASE | AES_ERROR_DATA_BITS)},
};


typedef struct {
    aes_key_len_bits_e bits;
    aes_crypto_mode_e crypto_mode;
    uint8_t *key;
    uint32_t  expect_out;
} aes_set_key_test_t;
static aes_set_key_test_t csi_aes_set_key_cases[] = {
    {AES_KEY_LEN_BITS_256 + 1, AES_CRYPTO_MODE_ENCRYPT, key, (CSI_DRV_ERRNO_AES_BASE | DRV_ERROR_PARAMETER)},
    {AES_KEY_LEN_BITS_128, AES_CRYPTO_MODE_DECRYPT + 1, key, (CSI_DRV_ERRNO_AES_BASE | DRV_ERROR_PARAMETER)},
    {AES_KEY_LEN_BITS_128, AES_CRYPTO_MODE_ENCRYPT, NULL, (CSI_DRV_ERRNO_AES_BASE | DRV_ERROR_PARAMETER)},
};

void test_aes_ecb_256()
{
    uint8_t out[16];
    uint32_t index;
    uint32_t ret;
    char string[][10] = {"little", "big"};
    int i;
    int j;
    int k;
    int idx;
    int count = 16;

    printf("- - -Testing AES ECB Mode- - -\n");

    index = 1;

    for (j = 0; j < index; j++) {
        aes = csi_aes_initialize(j, aes_interrupt_handler);
        ASSERT_TRUE(aes != NULL);

        for (k = 0; k < sizeof(aes_ecb_256_cases) / (sizeof(aes_test_t)); k++) {
            ret = csi_aes_config(aes, AES_MODE_ECB, AES_KEY_LEN_BITS_256, aes_ecb_256_cases[k].endian);
            ASSERT_TRUE(ret == 0);

            ret = csi_aes_set_key(aes, NULL, (void *)aes_ecb_256_cases[k].key, AES_KEY_LEN_BITS_256, aes_ecb_256_cases[k].crypto_mode);
            ASSERT_TRUE(ret == 0);
            ret = csi_aes_ecb_crypto(aes, NULL, (void *)aes_ecb_256_cases[k].in, (void *)out, count);
            ASSERT_TRUE(ret == 0);

            if (aes_ecb_256_cases[k].endian == AES_ENDIAN_LITTLE) {
                idx = 0;
            } else {
                idx = 1;
            }

#if CONFIG_AES_BLOCK_MODE
            /*it is also valid by get status*/
            aes_status_t status;

            while (1) {
                status = csi_aes_get_status(aes);

                if (status.busy == 0) {
                    break;
                }
            }

#else
            while (aes_int_flag);

            aes_int_flag = 1;

#endif
            for (i = 0; i < 16; i++) {
                if (out[i] != aes_ecb_256_cases[k].expect_out[i]) {
                    break;
                }
            }

            ASSERT_TRUE(i == 16);

            if (i != 16) {
                printf("case%d aes%d 256 ecb endian %s encrypt failed\n", k, j, string[idx]);
            } else {
                printf("case%d aes%d 256 ecb endian %s encrypt passed\n", k, j, string[idx]);
            }
        }

        ret = csi_aes_uninitialize(aes);
        ASSERT_TRUE(ret == 0);
    }
}

void test_aes_ecb_192()
{
    uint8_t out[16];
    uint32_t index;
    uint32_t ret;
    char string[][10] = {"little", "big"};
    int i;
    int j;
    int k;
    int idx;
    int count = 16;

    printf("- - -Testing AES ECB Mode- - -\n");

    index = 1;

    for (j = 0; j < index; j++) {
        aes = csi_aes_initialize(j, aes_interrupt_handler);
        ASSERT_TRUE(aes != NULL);

        for (k = 0; k < sizeof(aes_ecb_192_cases) / (sizeof(aes_test_t)); k++) {
            ret = csi_aes_config(aes, AES_MODE_ECB, AES_KEY_LEN_BITS_192, aes_ecb_192_cases[k].endian);
            ASSERT_TRUE(ret == 0);

            ret = csi_aes_set_key(aes, NULL, (void *)aes_ecb_192_cases[k].key, AES_KEY_LEN_BITS_192, aes_ecb_192_cases[k].crypto_mode);
            ASSERT_TRUE(ret == 0);
            ret = csi_aes_ecb_crypto(aes, NULL, (void *)aes_ecb_192_cases[k].in, (void *)out, count);
            ASSERT_TRUE(ret == 0);

            if (aes_ecb_192_cases[k].endian == AES_ENDIAN_LITTLE) {
                idx = 0;
            } else {
                idx = 1;
            }

#if CONFIG_AES_BLOCK_MODE
            /*it is also valid by get status*/
            aes_status_t status;

            while (1) {
                status = csi_aes_get_status(aes);

                if (status.busy == 0) {
                    break;
                }
            }

#else
            while (aes_int_flag);

            aes_int_flag = 1;
#endif
            for (i = 0; i < 16; i++) {
                if (out[i] != aes_ecb_192_cases[k].expect_out[i]) {
                    break;
                }
            }

            ASSERT_TRUE(i == 16);

            if (i != 16) {
                printf("case%d aes%d 192 ecb endian %s encrypt failed\n", k, j, string[idx]);
            } else {
                printf("case%d aes%d 192 ecb endian %s encrypt passed\n", k, j, string[idx]);
            }
        }

        ret = csi_aes_uninitialize(aes);
        ASSERT_TRUE(ret == 0);
    }
}

void test_aes_cbc_256()
{
    uint8_t out[16];
    char string[][10] = {"little", "big"};
    uint32_t index;
    uint32_t ret;
    int i;
    int j;
    int k;
    int idx;
    int count = 16;

    printf("- - -Testing AES CBC Mode- - -\n");

    index = 1;

    for (j = 0; j < index; j++) {
        aes = csi_aes_initialize(j, aes_interrupt_handler);
        ASSERT_TRUE(aes != NULL);

        for (k = 0; k < sizeof(aes_cbc_256_cases) / (sizeof(aes_test_t)); k++) {
            memset(aes_cbc_256_cases[k].iv, 0x0, 16);
            ret = csi_aes_config(aes, AES_MODE_CBC, AES_KEY_LEN_BITS_256, aes_cbc_256_cases[k].endian);
            ASSERT_TRUE(ret == 0);

            ret = csi_aes_set_key(aes, NULL, (void *)aes_cbc_256_cases[k].key, AES_KEY_LEN_BITS_256, aes_cbc_256_cases[k].crypto_mode);
            ASSERT_TRUE(ret == 0);
            ret = csi_aes_cbc_crypto(aes, NULL, (void *)aes_cbc_256_cases[k].in, (void *)out, count, aes_cbc_256_cases[k].iv);
            ASSERT_TRUE(ret == 0);

            if (aes_cbc_256_cases[k].endian == AES_ENDIAN_LITTLE) {
                idx = 0;
            } else {
                idx = 1;
            }

#if CONFIG_AES_BLOCK_MODE
            /*it is also valid by get status*/
            aes_status_t status;

            while (1) {
                status = csi_aes_get_status(aes);

                if (status.busy == 0) {
                    break;
                }
            }

#else
            while (aes_int_flag);

            aes_int_flag = 1;
#endif
            for (i = 0; i < 16; i++) {
                if (out[i] != aes_cbc_256_cases[k].expect_out[i]) {
                    break;
                }
            }

            ASSERT_TRUE(i == 16);

            if (i != 16) {
                printf("case%d aes%d 256 cbc endian %s encrypt failed\n", k, j, string[idx]);
            } else {
                printf("case%d aes%d 256 cbc endian %s encrypt passed\n", k, j, string[idx]);
            }
        }

        ret = csi_aes_uninitialize(aes);
        ASSERT_TRUE(ret == 0);
    }

}

void test_aes_cbc_192()
{
    uint8_t out[16];
    uint32_t index;
    uint32_t ret;
    int i;
    int j;
    int k;
    int idx;
    int count = 16;
    char string[][10] = {"little", "big"};

    printf("- - -Testing AES CBC Mode- - -\n");

    index = 1;

    for (j = 0; j < index; j++) {
        aes = csi_aes_initialize(j, aes_interrupt_handler);
        ASSERT_TRUE(aes != NULL);

        for (k = 0; k < sizeof(aes_cbc_192_cases) / (sizeof(aes_test_t)); k++) {
            memset(aes_cbc_256_cases[k].iv, 0x0, 16);
            ret = csi_aes_config(aes, AES_MODE_CBC, AES_KEY_LEN_BITS_192, aes_cbc_192_cases[k].endian);
            ASSERT_TRUE(ret == 0);

            ret = csi_aes_set_key(aes, NULL, (void *)aes_cbc_192_cases[k].key, AES_KEY_LEN_BITS_192, aes_cbc_192_cases[k].crypto_mode);
            ASSERT_TRUE(ret == 0);
            ret = csi_aes_cbc_crypto(aes, NULL, (void *)aes_cbc_192_cases[k].in, (void *)out, count, aes_cbc_192_cases[k].iv);
            ASSERT_TRUE(ret == 0);

            if (aes_cbc_192_cases[k].endian == AES_ENDIAN_LITTLE) {
                idx = 0;
            } else {
                idx = 1;
            }

#if CONFIG_AES_BLOCK_MODE
            /*it is also valid by get status*/
            aes_status_t status;

            while (1) {
                status = csi_aes_get_status(aes);

                if (status.busy == 0) {
                    break;
                }
            }

#else
            while (aes_int_flag);

            aes_int_flag = 1;
#endif
            for (i = 0; i < 16; i++) {
                if (out[i] != aes_cbc_192_cases[k].expect_out[i]) {
                    break;
                }
            }

            ASSERT_TRUE(i == 16);

            if (i != 16) {
                printf("case%d aes%d 192 cbc endian %s encrypt failed\n", k, j, string[idx]);
            } else {
                printf("case%d aes%d 192 cbc endian %s encrypt passed\n", k, j, string[idx]);
            }
        }

        ret = csi_aes_uninitialize(aes);
        ASSERT_TRUE(ret == 0);
    }

}

void test_aes_cbc_128()
{
    uint8_t out[16];
    uint32_t index;
    uint32_t ret;
    int i;
    int j;
    int k;
    int idx;
    int count = 16;
    char string[][10] = {"little", "big"};

    printf("- - -Testing AES CBC Mode- - -\n");

    index = 1;

    for (j = 0; j < index; j++) {
        aes = csi_aes_initialize(j, aes_interrupt_handler);
        ASSERT_TRUE(aes != NULL);

        for (k = 0; k < sizeof(aes_cbc_128_cases) / (sizeof(aes_test_t)); k++) {
            memset(aes_cbc_256_cases[k].iv, 0x0, 16);
            ret = csi_aes_config(aes, AES_MODE_CBC, AES_KEY_LEN_BITS_128, aes_cbc_128_cases[k].endian);
            ASSERT_TRUE(ret == 0);

            ret = csi_aes_set_key(aes, NULL, (void *)aes_cbc_128_cases[k].key, AES_KEY_LEN_BITS_128, aes_cbc_128_cases[k].crypto_mode);
            ASSERT_TRUE(ret == 0);
            ret = csi_aes_cbc_crypto(aes, NULL, (void *)aes_cbc_128_cases[k].in, (void *)out, count, aes_cbc_128_cases[k].iv);
            ASSERT_TRUE(ret == 0);

            if (aes_cbc_128_cases[k].endian == AES_ENDIAN_LITTLE) {
                idx = 0;
            } else {
                idx = 1;
            }

#if CONFIG_AES_BLOCK_MODE
            /*it is also valid by get status*/
            aes_status_t status;

            while (1) {
                status = csi_aes_get_status(aes);

                if (status.busy == 0) {
                    break;
                }
            }

#else
            while (aes_int_flag);

            aes_int_flag = 1;
#endif
            for (i = 0; i < 16; i++) {
                if (out[i] != aes_cbc_128_cases[k].expect_out[i]) {
                    break;
                }
            }

            ASSERT_TRUE(i == 16);

            if (i != 16) {
                printf("case%d aes%d 128 cbc endian %s encrypt failed\n", k, j, string[idx]);
            } else {
                printf("case%d aes%d 128 cbc endian %s encrypt passed\n", k, j, string[idx]);
            }
        }

        ret = csi_aes_uninitialize(aes);
        ASSERT_TRUE(ret == 0);
    }
}

void test_aes_ecb_128()
{
    uint8_t out[16];
    uint32_t index;
    uint32_t ret;
    int i;
    int j;
    int k;
    int idx;
    int count = 16;
    char string[][10] = {"little", "big"};

    printf("- - -Testing AES ECB Mode- - -\n");

    index = 1;

    for (j = 0; j < index; j++) {
        aes = csi_aes_initialize(j, aes_interrupt_handler);
        ASSERT_TRUE(aes != NULL);

        for (k = 0; k < sizeof(aes_ecb_128_cases) / (sizeof(aes_test_t)); k++) {
            ret = csi_aes_config(aes, AES_MODE_ECB, AES_KEY_LEN_BITS_128, aes_ecb_128_cases[k].endian);
            ASSERT_TRUE(ret == 0);

            ret = csi_aes_set_key(aes, NULL, (void *)aes_ecb_128_cases[k].key, AES_KEY_LEN_BITS_128, aes_ecb_128_cases[k].crypto_mode);
            ASSERT_TRUE(ret == 0);
            ret = csi_aes_ecb_crypto(aes, NULL, (void *)aes_ecb_128_cases[k].in, (void *)out, count);
            ASSERT_TRUE(ret == 0);

            if (aes_ecb_128_cases[k].endian == AES_ENDIAN_LITTLE) {
                idx = 0;
            } else {
                idx = 1;
            }

#if CONFIG_AES_BLOCK_MODE
            /*it is also valid by get status*/
            aes_status_t status;

            while (1) {
                status = csi_aes_get_status(aes);

                if (status.busy == 0) {
                    break;
                }
            }

#else
            while (aes_int_flag);

            aes_int_flag = 1;
#endif
            for (i = 0; i < 16; i++) {
                if (out[i] != aes_ecb_128_cases[k].expect_out[i]) {
                    break;
                }
            }

            ASSERT_TRUE(i == 16);

            if (i != 16) {
                printf("case%d aes%d 128 ecb endian %s encrypt failed\n", k, j, string[idx]);
            } else {
                printf("case%d aes%d 128 ecb endian %s encrypt passed\n", k, j, string[idx]);
            }
        }

        ret = csi_aes_uninitialize(aes);
        ASSERT_TRUE(ret == 0);
    }

}

static void test_csi_aes_config(void)
{
    uint32_t index;
    uint32_t j;
    uint32_t i;
    uint32_t ret;
    index = 1;

    for (j = 0; j < index; j++) {
        aes = csi_aes_initialize(j, aes_interrupt_handler);
        ASSERT_TRUE(aes != NULL);

        for (i = 0; i < sizeof(csi_aes_config_cases) / (sizeof(aes_test_t)); i++) {
            ret = csi_aes_config(aes, csi_aes_config_cases[i].aes_mode, csi_aes_config_cases[i].bits, csi_aes_config_cases[i].endian);
            ASSERT_TRUE(ret == csi_aes_config_cases[i].expect_out);
        }
    }
}

static void test_csi_aes_set_key(void)
{
    uint32_t index;
    uint32_t j;
    uint32_t i;
    uint32_t ret;
    index = 1;

    for (j = 0; j < index; j++) {
        aes = csi_aes_initialize(j, aes_interrupt_handler);
        ASSERT_TRUE(aes != NULL);

        for (i = 0; i < sizeof(csi_aes_set_key_cases) / (sizeof(aes_test_t)); i++) {
            ret = csi_aes_set_key(aes, NULL, csi_aes_set_key_cases[i].key, csi_aes_set_key_cases[i].bits, csi_aes_set_key_cases[i].crypto_mode);
            ASSERT_TRUE(ret == csi_aes_set_key_cases[i].expect_out);
        }
    }
}

/*****************************************************************************
test_aes: main function of the aes test

INPUT: NULL

RETURN: NULL

*****************************************************************************/
int test_aes(void)
{
    dtest_suite_info_t test_suite_info = {
        "test_aes", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
        { "test_aes_cbc_128", test_aes_cbc_128, AES_CBC_128_EN },
        { "test_aes_cbc_192", test_aes_cbc_192, AES_CBC_192_EN },
        { "test_aes_cbc_256", test_aes_cbc_256, AES_CBC_256_EN },
        { "test_aes_ecb_128", test_aes_ecb_128, AES_ECB_128_EN },
        { "test_aes_ecb_192", test_aes_ecb_192, AES_ECB_192_EN },
        { "test_aes_ecb_256", test_aes_ecb_256, AES_ECB_256_EN },
        { "test_csi_aes_set_key", test_csi_aes_set_key, AES_SET_KEY_EN },
        { "test_csi_aes_config", test_csi_aes_config, AES_CONFIG_EN },
        { NULL, NULL, 0 }
    };

    dtest_add_cases(test_suite, test_case_info_array);

    return 0;
}

int main(void)
{
    dtest_register_cb(test_aes);
    dtest();

    return 0;
}
