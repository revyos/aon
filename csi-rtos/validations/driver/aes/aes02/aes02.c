/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     aes02.c
 * @brief    aes driver test
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "aes_test.h"
#include <stdio.h>
#include <string.h>
#include "dtest.h"

static uint8_t aes_int_flag;

static void aes_interrupt_handler(int32_t idx, aes_event_e event)
{
    if (event == AES_EVENT_CRYPTO_COMPLETE) {
        aes_int_flag = 0;
        //  printf("This is AES%d\n", idx);
    }
}

void aes_if_test()
{
    // get_capabilities
    aes_capabilities_t cap = csi_aes_get_capabilities(2);
    ASSERT_TRUE(cap.ecb_mode == 0);

    //initialize
    aes_handle_t aes = csi_aes_initialize(2, aes_interrupt_handler);
    ASSERT_TRUE(aes == NULL);

    aes = csi_aes_initialize(0, NULL);
    ASSERT_TRUE(aes != NULL);

    int32_t ret;
    //uninitialize
    ret = csi_aes_uninitialize(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_AES_BASE | DRV_ERROR_PARAMETER));

    //config
    ret = csi_aes_config(NULL, AES_MODE_ECB, AES_KEY_LEN_BITS_128, AES_ENDIAN_LITTLE);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_AES_BASE | DRV_ERROR_PARAMETER));

    ret = csi_aes_config(aes, 7, AES_KEY_LEN_BITS_128, AES_ENDIAN_LITTLE);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_AES_BASE | AES_ERROR_MODE));

    ret = csi_aes_config(aes, AES_MODE_ECB, 3, AES_ENDIAN_LITTLE);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_AES_BASE | AES_ERROR_DATA_BITS));

    ret = csi_aes_config(aes, AES_MODE_ECB, AES_KEY_LEN_BITS_128, 2);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_AES_BASE | AES_ERROR_ENDIAN));

    ret = csi_aes_config(aes, AES_MODE_ECB, AES_KEY_LEN_BITS_128, AES_ENDIAN_LITTLE);

    char key[16]  = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    //set_key
    ret = csi_aes_set_key(NULL, NULL, key, AES_KEY_LEN_BITS_128, AES_CRYPTO_MODE_ENCRYPT);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_AES_BASE | DRV_ERROR_PARAMETER));

    /*	//don't use
    ret = csi_aes_set_key(aes,NULL,key,3,AES_CRYPTO_MODE_ENCRYPT);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_AES_BASE |AES_ERROR_DATA_BITS));
    */

    ret = csi_aes_set_key(aes, NULL, key, AES_KEY_LEN_BITS_128, 2);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_AES_BASE | DRV_ERROR_PARAMETER));

    ret = csi_aes_set_key(aes, NULL, key, AES_KEY_LEN_BITS_128, AES_CRYPTO_MODE_ENCRYPT);

    char pass[16] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    char out[16];
    //ecb_crypto
    ret = csi_aes_ecb_crypto(NULL, NULL, pass, out, 16);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_AES_BASE | DRV_ERROR_PARAMETER));

    ret = csi_aes_ecb_crypto(aes, NULL, NULL, out, 16);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_AES_BASE | DRV_ERROR_PARAMETER));

    ret = csi_aes_ecb_crypto(aes, NULL, pass, NULL, 16);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_AES_BASE | DRV_ERROR_PARAMETER));

    ret = csi_aes_ecb_crypto(aes, NULL, pass, out, 0);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_AES_BASE | DRV_ERROR_PARAMETER));

    ret = csi_aes_ecb_crypto(aes, NULL, pass, out, 16);

    ret = csi_aes_config(aes, AES_MODE_CBC, AES_KEY_LEN_BITS_128, AES_ENDIAN_LITTLE);

    uint8_t iv[16]   = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    //cbc_crypto
    ret = csi_aes_cbc_crypto(NULL, NULL, pass, out, 16, iv);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_AES_BASE | DRV_ERROR_PARAMETER));

    ret = csi_aes_cbc_crypto(aes, NULL, NULL, out, 16, iv);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_AES_BASE | DRV_ERROR_PARAMETER));

    ret = csi_aes_cbc_crypto(aes, NULL, pass, NULL, 16, iv);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_AES_BASE | DRV_ERROR_PARAMETER));

    ret = csi_aes_cbc_crypto(aes, NULL, pass, out, 0, iv);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_AES_BASE | DRV_ERROR_PARAMETER));

    ret = csi_aes_cbc_crypto(aes, NULL, pass, out, 16, NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_AES_BASE | DRV_ERROR_PARAMETER));

    ret = csi_aes_cbc_crypto(aes, NULL, pass, out, 16, iv);

    //power control
    ret = csi_aes_power_control(NULL, DRV_POWER_OFF);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_AES_BASE | DRV_ERROR_UNSUPPORTED));
    ret = csi_aes_power_control(aes, 10);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_AES_BASE | DRV_ERROR_UNSUPPORTED));

    //cfb1_ctrpto
    ret = csi_aes_cfb1_crypto(NULL, NULL, NULL, NULL, 16, iv);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_AES_BASE | DRV_ERROR_UNSUPPORTED));

    //cfb8_ctrpto
    ret = csi_aes_cfb8_crypto(NULL, NULL, NULL, NULL, 16, iv);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_AES_BASE | DRV_ERROR_UNSUPPORTED));

    //cfb128_ctrpto
    ret = csi_aes_cfb128_crypto(NULL, NULL, NULL, NULL, 16, iv, NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_AES_BASE | DRV_ERROR_UNSUPPORTED));

    //ofb_ctrpto
    ret = csi_aes_ofb_crypto(NULL, NULL, NULL, NULL, 16, iv, NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_AES_BASE | DRV_ERROR_UNSUPPORTED));

    //ctr_ctrpto
    ret = csi_aes_ctr_crypto(NULL, NULL, NULL, NULL, 16, iv, iv, NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_AES_BASE | DRV_ERROR_UNSUPPORTED));

    ret = csi_aes_uninitialize(aes);
}

