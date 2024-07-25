/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     test_rsa.c
 * @brief    the test function for the rsa driver
 * @version  V1.0
 * @date     20. July 2016
 ******************************************************************************/
#include <stdio.h>
#include "dtest.h"
#include "soc.h"
#include "drv/rsa.h"
#include <string.h>
#include "test_driver_config.h"

static rsa_handle_t *rsa = NULL;

uint8_t modulus[] = {
    0xdf, 0x83, 0xe4, 0x76, 0x2d, 0x00, 0x61, 0xf6, 0xd0, 0x8d, 0x4a, 0x04, 0x66, 0xb1, 0xd5, 0x55,
    0xef, 0x71, 0xb5, 0xa5, 0x4e, 0x69, 0x44, 0xd3, 0x4f, 0xb8, 0x3d, 0xec, 0xb1, 0x1d, 0x5f, 0x82,
    0x6a, 0x48, 0x21, 0x00, 0x7f, 0xd7, 0xd5, 0xf6, 0x82, 0x35, 0xc2, 0xa6, 0x67, 0xa3, 0x53, 0x2d,
    0x3a, 0x83, 0x9a, 0xba, 0x60, 0xc2, 0x11, 0x22, 0xc2, 0x35, 0x83, 0xe9, 0x10, 0xa1, 0xb4, 0xa6,
    0x74, 0x57, 0x99, 0xd3, 0xa8, 0x6a, 0x21, 0x83, 0x76, 0xc1, 0x67, 0xde, 0xd8, 0xec, 0xdf, 0xf7,
    0xc0, 0x1b, 0xf6, 0xfa, 0x14, 0xa4, 0x0a, 0xec, 0xd1, 0xee, 0xc0, 0x76, 0x4c, 0xcd, 0x4a, 0x0a,
    0x5c, 0x96, 0xf2, 0xc9, 0xa4, 0x67, 0x03, 0x97, 0x2e, 0x17, 0xcd, 0xa9, 0x27, 0x9d, 0xa6, 0x35,
    0x5f, 0x7d, 0xb1, 0x6b, 0x68, 0x0e, 0x99, 0xc7, 0xdd, 0x5d, 0x6f, 0x15, 0xce, 0x8e, 0x85, 0x33
};
static const uint8_t publicExponent[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01
};
static const uint8_t privateExponent[] = {
    0xc6, 0x15, 0x3d, 0x02, 0xfe, 0x1e, 0xb8, 0xb2, 0xe3, 0x60, 0x53, 0x98, 0x52, 0xea, 0x87, 0x06,
    0x01, 0x8d, 0xe4, 0x4c, 0xfb, 0x90, 0x8f, 0x4e, 0x35, 0xf8, 0x31, 0xe8, 0xf1, 0x8d, 0xf6, 0x76,
    0xbd, 0x79, 0xee, 0xc5, 0x62, 0x87, 0x05, 0x37, 0xd1, 0x6d, 0x93, 0x73, 0xa5, 0xa5, 0x38, 0xb1,
    0x7c, 0x89, 0xe5, 0x36, 0x07, 0x49, 0xf5, 0xa5, 0xb8, 0x37, 0x75, 0x0f, 0xb7, 0x8d, 0x97, 0x69,
    0xc4, 0xd4, 0x8a, 0xb7, 0xfe, 0x74, 0x48, 0x45, 0x58, 0x47, 0x29, 0xa3, 0x0b, 0xa7, 0xdc, 0x55,
    0x98, 0x18, 0x8c, 0xd4, 0x52, 0xf5, 0xc9, 0xe8, 0x40, 0xce, 0x97, 0x46, 0x14, 0x1f, 0x62, 0x94,
    0xc3, 0x21, 0x1e, 0x5d, 0x49, 0x59, 0x31, 0xeb, 0xc4, 0x95, 0xf9, 0x33, 0x70, 0xa7, 0x90, 0xc3,
    0x9e, 0x98, 0x58, 0xa4, 0x00, 0xa4, 0x0f, 0xf3, 0x51, 0x80, 0xc6, 0x14, 0xfb, 0xd5, 0x5b, 0x01
};

