/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     test_sha.c
 * @brief    the test function for the sh driver
 * @version  V1.0
 * @date     20. July 2016
 ******************************************************************************/
#include <stdio.h>
#include "dtest.h"
#include "soc.h"
#include "drv/sha.h"
#include "test_driver_config.h"


#define CONFIG_SHA_BLOCK_MODE   1

sha_handle_t *sha = NULL;
static uint8_t sha1_int_flag = 1;

static void sha_interrupt_handler(int32_t idx, sha_event_e event)
{
    sha1_int_flag = 0;
}

#if SHA_HASH_LITTLE_ENDIAN
static uint32_t data_in_little[][34] = {
    {0x61000000},
    {
        0x61626364, 0x65666768, 0x61626364, 0x65666768, 0x61626364, 0x65666768,
        0x61626364, 0x65666768, 0x61626364, 0x65666768, 0x61626364, 0x65666768,
        0x61626364, 0x65666768, 0x61626364, 0x65666768
    },
    {
        0x61626364, 0x65666768, 0x61626364, 0x65666768, 0x61626364, 0x65666768,
        0x61626364, 0x65666768, 0x61626364, 0x65666768, 0x61626364, 0x65666768,
        0x61626364, 0x65666768, 0x61626364, 0x65666768, 0x61626364, 0x65666768
    },
    {
        0x61626364, 0x65666768, 0x61626364, 0x65666768, 0x61626364, 0x65666768,
        0x61626364, 0x65666768, 0x61626364, 0x65666768, 0x61626364, 0x65666768,
        0x61626364, 0x65666768, 0x61626364, 0x65666768, 0x61626364, 0x65666768,
        0x61626364, 0x65666768, 0x61626364, 0x65666768, 0x61626364, 0x65666768,
        0x61626364, 0x65666768, 0x61626364, 0x65666768, 0x61626364, 0x65666768,
        0x61626364, 0x65666768
    },
    {
        0x61626364, 0x65666768, 0x61626364, 0x65666768, 0x61626364, 0x65666768,
        0x61626364, 0x65666768, 0x61626364, 0x65666768, 0x61626364, 0x65666768,
        0x61626364, 0x65666768, 0x61626364, 0x65666768, 0x61626364, 0x65666768,
        0x61626364, 0x65666768, 0x61626364, 0x65666768, 0x61626364, 0x65666768,
        0x61626364, 0x65666768, 0x61626364, 0x65666768, 0x61626364, 0x65666768,
        0x61626364, 0x65666768, 0x61626364, 0x65666768
    }
};
#endif

static uint32_t data_in_big[][34] = {
    {0x61},
    {
        0x64636261, 0x68676665, 0x64636261, 0x68676665, 0x64636261, 0x68676665,
        0x64636261, 0x68676665, 0x64636261, 0x68676665, 0x64636261, 0x68676665,
        0x64636261, 0x68676665, 0x64636261, 0x68676665
    },
    {
        0x64636261, 0x68676665, 0x64636261, 0x68676665, 0x64636261, 0x68676665,
        0x64636261, 0x68676665, 0x64636261, 0x68676665, 0x64636261, 0x68676665,
        0x64636261, 0x68676665, 0x64636261, 0x68676665, 0x64636261, 0x68676665
    },
    {
        0x64636261, 0x68676665, 0x64636261, 0x68676665, 0x64636261, 0x68676665,
        0x64636261, 0x68676665, 0x64636261, 0x68676665, 0x64636261, 0x68676665,
        0x64636261, 0x68676665, 0x64636261, 0x68676665, 0x64636261, 0x68676665,
        0x64636261, 0x68676665, 0x64636261, 0x68676665, 0x64636261, 0x68676665,
        0x64636261, 0x68676665, 0x64636261, 0x68676665, 0x64636261, 0x68676665,
        0x64636261, 0x68676665
    },
    {
        0x64636261, 0x68676665, 0x64636261, 0x68676665, 0x64636261, 0x68676665,
        0x64636261, 0x68676665, 0x64636261, 0x68676665, 0x64636261, 0x68676665,
        0x64636261, 0x68676665, 0x64636261, 0x68676665, 0x64636261, 0x68676665,
        0x64636261, 0x68676665, 0x64636261, 0x68676665, 0x64636261, 0x68676665,
        0x64636261, 0x68676665, 0x64636261, 0x68676665, 0x64636261, 0x68676665,
        0x64636261, 0x68676665, 0x64636261, 0x68676665
    }
};