void aes_func_test()
{
    printf("Test starts here\n");
    aes_int_flag = 1;

    char key[16]  = {1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2};
    char key_128[16]  = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    unsigned char iv[16]   = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    char pass[16] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    char key_192[24] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    char key_256[32] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    char pass1[16] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};
    //char pass2[16]  = {1,2,3,4,1,2,3,4,2,2,2,2,2,2,2,2};

    char cbc_expect[][16] = {
        /**************************  CBC ****************************************/
        {0xb6, 0xae, 0xaf, 0xfa, 0x75, 0x2d, 0xc0, 0x8b, 0x51, 0x63, 0x97, 0x31, 0x76, 0x1a, 0xed, 0x00},
        {0x5f, 0x88, 0xef, 0x3f, 0xbd, 0xeb, 0xf2, 0xe4, 0xe2, 0x66, 0x65, 0x12, 0xd3, 0xbc, 0xb7, 0x0f},
        {0x72, 0x98, 0xca, 0xa5, 0x65, 0x03, 0x1e, 0xad, 0xc6, 0xce, 0x23, 0xd2, 0x3e, 0xa6, 0x63, 0x78},
        {0xe9, 0xe2, 0xd0, 0x68, 0xdc, 0xff, 0x48, 0x40, 0xc0, 0x6d, 0xf3, 0xa3, 0xfd, 0xe3, 0xe8, 0x0b},
        {0x18, 0x71, 0xb6, 0xb7, 0xe0, 0x1b, 0xc8, 0xe8, 0x63, 0x80, 0x05, 0xb3, 0x34, 0x37, 0x05, 0x18},
        {0x3b, 0x38, 0x7e, 0x87, 0xdf, 0x67, 0x82, 0x85, 0x47, 0x06, 0x72, 0xa9, 0xed, 0x50, 0x7e, 0xc0},
    };

    char ecb_expect[][16] = {
        /*****************************  ECB  ******************************************/
        {0x5e, 0x77, 0xe5, 0x9f, 0x8f, 0x85, 0x94, 0x34, 0x89, 0xa2, 0x41, 0x49, 0xc7, 0x5f, 0x4e, 0xc9},
        {0x98, 0xb8, 0x95, 0xa1, 0x45, 0xca, 0x4e, 0x0b, 0xf8, 0x3e, 0x69, 0x32, 0x81, 0xc1, 0xa0, 0x97},
        {0x9c, 0xac, 0x94, 0xc6, 0xb4, 0x85, 0x61, 0xf8, 0xff, 0xaa, 0xa7, 0x86, 0x16, 0xba, 0x48, 0x92},
        {0xc1, 0x80, 0xe3, 0x8f, 0xc4, 0x7e, 0x4c, 0xeb, 0xbb, 0xcd, 0xdd, 0x42, 0x6d, 0xa4, 0x97, 0xc0},
        {0x63, 0xee, 0x33, 0xba, 0xac, 0x1c, 0xc6, 0x7f, 0xe8, 0xbc, 0xc0, 0x91, 0x95, 0x3a, 0x7d, 0xef},
        {0x7f, 0xa7, 0xc6, 0xbd, 0x73, 0xd4, 0x69, 0x60, 0xe4, 0x89, 0x8a, 0xd3, 0xeb, 0x16, 0xa1, 0x48},
    };

    /*********** first three as parameters , last one as result ***********/
    aes_test_t cbc_check_list[] = {
        /********************************* Little Endian  ***********************************************/
        /************************* CBC MODE  ************************************************/
        /**********  128bit CBC NOPAD LE ENC *********/
        {key_128, iv, pass, AES_MODE_CBC, AES_CRYPTO_MODE_ENCRYPT, AES_KEY_LEN_BITS_128, AES_ENDIAN_LITTLE, cbc_expect[0]},
        /**********  128bit CBC NOPAD LE DEC *********/
        {key_128, iv, cbc_expect[0], AES_MODE_CBC, AES_CRYPTO_MODE_DECRYPT, AES_KEY_LEN_BITS_128, AES_ENDIAN_LITTLE, pass},
        /**********  192bit CBC NOPAD LE ENC *********/
        {key_192, iv, pass, AES_MODE_CBC, AES_CRYPTO_MODE_ENCRYPT, AES_KEY_LEN_BITS_192, AES_ENDIAN_LITTLE, cbc_expect[1]},
        /**********  192bit CBC NOPAD LE DEC *********/
        {key_192, iv, cbc_expect[1], AES_MODE_CBC, AES_CRYPTO_MODE_DECRYPT, AES_KEY_LEN_BITS_192, AES_ENDIAN_LITTLE, pass},
        /**********  256bit CBC NOPAD LE ENC *********/
        {key_256, iv, pass, AES_MODE_CBC, AES_CRYPTO_MODE_ENCRYPT, AES_KEY_LEN_BITS_256, AES_ENDIAN_LITTLE, cbc_expect[2]},
        /**********  256bit CBC NOPAD LE DEC *********/
        {key_256, iv, cbc_expect[2], AES_MODE_CBC, AES_CRYPTO_MODE_DECRYPT, AES_KEY_LEN_BITS_256, AES_ENDIAN_LITTLE, pass},

        /**********  Change the password, everything will be different in little/big endian ****/
        /**********  128bit CBC NOPAD LE ENC *********/
        {key_128, iv, pass1, AES_MODE_CBC, AES_CRYPTO_MODE_ENCRYPT, AES_KEY_LEN_BITS_128, AES_ENDIAN_LITTLE, cbc_expect[3]},
        /**********  128bit CBC NOPAD LE DEC *********/
        {key_128, iv, cbc_expect[3], AES_MODE_CBC, AES_CRYPTO_MODE_DECRYPT, AES_KEY_LEN_BITS_128, AES_ENDIAN_LITTLE, pass1},
        /**********  192bit CBC NOPAD LE ENC *********/
        {key_192, iv, pass1, AES_MODE_CBC, AES_CRYPTO_MODE_ENCRYPT, AES_KEY_LEN_BITS_192, AES_ENDIAN_LITTLE, cbc_expect[4]},
        /**********  192bit CBC NOPAD LE DEC *********/
        {key_192, iv, cbc_expect[4], AES_MODE_CBC, AES_CRYPTO_MODE_DECRYPT, AES_KEY_LEN_BITS_192, AES_ENDIAN_LITTLE, pass1},
        /**********  256bit CBC NOPAD LE ENC *********/
        {key_256, iv, pass1, AES_MODE_CBC, AES_CRYPTO_MODE_ENCRYPT, AES_KEY_LEN_BITS_256, AES_ENDIAN_LITTLE, cbc_expect[5]},
        /**********  256bit CBC NOPAD LE DEC *********/
        {key_256, iv, cbc_expect[5], AES_MODE_CBC, AES_CRYPTO_MODE_DECRYPT, AES_KEY_LEN_BITS_256, AES_ENDIAN_LITTLE, pass1},
        /*********   128bit CBC NOPAD   BE ENC**************/
//	CM6501 don't supported
#if 0
        {key_128, iv, pass, AES_MODE_CBC, AES_CRYPTO_MODE_ENCRYPT, AES_KEY_LEN_BITS_128, AES_ENDIAN_BIG, cbc_expect[0]},
        /*********   128bit CBC NOPAD   BE DEC**************/
        {key_128, iv, cbc_expect[0], AES_MODE_CBC, AES_CRYPTO_MODE_DECRYPT, AES_KEY_LEN_BITS_128, AES_ENDIAN_BIG, pass},
        /*********   192bit CBC NOPAD   BE ENC**************/
        {key_192, iv, pass, AES_MODE_CBC, AES_CRYPTO_MODE_ENCRYPT, AES_KEY_LEN_BITS_192, AES_ENDIAN_BIG, cbc_expect[1]},
        /*********   192bit CBC NOPAD   BE DEC**************/
        {key_192, iv, cbc_expect[1], AES_MODE_CBC, AES_CRYPTO_MODE_DECRYPT, AES_KEY_LEN_BITS_192, AES_ENDIAN_BIG, pass},
        /*********   256bit CBC NOPAD   BE ENC**************/
        {key_256, iv, pass, AES_MODE_CBC, AES_CRYPTO_MODE_ENCRYPT, AES_KEY_LEN_BITS_256, AES_ENDIAN_BIG, cbc_expect[2]},
        /*********   256bit CBC NOPAD   BE DEC**************/
        {key_256, iv, cbc_expect[2], AES_MODE_CBC, AES_CRYPTO_MODE_DECRYPT, AES_KEY_LEN_BITS_256, AES_ENDIAN_BIG, pass},
        /*********   128bit CBC NOPAD   BE ENC**************/
        {key_128, iv, pass1, AES_MODE_CBC, AES_CRYPTO_MODE_ENCRYPT, AES_KEY_LEN_BITS_128, AES_ENDIAN_BIG, cbc_expect[3]},
        /*********   128bit CBC NOPAD   BE DEC**************/
        {key_128, iv, cbc_expect[3], AES_MODE_CBC, AES_CRYPTO_MODE_DECRYPT, AES_KEY_LEN_BITS_128, AES_ENDIAN_BIG, pass1},
        /*********   192bit CBC NOPAD   BE ENC**************/
        {key_192, iv, pass1, AES_MODE_CBC, AES_CRYPTO_MODE_ENCRYPT, AES_KEY_LEN_BITS_192, AES_ENDIAN_BIG, cbc_expect[4]},
        /*********   192bit CBC NOPAD   BE DEC**************/
        {key_192, iv, cbc_expect[4], AES_MODE_CBC, AES_CRYPTO_MODE_DECRYPT, AES_KEY_LEN_BITS_192, AES_ENDIAN_BIG, pass1},
        /*********   256bit CBC NOPAD   BE ENC**************/
        {key_256, iv, pass1, AES_MODE_CBC, AES_CRYPTO_MODE_ENCRYPT, AES_KEY_LEN_BITS_256, AES_ENDIAN_BIG, cbc_expect[5]},
        /*********   256bit CBC NOPAD   BE DEC**************/
        {key_256, iv, cbc_expect[5], AES_MODE_CBC, AES_CRYPTO_MODE_DECRYPT, AES_KEY_LEN_BITS_256, AES_ENDIAN_BIG, pass1},
#endif
    };

    aes_test_t ecb_check_list[] = {
        /***************   ECB MODE TEST  **********************/
        /**********  128bit ECB NOPAD LE ENC *********/
        {key_128, NULL, pass, AES_MODE_ECB, AES_CRYPTO_MODE_ENCRYPT, AES_KEY_LEN_BITS_128, AES_ENDIAN_LITTLE, ecb_expect[0]},
        /**********  128bit ECB NOPAD LE DEC *********/
        {key_128, NULL, ecb_expect[0], AES_MODE_ECB, AES_CRYPTO_MODE_DECRYPT, AES_KEY_LEN_BITS_128, AES_ENDIAN_LITTLE, pass},
        /**********  192bit ECB NOPAD LE ENC *********/
        {key_192, NULL, pass, AES_MODE_ECB, AES_CRYPTO_MODE_ENCRYPT, AES_KEY_LEN_BITS_192, AES_ENDIAN_LITTLE, ecb_expect[1]},
        /**********  192bit ECB NOPAD LE DEC *********/
        {key_192, NULL, ecb_expect[1], AES_MODE_ECB, AES_CRYPTO_MODE_DECRYPT, AES_KEY_LEN_BITS_192, AES_ENDIAN_LITTLE, pass},
        /**********  256bit ECB NOPAD LE ENC *********/
        {key_256, NULL, pass, AES_MODE_ECB, AES_CRYPTO_MODE_ENCRYPT, AES_KEY_LEN_BITS_256, AES_ENDIAN_LITTLE, ecb_expect[2]},
        /**********  256bit ECB NOPAD LE DEC *********/
        {key_256, NULL, ecb_expect[2], AES_MODE_ECB, AES_CRYPTO_MODE_DECRYPT, AES_KEY_LEN_BITS_256, AES_ENDIAN_LITTLE, pass},

        /**********  Change the password, everything will be different in little/big endian ****/
        /**********  128bit ECB NOPAD LE ENC *********/
        {key_128, NULL, pass1, AES_MODE_ECB, AES_CRYPTO_MODE_ENCRYPT, AES_KEY_LEN_BITS_128, AES_ENDIAN_LITTLE, ecb_expect[3]},
        /**********  128bit ECB NOPAD LE DEC *********/
        {key_128, NULL, ecb_expect[3], AES_MODE_ECB, AES_CRYPTO_MODE_DECRYPT, AES_KEY_LEN_BITS_128, AES_ENDIAN_LITTLE, pass1},
        /**********  192bit ECB NOPAD LE ENC *********/
        {key_192, NULL, pass1, AES_MODE_ECB, AES_CRYPTO_MODE_ENCRYPT, AES_KEY_LEN_BITS_192, AES_ENDIAN_LITTLE, ecb_expect[4]},
        /**********  192bit ECB NOPAD LE DEC *********/
        {key_192, NULL, ecb_expect[4], AES_MODE_ECB, AES_CRYPTO_MODE_DECRYPT, AES_KEY_LEN_BITS_192, AES_ENDIAN_LITTLE, pass1},
        /**********  256bit ECB NOPAD LE ENC *********/
        {key_256, NULL, pass1, AES_MODE_ECB, AES_CRYPTO_MODE_ENCRYPT, AES_KEY_LEN_BITS_256, AES_ENDIAN_LITTLE, ecb_expect[5]},
        /**********  256bit ECB NOPAD LE DEC *********/
        {key_256, NULL, ecb_expect[5], AES_MODE_ECB, AES_CRYPTO_MODE_DECRYPT, AES_KEY_LEN_BITS_256, AES_ENDIAN_LITTLE, pass1},

        /**********************************  Big Endian  ****************************************************/
// CM6501 don't supported
#if 0
        /*********   128bit ECB NOPAD   BE ENC**************/
        {key_128, NULL, pass, AES_MODE_ECB, AES_CRYPTO_MODE_ENCRYPT, AES_KEY_LEN_BITS_128, AES_ENDIAN_BIG, ecb_expect[0]},
        /*********   128bit ECB NOPAD   BE DEC**************/
        {key_128, NULL, ecb_expect[0], AES_MODE_ECB, AES_CRYPTO_MODE_DECRYPT, AES_KEY_LEN_BITS_128, AES_ENDIAN_BIG, pass},
        /*********   192bit ECB NOPAD   BE ENC**************/
        {key_192, NULL, pass, AES_MODE_ECB, AES_CRYPTO_MODE_ENCRYPT, AES_KEY_LEN_BITS_192, AES_ENDIAN_BIG, ecb_expect[1]},
        /*********   192bit ECB NOPAD   BE DEC**************/
        {key_192, NULL, ecb_expect[1], AES_MODE_ECB, AES_CRYPTO_MODE_DECRYPT, AES_KEY_LEN_BITS_192, AES_ENDIAN_BIG, pass},
        /*********   256bit ECB NOPAD   BE ENC**************/
        {key_256, NULL, pass, AES_MODE_ECB, AES_CRYPTO_MODE_ENCRYPT, AES_KEY_LEN_BITS_256, AES_ENDIAN_BIG, ecb_expect[2]},
        /*********   256bit ECB NOPAD   BE DEC**************/
        {key_256, NULL, ecb_expect[2], AES_MODE_ECB, AES_CRYPTO_MODE_DECRYPT, AES_KEY_LEN_BITS_256, AES_ENDIAN_BIG, pass},
        /*********   128bit ECB NOPAD   BE ENC**************/
        {key_128, NULL, pass1, AES_MODE_ECB, AES_CRYPTO_MODE_ENCRYPT, AES_KEY_LEN_BITS_128, AES_ENDIAN_BIG, ecb_expect[3]},
        /*********   128bit ECB NOPAD   BE DEC**************/
        {key_128, NULL, ecb_expect[3], AES_MODE_ECB, AES_CRYPTO_MODE_DECRYPT, AES_KEY_LEN_BITS_128, AES_ENDIAN_BIG, pass1},
        /*********   192bit ECB NOPAD   BE ENC**************/
        {key_192, NULL, pass1, AES_MODE_ECB, AES_CRYPTO_MODE_ENCRYPT, AES_KEY_LEN_BITS_192, AES_ENDIAN_BIG, ecb_expect[4]},
        /*********   192bit ECB NOPAD   BE DEC**************/
        {key_192, NULL, ecb_expect[4], AES_MODE_ECB, AES_CRYPTO_MODE_DECRYPT, AES_KEY_LEN_BITS_192, AES_ENDIAN_BIG, pass1},
        /*********   256bit ECB NOPAD   BE ENC**************/
        {key_256, NULL, pass1, AES_MODE_ECB, AES_CRYPTO_MODE_ENCRYPT, AES_KEY_LEN_BITS_256, AES_ENDIAN_BIG, ecb_expect[5]},
        /*********   256bit ECB NOPAD   BE DEC**************/
        {key_256, NULL, ecb_expect[5], AES_MODE_ECB, AES_CRYPTO_MODE_DECRYPT, AES_KEY_LEN_BITS_256, AES_ENDIAN_BIG, pass1},
#endif
    };

    int i;
    uint32_t ret;
    uint8_t out[16];
    unsigned char temp_iv[16];
    aes_handle_t aes;