uint8_t Digest_signature_pkcs1_padding_out[] = {
    0x07, 0x2d, 0x25, 0xde, 0xa5, 0xfd, 0x7c, 0xb0, 0x92, 0xb4, 0xee, 0x57, 0xe8, 0xd3, 0x79, 0x74,
    0x59, 0x25, 0x34, 0xef, 0xfd, 0x2b, 0xda, 0x8b, 0xa4, 0x40, 0x4e, 0xd8, 0x92, 0x6e, 0xee, 0x84,
    0x52, 0xb0, 0xe1, 0x0e, 0xa8, 0xa9, 0x68, 0x62, 0x1b, 0x51, 0xed, 0x50, 0x84, 0x98, 0x6a, 0x97,
    0x98, 0xe8, 0xcf, 0x3f, 0x85, 0xd3, 0x28, 0x26, 0xf3, 0x7a, 0x52, 0x4b, 0x04, 0x95, 0xe6, 0xfd,
    0xfa, 0x41, 0xf3, 0xac, 0x8a, 0x6d, 0x74, 0x91, 0x8c, 0x87, 0x52, 0x38, 0x08, 0x49, 0xf4, 0x60,
    0xcd, 0x4b, 0x1a, 0x9e, 0x52, 0x60, 0xf2, 0x73, 0x60, 0x31, 0x78, 0x37, 0xd9, 0x42, 0xc4, 0x61,
    0x43, 0xcf, 0x6d, 0x55, 0xee, 0x05, 0x19, 0xb7, 0xc3, 0x37, 0xa7, 0xa8, 0xa4, 0xbd, 0xf1, 0xac,
    0x8e, 0x39, 0x20, 0x59, 0xcd, 0xfc, 0x50, 0x16, 0x81, 0x2d, 0xeb, 0xba, 0x95, 0xe9, 0x38, 0xa5,
};

static const uint8_t Digest[] = {
    0xe4, 0x2c, 0x9f, 0x12, 0xf7, 0xd2, 0x67, 0x3a, 0x23, 0xea, 0x85, 0x61, 0xeb, 0xb2, 0xc2, 0x19,
    0xdc, 0xd9, 0xf1, 0xaa
};

uint8_t dst[1024 >> 3] = {0};
uint32_t dst_size = 0;

static void rsa_crypto(int32_t idx, rsa_event_e event)
{
    if (event == RSA_EVENT_ENCRYPT_COMPLETE) {
        printf("rsa encrypt is complete\r\n");
    } else if (event == RSA_EVENT_DECRYPT_COMPLETE) {
        printf("rsa decrypt is cmplete\r\n");
    } else if (event == RSA_EVENT_SIGN_COMPLETE) {
        printf("rsa sign is cmplete\r\n");
    } else if (event == RSA_EVENT_VERIFY_COMPLETE) {
        printf("rsa verify is cmplete\r\n");
    }
}

static rsa_padding_t rsa_padding[] = {
    {RSA_PADDING_MODE_PKCS1, RSA_HASH_TYPE_MD5},
    {RSA_PADDING_MODE_PKCS1, RSA_HASH_TYPE_SHA1},
    {RSA_PADDING_MODE_PKCS1, RSA_HASH_TYPE_SHA224},
    {RSA_PADDING_MODE_PKCS1, RSA_HASH_TYPE_SHA256},
    {RSA_PADDING_MODE_PKCS1, RSA_HASH_TYPE_SHA384},
    {RSA_PADDING_MODE_PKCS1, RSA_HASH_TYPE_SHA512},
    {RSA_PADDING_MODE_NO, RSA_HASH_TYPE_SHA1},
    {RSA_PADDING_MODE_SSLV23, RSA_HASH_TYPE_SHA1},
    {RSA_PADDING_MODE_PKCS1_OAEP, RSA_HASH_TYPE_SHA1},
    {RSA_PADDING_MODE_X931, RSA_HASH_TYPE_SHA1},
    {RSA_PADDING_MODE_PSS, RSA_HASH_TYPE_SHA1},
};