static uint32_t expect_out[][16] = {
    {0x37e4f786, 0xfca7a5fa, 0xdc1d5de1, 0xeaeaeab9, 0xb8677637},
    {0xeec1d8cb, 0x82117907, 0x90225616, 0x2d909a57, 0x0a08c7a9},
    {0x340c0ed0, 0xf455e02d, 0x8f67932e, 0x69831da1, 0x7c873c58},
    {0x5995e2ad, 0x2cc81839, 0x4528bb9a, 0x0ccb6b87, 0xc86aa9d1},
    {0xc136c950, 0x12ccce26, 0xfa939224, 0x4ef6781a, 0xbc96fbf1},
    {0xa320f437, 0x4e15c9a3, 0x39dbbccd, 0x12ab1f8b, 0x36fab884, 0xe4a6f7e9, 0xcadf5659},
    {0x9d96864d, 0xcf100639, 0x8846e4c6, 0x8da10a1e, 0x3c860112, 0x6cd51260, 0x51834d2f, 0x341f2485},
    {0xc33b8797, 0xa74a995a, 0x100f7f41, 0xd8ea62da, 0x04327345, 0x3f3f5e75, 0x1c8f24e5, 0x7f125af8, 0xce353a99, 0x6445b2ab, 0x159974aa, 0x72288db9},
    {0x4fe77661, 0x962f33d1, 0xee7ff604, 0xfca3605b, 0x6ca350ba, 0x910573ff, 0x64d764c9, 0x02c481cc, 0x31e5bf39, 0xb48c8acc, 0x8ff671b2, 0x8ce654c7, 0x5a24ff47, 0xdbb5df5f, 0x9f01c78c, 0x53a11fac}
};

typedef struct {
    sha_mode_e  mode;
    sha_endian_mode_e endian;
    uint32_t *input;
    uint32_t *expect_out;
} sha_test_t;

static sha_test_t sha1_8_cases[] = {
#if SHA_HASH_LITTLE_ENDIAN
    {NULL, SHA_ENDIAN_MODE_LITTLE, data_in_little[0], expect_out[0]},
#endif
#if SHA_HASH_BIG_ENDIAN
    {0, SHA_ENDIAN_MODE_BIG, data_in_big[0], expect_out[0]},
#endif
};

static sha_test_t sha1_512_cases[] = {
#if SHA_HASH_LITTLE_ENDIAN
    {NULL, SHA_ENDIAN_MODE_LITTLE, data_in_little[1], expect_out[1]},
#endif
#if SHA_HASH_BIG_ENDIAN
    {0, SHA_ENDIAN_MODE_BIG, data_in_big[1], expect_out[1]},
#endif
};

static sha_test_t sha1_576_cases[] = {
#if SHA_HASH_LITTLE_ENDIAN
    {NULL, SHA_ENDIAN_MODE_LITTLE, data_in_little[2], expect_out[2]},
#endif
#if SHA_HASH_BIG_ENDIAN
    {0, SHA_ENDIAN_MODE_BIG, data_in_big[2], expect_out[2]},
#endif
};

static sha_test_t sha1_1024_cases[] = {
#if SHA_HASH_LITTLE_ENDIAN
    {NULL, SHA_ENDIAN_MODE_LITTLE, data_in_little[3], expect_out[3]},
#endif
#if SHA_HASH_BIG_ENDIAN
    {0, SHA_ENDIAN_MODE_BIG, data_in_big[3], expect_out[3]},
#endif
};

static sha_test_t sha1_1088_cases[] = {
#if SHA_HASH_LITTLE_ENDIAN
    {NULL, SHA_ENDIAN_MODE_LITTLE, data_in_little[4], expect_out[4]},
#endif
#if SHA_HASH_BIG_ENDIAN
    {0, SHA_ENDIAN_MODE_BIG, data_in_big[4], expect_out[4]},
#endif
};

static sha_test_t sha224_1024_cases[] = {
#if SHA_HASH_LITTLE_ENDIAN
    {NULL, SHA_ENDIAN_MODE_LITTLE, data_in_little[3], expect_out[5]},
#endif
#if SHA_HASH_BIG_ENDIAN
    {0, SHA_ENDIAN_MODE_BIG, data_in_big[3], expect_out[5]},
#endif
};