//	aes supported interrupt
#if 0
    /********** Initialize AES  ************/
    printf("Begin to init aes\n");
    aes = csi_aes_initialize(0, aes_interrupt_handler);
    ASSERT_TRUE(aes != NULL);

    /**************  CBC CRYPTO ****************/
    for (i = 0; i < sizeof(cbc_check_list) / (sizeof(aes_test_t)); i++) {
        if (cbc_check_list[i].iv != NULL) {
            memcpy(temp_iv, cbc_check_list[i].iv, 16);
        }

        ret = csi_aes_config(aes, cbc_check_list[i].aes_mode, cbc_check_list[i].bits, cbc_check_list[i].endian);
        ASSERT_TRUE(ret == 0);

        ret = csi_aes_set_key(aes, NULL, cbc_check_list[i].key, cbc_check_list[i].bits, cbc_check_list[i].crypto_mode);

        ret = csi_aes_cbc_crypto(aes, NULL, (void *)cbc_check_list[i].pass, (void *)out, 16 , temp_iv);
        ASSERT_TRUE(ret == 0);

        while (aes_int_flag);

        aes_int_flag = 1;

        ASSERT_TRUE(memcmp(out, cbc_check_list[i].expect, 16) == 0);
    }

    /****************  ECB  CRYPTO  ******************/
    for (i = 0; i < sizeof(ecb_check_list) / (sizeof(aes_test_t)); i++) {
        ret = csi_aes_config(aes, ecb_check_list[i].aes_mode, ecb_check_list[i].bits, ecb_check_list[i].endian);
        ASSERT_TRUE(ret == 0);

        ret = csi_aes_set_key(aes, NULL, ecb_check_list[i].key, ecb_check_list[i].bits, ecb_check_list[i].crypto_mode);

        ASSERT_TRUE(ret == 0);

        ret = csi_aes_ecb_crypto(aes, NULL, (void *)ecb_check_list[i].pass, (void *)out, 16);
        ASSERT_TRUE(ret == 0);

        while (aes_int_flag);

        aes_int_flag = 1;

        ASSERT_TRUE(memcmp(out, ecb_check_list[i].expect, 16) == 0);
    }