typedef struct {
    uint8_t *modulus;
    const uint8_t *publicExponent;
    const uint8_t *privateExponent;
    rsa_padding_t *padding;
    rsa_data_bits_e datawidth;
    rsa_endian_mode_e endian;
    uint32_t expect_out;
} rsa_test_t;

static rsa_test_t rsa_config_cases[] = {
    {NULL, NULL, NULL, NULL, RSA_DATA_BITS_2048 + 1, RSA_ENDIAN_MODE_LITTLE, (CSI_DRV_ERRNO_RSA_BASE | RSA_ERROR_DATA_BITS)},
    {NULL, NULL, NULL, NULL, RSA_DATA_BITS_1024, RSA_ENDIAN_MODE_BIG + 1, (CSI_DRV_ERRNO_RSA_BASE | RSA_ERROR_ENDIAN)},
};

static rsa_test_t rsa_sign_verify_cases[] = {
    {NULL, publicExponent, privateExponent, (rsa_padding_t *) &rsa_padding[1], 0, 0, (CSI_DRV_ERRNO_RSA_BASE | DRV_ERROR_PARAMETER)},
    {modulus, publicExponent, privateExponent, (rsa_padding_t *) &rsa_padding[2], 0, 0, (CSI_DRV_ERRNO_RSA_BASE | DRV_ERROR_PARAMETER)},
    {modulus, publicExponent, privateExponent, (rsa_padding_t *) &rsa_padding[4], 0, 0, (CSI_DRV_ERRNO_RSA_BASE | DRV_ERROR_PARAMETER)},
    {modulus, publicExponent, privateExponent, (rsa_padding_t *) &rsa_padding[5], 0, 0, (CSI_DRV_ERRNO_RSA_BASE | DRV_ERROR_PARAMETER)},
};

static rsa_test_t rsa_encrypt_decrypt_cases[] = {
    {NULL, publicExponent, privateExponent, (rsa_padding_t *) &rsa_padding[1], 0, 0, (CSI_DRV_ERRNO_RSA_BASE | DRV_ERROR_PARAMETER)},
    {modulus, publicExponent, privateExponent, (rsa_padding_t *) &rsa_padding[7], 0, 0, (CSI_DRV_ERRNO_RSA_BASE | DRV_ERROR_PARAMETER)},
    {modulus, publicExponent, privateExponent, (rsa_padding_t *) &rsa_padding[8], 0, 0, (CSI_DRV_ERRNO_RSA_BASE | DRV_ERROR_PARAMETER)},
    {modulus, publicExponent, privateExponent, (rsa_padding_t *) &rsa_padding[9], 0, 0, (CSI_DRV_ERRNO_RSA_BASE | DRV_ERROR_PARAMETER)},
    {modulus, publicExponent, privateExponent, (rsa_padding_t *) &rsa_padding[10], 0, 0, (CSI_DRV_ERRNO_RSA_BASE | DRV_ERROR_PARAMETER)},
};

static void test_csi_rsa_encrypt(void)
{
    int ret;
    uint8_t i;
    uint8_t k;
    int index;
    index = 1;
    ASSERT_TRUE(index >= 0);

    for (i = 0; i < index; i++) {
        rsa = csi_rsa_initialize(i, rsa_crypto);
        ASSERT_TRUE(rsa != NULL);

        for (k = 0; k < sizeof(rsa_encrypt_decrypt_cases) / (sizeof(rsa_test_t)); k++) {
            ret = csi_rsa_encrypt(rsa, (void *)rsa_encrypt_decrypt_cases[k].modulus, (void *)rsa_encrypt_decrypt_cases[k].publicExponent, (void *)Digest, 20, (void *)dst, &dst_size, *rsa_encrypt_decrypt_cases[k].padding);
            ASSERT_TRUE(ret == rsa_encrypt_decrypt_cases[k].expect_out);
        }
    }

}