static sha_test_t sha256_1024_cases[] = {
#if SHA_HASH_LITTLE_ENDIAN
    {NULL, SHA_ENDIAN_MODE_LITTLE, data_in_little[3], expect_out[6]},
#endif
#if SHA_HASH_BIG_ENDIAN
    {0, SHA_ENDIAN_MODE_BIG, data_in_big[3], expect_out[6]},
#endif
};

static sha_test_t sha384_1024_cases[] = {
#if SHA_HASH_LITTLE_ENDIAN
    {NULL, SHA_ENDIAN_MODE_LITTLE, data_in_little[3], expect_out[7]},
#endif
#if SHA_HASH_BIG_ENDIAN
    {0, SHA_ENDIAN_MODE_BIG, data_in_big[3], expect_out[7]},
#endif
};

static sha_test_t sha512_1024_cases[] = {
#if SHA_HASH_LITTLE_ENDIAN
    {NULL, SHA_ENDIAN_MODE_LITTLE, data_in_little[3], expect_out[8]},
#endif
#if SHA_HASH_BIG_ENDIAN
    {0, SHA_ENDIAN_MODE_BIG, data_in_big[3], expect_out[8]},
#endif
};

typedef struct {
    sha_mode_e  mode;
    sha_endian_mode_e endian;
    int32_t expect_out;
} sha_config_test_t;
static sha_config_test_t csi_sha_config_cases[] = {
#if SHA_HASH_BIG_ENDIAN
    {SHA_MODE_512_224 + 1, SHA_ENDIAN_MODE_BIG, (CSI_DRV_ERRNO_SHA_BASE | SHA_ERROR_MODE)},
    {SHA_MODE_1, SHA_ENDIAN_MODE_BIG + 2, (CSI_DRV_ERRNO_SHA_BASE | SHA_ERROR_ENDIAN)},
#endif
#if SHA_HASH_LITTLE_ENDIAN
    {SHA_MODE_1, SHA_ENDIAN_MODE_LITTLE + 1, (CSI_DRV_ERRNO_SHA_BASE | SHA_ERROR_ENDIAN)},
    {SHA_MODE_512_224 + 1, SHA_ENDIAN_MODE_LITTLE, (CSI_DRV_ERRNO_SHA_BASE | SHA_ERROR_MODE)},
#endif
};

typedef struct {
    uint32_t *input;
    int32_t expect_out;
} sha_update_test_t;
static sha_update_test_t csi_sha_update_cases[] = {
    {NULL, (CSI_DRV_ERRNO_SHA_BASE | DRV_ERROR_PARAMETER)},
};

uint8_t sha_contex[SHA_CONTEXT_SIZE] = {0x0};

static void test_csi_sha_config(void)
{
    int i;
    int k;
    uint32_t ret = 0;
    uint32_t index;
    index = 1;

    for (i = 0; i < index; i++) {
        sha = csi_sha_initialize(i, &sha_contex, sha_interrupt_handler);
        ASSERT_TRUE(sha != NULL);

        for (k = 0; k < sizeof(csi_sha_config_cases) / (sizeof(sha_config_test_t)); k++) {
            ret = csi_sha_config(sha, csi_sha_config_cases[k].mode, csi_sha_config_cases[k].endian);
            ASSERT_TRUE(ret == csi_sha_config_cases[k].expect_out);
        }
    }

}

static void test_csi_sha_update(void)
{
    int i;
    int k;
    uint32_t ret = 0;
    uint32_t index;
    index = 1;

    for (i = 0; i < index; i++) {
        sha = csi_sha_initialize(i, &sha_contex, sha_interrupt_handler);
        ASSERT_TRUE(sha != NULL);

        for (k = 0; k < sizeof(csi_sha_update_cases) / (sizeof(sha_update_test_t)); k++) {
            ret = csi_sha_update(sha, NULL, csi_sha_update_cases[k].input, 1);
            ASSERT_TRUE(ret == csi_sha_update_cases[k].expect_out);
        }
    }
}

static void test_drv_sha_start(void)
{
    int i;
    uint32_t ret = 0;
    uint32_t index;
    index = 1;

    for (i = 0; i < index; i++) {
        sha = csi_sha_initialize(i, &sha_contex, sha_interrupt_handler);
        ASSERT_TRUE(sha != NULL);
        ret = csi_sha_start(NULL, NULL);
        ASSERT_TRUE(ret != 0);
    }
}