#endif

// aes don't supported interrupt
#if 1
    /********** Initialize AES  ************/
    printf("Begin to init aes\n");
    aes = csi_aes_initialize(0, NULL);
    ASSERT_TRUE(aes != NULL);

    uint8_t lock;
    aes_status_t status;

    /**************  CBC CRYPTO ****************/
    for (i = 0; i < sizeof(cbc_check_list) / (sizeof(aes_test_t)); i++) {
        if (cbc_check_list[i].iv != NULL) {
            memcpy(temp_iv, cbc_check_list[i].iv, 16);
        }

        ret = csi_aes_config(aes, cbc_check_list[i].aes_mode, cbc_check_list[i].bits, cbc_check_list[i].endian);
        ASSERT_TRUE(ret == 0);

        ret = csi_aes_set_key(aes, NULL, cbc_check_list[i].key, cbc_check_list[i].bits, cbc_check_list[i].crypto_mode);

        ret = csi_aes_cbc_crypto(aes, NULL, (void *)cbc_check_list[i].pass, (void *)out, 16 , temp_iv);
        ASSERT_TRUE(ret == 0);
        lock = 1;

        while (lock) {
            status = csi_aes_get_status(aes);

            if (status.busy == 0) {
                lock = 0;
            } else {
                mdelay(1000);
            }
        }

        ASSERT_TRUE(memcmp(out, cbc_check_list[i].expect, 16) == 0);
    }

    /****************  ECB  CRYPTO  ******************/
    for (i = 0; i < sizeof(ecb_check_list) / (sizeof(aes_test_t)); i++) {
        ret = csi_aes_config(aes, ecb_check_list[i].aes_mode, ecb_check_list[i].bits, ecb_check_list[i].endian);
        ASSERT_TRUE(ret == 0);

        ret = csi_aes_set_key(aes, NULL, ecb_check_list[i].key, ecb_check_list[i].bits, ecb_check_list[i].crypto_mode);

        ASSERT_TRUE(ret == 0);

        ret = csi_aes_ecb_crypto(aes, NULL, (void *)ecb_check_list[i].pass, (void *)out, 16);
        ASSERT_TRUE(ret == 0);
        lock = 1;

        while (lock) {
            status = csi_aes_get_status(aes);

            if (status.busy == 0) {
                lock = 0;
            } else {
                mdelay(1000);
            }
        }

        ASSERT_TRUE(memcmp(out, ecb_check_list[i].expect, 16) == 0);
    }

#endif

    csi_aes_uninitialize(aes);
    printf("Test finished\n");
}

int test_aes(void)
{
    dtest_suite_info_t test_suite_info = {
        "test_aes", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
        {"aes_if_test", aes_if_test, 1},
        {"aes_func_test", aes_func_test, 1},
        {NULL, NULL, 0}
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