static void test_csi_rsa_decrypt(void)
{
    int ret;
    uint8_t i;
    uint8_t k;
    int index;
    index = 1;
    ASSERT_TRUE(index >= 0);

    for (i = 0; i < index; i++) {
        rsa = csi_rsa_initialize(i, rsa_crypto);
        ASSERT_TRUE(rsa != NULL);

        for (k = 0; k < sizeof(rsa_encrypt_decrypt_cases) / (sizeof(rsa_test_t)); k++) {
            ret = csi_rsa_decrypt(rsa, (void *)rsa_encrypt_decrypt_cases[k].modulus, (void *)rsa_encrypt_decrypt_cases[k].privateExponent, (void *)dst, 20, (void *)dst, &dst_size, *rsa_encrypt_decrypt_cases[k].padding);
            ASSERT_TRUE(ret == rsa_encrypt_decrypt_cases[k].expect_out);
        }
    }

}


static void test_csi_rsa_verify(void)
{
    int ret;
    uint8_t i;
    uint8_t k;
    int index;
    int result;
    index = 1;
    ASSERT_TRUE(index >= 0);

    for (i = 0; i < index; i++) {
        rsa = csi_rsa_initialize(i, rsa_crypto);
        ASSERT_TRUE(rsa != NULL);

        for (k = 0; k < sizeof(rsa_sign_verify_cases) / (sizeof(rsa_test_t)); k++) {
            ret = csi_rsa_verify(rsa, (void *)rsa_sign_verify_cases[k].modulus, (void *)rsa_sign_verify_cases[k].publicExponent, (void *)Digest, 20, (void *)dst, dst_size, (void *)&result, *rsa_sign_verify_cases[k].padding);
            ASSERT_TRUE(ret == rsa_sign_verify_cases[k].expect_out);

            if (ret != rsa_sign_verify_cases[k].expect_out) {
                printf("the k is %d\n", k);
            }
        }
    }
}

static void test_csi_rsa_sign(void)
{
    int ret;
    uint8_t i;
    uint8_t k;
    int index;
    index = 1;
    ASSERT_TRUE(index >= 0);

    for (i = 0; i < index; i++) {
        rsa = csi_rsa_initialize(i, rsa_crypto);
        ASSERT_TRUE(rsa != NULL);

        for (k = 0; k < sizeof(rsa_sign_verify_cases) / (sizeof(rsa_test_t)); k++) {
            ret = csi_rsa_sign(rsa, (void *)rsa_sign_verify_cases[k].modulus, (void *)rsa_sign_verify_cases[k].privateExponent, (void *)Digest, 20, (void *)dst, &dst_size, *rsa_sign_verify_cases[k].padding);
            ASSERT_TRUE(ret == rsa_sign_verify_cases[k].expect_out);

            if (ret != rsa_sign_verify_cases[k].expect_out) {
                printf("the k is %d\n", k);
            }
        }
    }
}

static void test_csi_rsa_config(void)
{
    int ret;
    uint8_t i;
    uint8_t k;
    int index;
    index = 1;
    ASSERT_TRUE(index >= 0);

    for (i = 0; i < index; i++) {
        rsa = csi_rsa_initialize(i, rsa_crypto);
        ASSERT_TRUE(rsa != NULL);

        for (k = 0; k < sizeof(rsa_config_cases) / (sizeof(rsa_test_t)); k++) {
            ret = csi_rsa_config(rsa, rsa_config_cases[k].datawidth, rsa_config_cases[k].endian, NULL);
            ASSERT_TRUE(ret == rsa_config_cases[k].expect_out);
        }
    }
}