static void test_csi_sha_finish(void)
{
    int i;
    uint32_t ret = 0;
    uint32_t index;
    uint32_t data;
    index = 1;

    for (i = 0; i < index; i++) {
        sha = csi_sha_initialize(i, &sha_contex, sha_interrupt_handler);
        ASSERT_TRUE(sha != NULL);
        ret = csi_sha_finish(NULL, &data, NULL);
        ASSERT_TRUE(ret != 0);
        ret = csi_sha_finish(sha, NULL, NULL);
        ASSERT_TRUE(ret != 0);
    }
}
/*****************************************************************************
test_sha1_8: test the 8 bit encode

INPUT: NULL

RETURN: NULL

*****************************************************************************/
void test_sha1_8()
{
    uint32_t data[5] = {0};

    printf("- - -Testing sha-1 8bit- - -\n");

    int i;
    int j;
    int k;
    uint32_t ret = 0;
    uint32_t index;
    uint32_t idx;
    char string[][10] = {"little", "big"};
    i = 5;
    index = 1;

    for (i = 0; i < index; i++) {
        sha = csi_sha_initialize(i, &sha_contex, sha_interrupt_handler);
        ASSERT_TRUE(sha != NULL);

        for (k = 0; k < sizeof(sha1_8_cases) / (sizeof(sha_test_t)); k++) {
            ret = csi_sha_config(sha, SHA_MODE_1, sha1_8_cases[k].endian);
            ASSERT_TRUE(ret == 0);
            ret = csi_sha_start(sha, NULL);
            ASSERT_TRUE(ret == 0);
            ret = csi_sha_update(sha, NULL, sha1_8_cases[k].input, 1);
            ASSERT_TRUE(ret == 0);

#if CONFIG_SHA_BLOCK_MODE   /*it is also valid by get status*/
            sha_status_t status;

            while (1) {
                status = csi_sha_get_status(sha);

                if (status.busy == 0) {
                    break;
                }
            }

            ret = csi_sha_finish(sha, NULL, data);
            ASSERT_TRUE(ret == 0);
#else
            ret = csi_sha_finish(sha, NULL, data);
            ASSERT_TRUE(ret == 0);

            while (sha1_int_flag);

            sha1_int_flag = 1;

#endif

            for (j = 0; j < sizeof(data) / 4; j++) {
                if (data[j] != sha1_8_cases[k].expect_out[j]) {
                    break;
                }
            }

            if (sha1_8_cases[k].endian == SHA_ENDIAN_MODE_LITTLE) {
                idx = 0;
            } else {
                idx = 1;
            }

            if (j == sizeof(data) / 4) {
                printf("index %d sha1-8 %s encode successfully\n", i, string[idx]);
            } else {
                printf("index %d sha1-8 %s encode failed\n", i, string[idx]);
            }
        }
    }
}
/*****************************************************************************
test_sha_512: test the 512 bit encode

INPUT: NULL

RETURN: NULL

*****************************************************************************/
void test_sha1_512()
{
    uint32_t data[5];
    printf("- - -Testing sha-1 512bit- - -\n");

    int i;
    int j;
    int k;
    uint32_t index;
    uint32_t idx;
    char string[][10] = {"little", "big"};
    uint32_t ret = 0;
    i = 5;

    index = 1;

    for (i = 0; i < index; i++) {
        sha = csi_sha_initialize(i, &sha_contex, sha_interrupt_handler);
        ASSERT_TRUE(sha != NULL);

        for (k = 0; k < sizeof(sha1_512_cases) / (sizeof(sha_test_t)); k++) {
            ret = csi_sha_config(sha, SHA_MODE_1, sha1_512_cases[k].endian);
            ASSERT_TRUE(ret == 0);
            ret = csi_sha_start(sha, NULL);
            ASSERT_TRUE(ret == 0);
            ret = csi_sha_update(sha, NULL, sha1_512_cases[k].input, 64);
            ASSERT_TRUE(ret == 0);

#if CONFIG_SHA_BLOCK_MODE   /*it is also valid by get status*/
            sha_status_t status;

            while (1) {
                status = csi_sha_get_status(sha);

                if (status.busy == 0) {
                    break;
                }
            }

            ret = csi_sha_finish(sha, NULL, data);
            ASSERT_TRUE(ret == 0);
#else
            ret = csi_sha_finish(sha, NULL, data);
            ASSERT_TRUE(ret == 0);

            while (sha1_int_flag);

            sha1_int_flag = 1;

#endif

            for (j = 0; j < sizeof(data) / 4; j++) {
                if (data[j] != sha1_512_cases[k].expect_out[j]) {
                    break;
                }
            }

            if (sha1_512_cases[k].endian == SHA_ENDIAN_MODE_LITTLE) {
                idx = 0;
            } else {
                idx = 1;
            }

            if (j == sizeof(data) / 4) {
                printf("index %d sha1-512 %s encode successfully\n", i, string[idx]);
            } else {
                printf("index %d sha1-512 %s encode failed\n", i, string[idx]);
            }
        }
    }
}

/*****************************************************************************
test_sha1_576: test the 576 bit encode

INPUT: NULL

RETURN: NULL

*****************************************************************************/
void test_sha1_576()
{
    uint32_t data[5];

    printf("- - -Testing sha-1 576bit- - -\n");

    int i;
    int j;
    int k;
    uint32_t ret = 0;
    uint32_t index;
    uint32_t idx;
    char string[][10] = {"little", "big"};
    i = 5;

    index = 1;

    for (i = 0; i < index; i++) {
        sha = csi_sha_initialize(i, &sha_contex, sha_interrupt_handler);
        ASSERT_TRUE(sha != NULL);

        for (k = 0; k < sizeof(sha1_576_cases) / (sizeof(sha_test_t)); k++) {
            ret = csi_sha_config(sha, SHA_MODE_1, sha1_576_cases[k].endian);
            ASSERT_TRUE(ret == 0);
            ret = csi_sha_start(sha, NULL);
            ASSERT_TRUE(ret == 0);
            ret = csi_sha_update(sha, NULL, sha1_576_cases[k].input, 72);
            ASSERT_TRUE(ret == 0);
#if CONFIG_SHA_BLOCK_MODE   /*it is also valid by get status*/
            sha_status_t status;

            while (1) {
                status = csi_sha_get_status(sha);

                if (status.busy == 0) {
                    break;
                }
            }

            ret = csi_sha_finish(sha, NULL, data);
            ASSERT_TRUE(ret == 0);
#else

            ret = csi_sha_finish(sha, NULL, data);
            ASSERT_TRUE(ret == 0);

            while (sha1_int_flag);

            sha1_int_flag = 1;

#endif

            for (j = 0; j < sizeof(data) / 4; j++) {
                if (data[j] != sha1_576_cases[k].expect_out[j]) {
                    break;
                }
            }

            if (sha1_576_cases[k].endian == SHA_ENDIAN_MODE_LITTLE) {
                idx = 0;
            } else {
                idx = 1;
            }

            if (j == sizeof(data) / 4) {
                printf("index %d sha1-576 %s encode successfully\n", i, string[idx]);
            } else {
                printf("index %d sha1-576 %s encode failed\n", i, string[idx]);
            }
        }
    }
}