void test_rsa_encrypt_decrypt()
{
    int ret;
    uint8_t i;
    int index;
    rsa_padding_t padding;
    padding.padding_type = RSA_PADDING_MODE_PKCS1;
    padding.hash_type = RSA_HASH_TYPE_SHA1;

    index = 1;
    ASSERT_TRUE(index >= 0);

    for (i = 0; i < index; i++) {
        rsa = csi_rsa_initialize(i, rsa_crypto);
        ASSERT_TRUE(rsa != NULL);
        ret = csi_rsa_config(rsa, RSA_DATA_BITS_1024, RSA_ENDIAN_MODE_LITTLE, NULL);
        ASSERT_TRUE(ret == 0);

        ret = csi_rsa_encrypt(rsa, (void *)modulus, (void *)publicExponent, (void *)Digest, 20, (void *)dst, &dst_size, padding);
        ASSERT_TRUE(ret == 0);

        ret = csi_rsa_decrypt(rsa, (void *)modulus, (void *)privateExponent, (void *)dst, dst_size, (void *)dst, (uint32_t *)&dst_size, padding);
        ASSERT_TRUE(ret == 0);

        if (ret == 0) {
            printf("rsa encrypt and decrypt: %s\n", memcmp(dst, Digest, 20) ? "Fail" : "Pass");
        }

        ret = csi_rsa_uninitialize(rsa);
        ASSERT_TRUE(ret == 0);
    }
}

void test_rsa_sign_verify()
{
    int ret;
    uint8_t i;
    uint8_t result;
    int index;
    rsa_padding_t padding;
    padding.padding_type = RSA_PADDING_MODE_PKCS1;
    padding.hash_type = RSA_HASH_TYPE_SHA1;

    index = 1;
    ASSERT_TRUE(index >= 0);

    for (i = 0; i < index; i++) {
        rsa = csi_rsa_initialize(i, rsa_crypto);
        ASSERT_TRUE(rsa != NULL);
        ret = csi_rsa_config(rsa, RSA_DATA_BITS_1024, RSA_ENDIAN_MODE_LITTLE, NULL);
        ASSERT_TRUE(ret == 0);

        ret = csi_rsa_sign(rsa, (void *)modulus, (void *)privateExponent, (void *)Digest, 20, (void *)dst, &dst_size, padding);
        ASSERT_TRUE(ret == 0);

        printf("rsa sign: %s\n", memcmp(dst, Digest_signature_pkcs1_padding_out, dst_size) ? "Fail" : "Pass");

        ret = csi_rsa_verify(rsa, (void *)modulus, (void *)publicExponent, (void *)Digest, 20, (void *)dst, dst_size, (void *)&result, padding);
        ASSERT_TRUE(ret == 0);
        printf("rsa verify: %s\n", result ? "Pass" : "Fail");

        ret = csi_rsa_uninitialize(rsa);
        ASSERT_TRUE(ret == 0);
    }
}


/*****************************************************************************
test_rsa: main function of the rsa test

INPUT: NULL

RETURN: NULL

*****************************************************************************/
int test_rsa()
{
    dtest_suite_info_t test_suite_info = {
        "test_rsa", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
        { "test_rsa_encrypt_decrypt", test_rsa_encrypt_decrypt, RSA_ENCRYPT_DECRYPT_EN },
        { "test_rsa_sign_verify", test_rsa_sign_verify, RSA_SIGN_VERIFY_EN },
        { "test_csi_rsa_decrypt", test_csi_rsa_decrypt, RSA_DECRYPT_INTERFACE_EN },
        { "test_csi_rsa_encrypt", test_csi_rsa_encrypt, RSA_ENCRYPT_INTERFACE_EN },
        { "test_csi_rsa_verify", test_csi_rsa_verify, RSA_VERIFY_INTERFACE_EN },
        { "test_csi_rsa_sign", test_csi_rsa_sign, RSA_SIGN_INTERFACE_EN },
        { "test_csi_rsa_config", test_csi_rsa_config, RSA_CONFIG_INTERFACE_EN },
        { NULL, NULL, 0 }
    };

    dtest_add_cases(test_suite, test_case_info_array);

    return 0;
}

int main(void)
{
    dtest_register_cb(test_rsa);
    dtest();

    return 0;
}