/*****************************************************************************
test_sha1_1024: test the 1024 bit encode

INPUT: NULL

RETURN: NULL

*****************************************************************************/
void test_sha1_1024()
{
    uint32_t data[5];
    printf("- - -Testing sha-1 1024bit- - -\n");

    int i;
    int j;
    int k;
    uint32_t ret = 0;
    uint32_t index;
    uint32_t idx;
    char string[][10] = {"little", "big"};
    i = 5;

    index = 1;

    for (i = 0; i < index; i++) {
        sha = csi_sha_initialize(i, &sha_contex, sha_interrupt_handler);
        ASSERT_TRUE(sha != NULL);

        for (k = 0; k < sizeof(sha1_1024_cases) / (sizeof(sha_test_t)); k++) {
            ret = csi_sha_config(sha, SHA_MODE_1, sha1_1024_cases[k].endian);
            ASSERT_TRUE(ret == 0);
            ret = csi_sha_start(sha, NULL);
            ASSERT_TRUE(ret == 0);
            ret = csi_sha_update(sha, NULL, sha1_1024_cases[k].input, 128);
            ASSERT_TRUE(ret == 0);

#if CONFIG_SHA_BLOCK_MODE   /*it is also valid by get status*/
            sha_status_t status;

            while (1) {
                status = csi_sha_get_status(sha);

                if (status.busy == 0) {
                    break;
                }
            }

            ret = csi_sha_finish(sha, NULL, data);
            ASSERT_TRUE(ret == 0);
#else
            ret = csi_sha_finish(sha, NULL, data);
            ASSERT_TRUE(ret == 0);

            while (sha1_int_flag);

            sha1_int_flag = 1;
#endif

            for (j = 0; j < sizeof(data) / 4; j++) {
                if (data[j] != sha1_1024_cases[k].expect_out[j]) {
                    break;
                }
            }

            if (sha1_1024_cases[k].endian == SHA_ENDIAN_MODE_LITTLE) {
                idx = 0;
            } else {
                idx = 1;
            }

            if (j == sizeof(data) / 4) {
                printf("index %d sha1-1024 %s encode successfully\n", i, string[idx]);
            } else {
                printf("index %d sha1-1024 %s encode failed\n", i, string[idx]);
            }
        }
    }
}
/*****************************************************************************
test_sha1_1088: test the 1088 bit encode

INPUT: NULL

RETURN: NULL

*****************************************************************************/
void test_sha1_1088()
{
    uint32_t data[5];
    printf("- - -Testing sha-1 1088bit- - -\n");

    int i;
    int j;
    int k;
    uint32_t ret = 0;
    uint32_t index;
    uint32_t idx;
    char string[][10] = {"little", "big"};
    i = 5;

    index = 1;

    for (i = 0; i < index; i++) {
        sha = csi_sha_initialize(i, &sha_contex, sha_interrupt_handler);
        ASSERT_TRUE(sha != NULL);

        for (k = 0; k < sizeof(sha1_1088_cases) / (sizeof(sha_test_t)); k++) {
            ret = csi_sha_config(sha, SHA_MODE_1, sha1_1088_cases[k].endian);
            ASSERT_TRUE(ret == 0);
            ret = csi_sha_start(sha, NULL);
            ASSERT_TRUE(ret == 0);
            ret = csi_sha_update(sha, NULL, sha1_1088_cases[k].input, 136);
            ASSERT_TRUE(ret == 0);

#if CONFIG_SHA_BLOCK_MODE   /*it is also valid by get status*/
            sha_status_t status;

            while (1) {
                status = csi_sha_get_status(sha);

                if (status.busy == 0) {
                    break;
                }
            }

            ret = csi_sha_finish(sha, NULL, data);
            ASSERT_TRUE(ret == 0);
#else
            ret = csi_sha_finish(sha, NULL, data);
            ASSERT_TRUE(ret == 0);

            while (sha1_int_flag);

            sha1_int_flag = 1;

#endif

            for (j = 0; j < sizeof(data) / 4; j++) {
                if (data[j] != sha1_1088_cases[k].expect_out[j]) {
                    break;
                }
            }

            if (sha1_1088_cases[k].endian == SHA_ENDIAN_MODE_LITTLE) {
                idx = 0;
            } else {
                idx = 1;
            }

            if (j == sizeof(data) / 4) {
                printf("index %d sha1-1088 %s encode successfully\n", i, string[idx]);
            } else {
                printf("index %d sha1-1088 %s encode failed\n", i, string[idx]);
            }
        }
    }
}

/*****************************************************************************
test_sha224_1024: test the 1024 bit encode

INPUT: NULL

RETURN: NULL

*****************************************************************************/
void test_sha224_1024()
{
    uint32_t data[7];

    printf("- - -Testing sha-224 1024bit- - -\n");

    int i;
    int j;
    int k;
    uint32_t index;
    uint32_t ret = 0;
    uint32_t idx;
    char string[][10] = {"little", "big"};
    i = 7;

    index = 1;

    for (i = 0; i < index; i++) {
        sha = csi_sha_initialize(i, &sha_contex, sha_interrupt_handler);
        ASSERT_TRUE(sha != NULL);

        for (k = 0; k < sizeof(sha224_1024_cases) / (sizeof(sha_test_t)); k++) {
            ret = csi_sha_config(sha, SHA_MODE_224, sha224_1024_cases[k].endian);
            ASSERT_TRUE(ret == 0);
            ret = csi_sha_start(sha, NULL);
            ASSERT_TRUE(ret == 0);
            ret = csi_sha_update(sha, NULL, sha224_1024_cases[k].input, 128);
            ASSERT_TRUE(ret == 0);

#if CONFIG_SHA_BLOCK_MODE   /*it is also valid by get status*/
            sha_status_t status;

            while (1) {
                status = csi_sha_get_status(sha);

                if (status.busy == 0) {
                    break;
                }
            }

            ret = csi_sha_finish(sha, NULL, data);
            ASSERT_TRUE(ret == 0);
#else
            ret = csi_sha_finish(sha, NULL, data);
            ASSERT_TRUE(ret == 0);

            while (sha1_int_flag);

            sha1_int_flag = 1;
#endif

            for (j = 0; j < sizeof(data) / 4; j++) {
                if (data[j] != sha224_1024_cases[k].expect_out[j]) {
                    break;
                }
            }

            if (sha224_1024_cases[k].endian == SHA_ENDIAN_MODE_LITTLE) {
                idx = 0;
            } else {
                idx = 1;
            }

            if (j == sizeof(data) / 4) {
                printf("index %d sha224-1024 %s encode successfully\n", i, string[idx]);
            } else {
                printf("index %d sha224-1024 %s encode failed\n", i, string[idx]);
            }
        }
    }
}

/*****************************************************************************
test_sha256_1024: test the 1024 bit encode

INPUT: NULL

RETURN: NULL

*****************************************************************************/
void test_sha256_1024()
{
    uint32_t data[8];
    printf("- - -Testing sha-256 1024bit- - -\n");

    int i;
    int j;
    int k;
    uint32_t index;
    uint32_t ret = 0;
    uint32_t idx;
    char string[][10] = {"little", "big"};
    i = 8;

    index = 1;

    for (i = 0; i < index; i++) {
        sha = csi_sha_initialize(i, &sha_contex, sha_interrupt_handler);
        ASSERT_TRUE(sha != NULL);

        for (k = 0; k < sizeof(sha256_1024_cases) / (sizeof(sha_test_t)); k++) {
            ret = csi_sha_config(sha, SHA_MODE_256, sha256_1024_cases[k].endian);
            ASSERT_TRUE(ret == 0);
            ret = csi_sha_start(sha, NULL);
            ASSERT_TRUE(ret == 0);
            ret = csi_sha_update(sha, NULL, sha256_1024_cases[k].input, 128);
            ASSERT_TRUE(ret == 0);

#if CONFIG_SHA_BLOCK_MODE   /*it is also valid by get status*/
            sha_status_t status;

            while (1) {
                status = csi_sha_get_status(sha);

                if (status.busy == 0) {
                    break;
                }
            }

            ret = csi_sha_finish(sha, NULL, data);
            ASSERT_TRUE(ret == 0);
#else
            ret = csi_sha_finish(sha, NULL, data);
            ASSERT_TRUE(ret == 0);

            while (sha1_int_flag);

            sha1_int_flag = 1;
#endif

            for (j = 0; j < sizeof(data) / 4; j++) {
                if (data[j] != sha256_1024_cases[k].expect_out[j]) {
                    break;
                }
            }

            if (sha256_1024_cases[k].endian == SHA_ENDIAN_MODE_LITTLE) {
                idx = 0;
            } else {
                idx = 1;
            }

            if (j == sizeof(data) / 4) {
                printf("index %d sha256-1024 %s encode successfully\n", i, string[idx]);
            } else {
                printf("index %d sha256-1024 %s encode failed\n", i, string[idx]);
            }
        }
    }
}

/*****************************************************************************
test_sha384_1024: test the 1024 bit encode

INPUT: NULL

RETURN: NULL

*****************************************************************************/
void test_sha384_1024()
{
    uint32_t data[12];

    printf("- - -Testing sha-384 1024bit- - -\n");

    int i;
    int j;
    int k;
    uint32_t index;
    uint32_t ret = 0;
    uint32_t idx;
    char string[][10] = {"little", "big"};
    i = 12;

    index = 1;

    for (i = 0; i < index; i++) {
        sha = csi_sha_initialize(i, &sha_contex, sha_interrupt_handler);
        ASSERT_TRUE(sha != NULL);

        for (k = 0; k < sizeof(sha384_1024_cases) / (sizeof(sha_test_t)); k++) {
            ret = csi_sha_config(sha, SHA_MODE_384, sha384_1024_cases[k].endian);
            ASSERT_TRUE(ret == 0);
            ret = csi_sha_start(sha, NULL);
            ASSERT_TRUE(ret == 0);
            ret = csi_sha_update(sha, NULL, sha384_1024_cases[k].input, 128);
            ASSERT_TRUE(ret == 0);

#if CONFIG_SHA_BLOCK_MODE   /*it is also valid by get status*/
            sha_status_t status;

            while (1) {
                status = csi_sha_get_status(sha);

                if (status.busy == 0) {
                    break;
                }
            }

            ret = csi_sha_finish(sha, NULL, data);
            ASSERT_TRUE(ret == 0);
#else
            ret = csi_sha_finish(sha, NULL, data);
            ASSERT_TRUE(ret == 0);

            while (sha1_int_flag);

            sha1_int_flag = 1;
#endif

            for (j = 0; j < sizeof(data) / 4; j++) {
                if (data[j] != sha384_1024_cases[k].expect_out[j]) {
                    break;
                }
            }

            if (sha384_1024_cases[k].endian == SHA_ENDIAN_MODE_LITTLE) {
                idx = 0;
            } else {
                idx = 1;
            }

            if (j == sizeof(data) / 4) {
                printf("index %d sha384-1024 %s encode successfully\n", i, string[idx]);
            } else {
                printf("index %d sha384-1024 %s encode failed\n", i, string[idx]);
            }
        }
    }
}

/*****************************************************************************
test_sha512_1024: test the 1024 bit encode

INPUT: NULL

RETURN: NULL

*****************************************************************************/
void test_sha512_1024()
{
    uint32_t data[16];

    printf("- - -Testing sha-512 1024bit- - -\n");

    int i;
    int j;
    int k;
    uint32_t index;
    uint32_t ret = 0;
    uint32_t idx;
    char string[][10] = {"little", "big"};
    i = 16;

    index = 1;

    for (i = 0; i < index; i++) {
        sha = csi_sha_initialize(i, &sha_contex, sha_interrupt_handler);
        ASSERT_TRUE(sha != NULL);

        for (k = 0; k < sizeof(sha512_1024_cases) / (sizeof(sha_test_t)); k++) {
            ret = csi_sha_config(sha, SHA_MODE_512, sha512_1024_cases[k].endian);
            ASSERT_TRUE(ret == 0);
            ret = csi_sha_start(sha, NULL);
            ASSERT_TRUE(ret == 0);
            ret = csi_sha_update(sha, NULL, sha512_1024_cases[k].input, 128);
            ASSERT_TRUE(ret == 0);

#if CONFIG_SHA_BLOCK_MODE   /*it is also valid by get status*/
            sha_status_t status;

            while (1) {
                status = csi_sha_get_status(sha);

                if (status.busy == 0) {
                    break;
                }
            }

            ret = csi_sha_finish(sha, NULL, data);
            ASSERT_TRUE(ret == 0);
#else
            ret = csi_sha_finish(sha, NULL, data);
            ASSERT_TRUE(ret == 0);

            while (sha1_int_flag);

            sha1_int_flag = 1;
#endif

            for (j = 0; j < sizeof(data) / 4; j++) {
                if (data[j] != sha512_1024_cases[k].expect_out[j]) {
                    break;
                }
            }

            if (sha512_1024_cases[k].endian == SHA_ENDIAN_MODE_LITTLE) {
                idx = 0;
            } else {
                idx = 1;
            }

            if (j == sizeof(data) / 4) {
                printf("index %d sha521-1024 %s encode successfully\n", i, string[idx]);
            } else {
                printf("index %d sha512-1024 %s encode failed\n", i, string[idx]);
            }
        }
    }
}

/*****************************************************************************
test_sha: main function of the sha test

INPUT: NULL

RETURN: NULL

*****************************************************************************/
int test_sha()
{
    dtest_suite_info_t test_suite_info = {
        "test_sha", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
        { "test_sha512_1024", test_sha512_1024, SHA512_1024BIT_EN },
        { "test_sha384_1024", test_sha384_1024, SHA384_1024BIT_EN },
        { "test_sha256_1024", test_sha256_1024, SHA256_1024BIT_EN },
        { "test_sha224_1024", test_sha224_1024, SHA224_1024BIT_EN },
        { "test_sha1_1088", test_sha1_1088, SHA1_1088BIT_EN },
        { "test_sha1_1024", test_sha1_1024, SHA1_1024BIT_EN },
        { "test_sha1_576", test_sha1_576, SHA1_576BIT_EN },
        { "test_sha1_512", test_sha1_512, SHA1_512BIT_EN },
        { "test_sha1_8", test_sha1_8, SHA1_8BIT_EN },
        { "test_csi_sha_finish", test_csi_sha_finish, SHA_FINISH_INTERFACE_EN },
        { "test_csi_sha_update", test_csi_sha_update, SHA_UPDATE_INTERFACE_EN },
        { "test_drv_sha_start", test_drv_sha_start, SHA_START_INTERFACE_EN },
        { "test_csi_sha_config", test_csi_sha_config, SHA_CONFIG_INTERFACE_EN },
        { NULL, NULL, 0 }
    };

    dtest_add_cases(test_suite, test_case_info_array);

    return 0;
}

int main(void)
{
    dtest_register_cb(test_sha);
    dtest();

    return 0;
}
