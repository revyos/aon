/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

#include <stdio.h>
#include "test_device.h"
#include "dtest.h"

#define __DEBUG 0

#if (__DCACHE_PRESENT == 1U)
#define MAXSIZE 1<<14
volatile char m[MAXSIZE];

#ifndef DCACHE_SIZE
#define DCACHE_SIZE 16*1024
#endif

#ifndef DCACHE_LINE_SIZE
#define DCACHE_LINE_SIZE 16
#endif

#if __CK80X >= 3
static void
test_dcache_operation(void)
{
    long *lp = (long *)m;
    int i, errs;
    printf("Data cache basic\n");
    csi_dcache_clean();
    csi_dcache_disable();
    csi_dcache_clean();

    // Fill test buffer
    for (i = 0;  i < sizeof(m) / sizeof(*lp);  i++) {
        lp[i] = i;
    }

    csi_dcache_invalid();
    csi_dcache_enable();

    // Now push data through the cache
    // Note: 256 seems like a reasonable offset.  It may be useful to actually
    // compute this (and the size of the test region) based on cache geometry
    for (i = 256;  i < 256 + DCACHE_SIZE / sizeof(*lp);  i++) {
        lp[i] = 0xFF000000 + i;
    }

    // Now force cache clean and off
    csi_dcache_clean();
    csi_dcache_disable();
    // Verify the data
    printf("Verify data with cache off\n");
    errs = 0;

    for (i = 0;  i < sizeof(m) / sizeof(*lp);  i++) {
        if ((i >= 256) && (i < 256 + DCACHE_SIZE / sizeof(*lp))) {
            if (lp[i] != (0xFF000000 + i)) {
                if (++errs < 16) {
                    printf("Data inside test range changed - was: %x, is %lx, index: %x\n",
                           0xFF000000 + i, lp[i], i);
                }
            }
        } else {
            if (lp[i] != i) {
                if (++errs < 16) {
                    printf("Data outside test range changed - was: %x, is %lx, index: %x\n",
                           i, lp[i], i);
                }
            }
        }
    }

    ASSERT_TRUE(0 == errs);
#if (__DEBUG)  // Additional information

    if (0 != errs) {
        printf("dcache basic failed")
    }

#endif
}
#endif


#if __CK80X >= 3
static void test_dsync(void)
{
    volatile uint8_t *aligned_p;
    int32_t i;

    printf("Data cache sync all\n");

    for (i = 0; i < DCACHE_LINE_SIZE * 16; i++) {
        m[i] = 0;
    }

    csi_dcache_clean();
    csi_dcache_disable();
    csi_dcache_clean();
    csi_dcache_invalid();
    csi_dcache_enable();

    aligned_p = (volatile uint8_t *)
                (((unsigned long) &m[DCACHE_LINE_SIZE * 2])
                 & ~(DCACHE_LINE_SIZE - 1));

    aligned_p[0] = 42 + aligned_p[1]; // Load causes cache to be used!
    aligned_p[DCACHE_LINE_SIZE] = 43 + aligned_p[DCACHE_LINE_SIZE + 1];

    csi_dcache_clean();

    ASSERT_TRUE(42 == aligned_p[0]);
    ASSERT_TRUE(43 == aligned_p[DCACHE_LINE_SIZE]);

    csi_dcache_invalid();

    ASSERT_TRUE(42 == aligned_p[0]);
    ASSERT_TRUE(43 == aligned_p[DCACHE_LINE_SIZE]);


    csi_icache_invalid();
    csi_dcache_disable();

    ASSERT_TRUE(42 == aligned_p[0]);
    ASSERT_TRUE(43 == aligned_p[DCACHE_LINE_SIZE]);
    csi_dcache_enable();
}
#endif

// -------------------------------------------------------------------------
// Test of data cache line flush.
//   Requires write-back cache.
//   Check that flushed data is written to memory.
//   Simple range check of macro.
// only if we know this, can we test:
#if __CK80X >= 3
static void test_dflush(void)
{
    volatile uint8_t *aligned_p;
    int32_t i;

    printf("Data cache flush region");

    for (i = 0; i < DCACHE_LINE_SIZE * 16; i++) {
        m[i] = 0;
    }

    csi_dcache_clean();
    csi_dcache_disable();
    csi_dcache_clean();
    csi_dcache_invalid();
    csi_dcache_enable();

    aligned_p = (volatile uint8_t *)
                (((unsigned long) &m[DCACHE_LINE_SIZE * 2])
                 & ~(DCACHE_LINE_SIZE - 1));


    aligned_p[0] = 42 + aligned_p[1]; // Load causes cache to be used!
    aligned_p[DCACHE_LINE_SIZE] = 43 + aligned_p[DCACHE_LINE_SIZE + 1];

    csi_dcache_clean_range((uint32_t *)aligned_p, DCACHE_LINE_SIZE);

    csi_dcache_disable();


    ASSERT_TRUE(42 == aligned_p[0]);
    ASSERT_TRUE(0 == aligned_p[DCACHE_LINE_SIZE]);

    csi_dcache_enable();
}
#endif

// -------------------------------------------------------------------------
// Test of data cache disable (which does NOT force contents out to RAM)
//   Requires write-back cache [so NOT invoked unconditionally]
//   Check that dirty data is not written to memory and is invalidated
//    in the cache.
//   Simple invocation check of macro.
// only if we know this, can we test:

#if __CK80X >= 3
static void test_ddisable(void)
{
    volatile uint8_t *aligned_p;
    int32_t i;

    printf("Data cache gross disable\n");

    csi_dcache_disable();

    for (i = 0; i < DCACHE_LINE_SIZE * 16; i++) {
        m[i] = 0;
    }

    csi_dcache_clean();
    csi_dcache_disable();
    csi_dcache_clean();
    csi_dcache_invalid();
    csi_dcache_enable();

    aligned_p = (volatile uint8_t *)
                (((unsigned long) &m[DCACHE_LINE_SIZE * 2])
                 & ~(DCACHE_LINE_SIZE - 1));


    aligned_p[0] = 43 + aligned_p[1]; // Load causes cache to be used!
    aligned_p[DCACHE_LINE_SIZE - 1] = 43;

    aligned_p[DCACHE_LINE_SIZE] = 42 + aligned_p[DCACHE_LINE_SIZE + 1];

    csi_dcache_disable();


    ASSERT_TRUE(0 == aligned_p[0] &&
                0 == aligned_p[DCACHE_LINE_SIZE - 1]);
    ASSERT_TRUE(0 == aligned_p[DCACHE_LINE_SIZE]);

    csi_dcache_enable();
}

#endif
// -------------------------------------------------------------------------
// Test of data cache total invalidate.
//   Requires write-back cache.
//   Check that invalidated data is not written to memory and is invalidated
//    in the cache.
//   Simple invocation check of macro.
// only if we know this, can we test:
void test_dinvalidate_all(void)
{
    volatile uint8_t *aligned_p;
    int32_t i;

    printf("Data cache invalidate all\n");

    for (i = 0; i < DCACHE_LINE_SIZE * 16; i++) {
        m[i] = 0;
    }

    csi_dcache_clean();
    csi_dcache_disable();
    csi_dcache_clean();
    csi_dcache_invalid();
    csi_dcache_enable();

    aligned_p = (volatile uint8_t *)
                (((unsigned long) &m[DCACHE_LINE_SIZE * 2])
                 & ~(DCACHE_LINE_SIZE - 1));

    aligned_p[0] = 43 + aligned_p[1]; // Load causes cache to be used!
    aligned_p[DCACHE_LINE_SIZE - 1] = 43;

    aligned_p[DCACHE_LINE_SIZE] = 42 + aligned_p[DCACHE_LINE_SIZE + 1];

    csi_dcache_invalid();

    ASSERT_TRUE(0 == aligned_p[0] && 0 == aligned_p[DCACHE_LINE_SIZE - 1]);
    ASSERT_TRUE(0 == aligned_p[DCACHE_LINE_SIZE]);
}
// -------------------------------------------------------------------------
// Test of data cache line invalidate.
//   Requires write-back cache.
//   Check that invalidated data is not written to memory and is invalidated
//    in the cache.
//   Simple range check of macro.
// only if we know this, can we test:
#if __CK80X >= 3
static void test_dinvalidate(void)
{
    volatile uint8_t *aligned_p;
    int32_t i;

    printf("Data cache invalidate region\n");

    for (i = 0; i < DCACHE_LINE_SIZE * 16; i++) {
        m[i] = 0;
    }

    csi_dcache_clean();
    csi_dcache_disable();
    csi_dcache_clean();
    csi_dcache_invalid();
    csi_dcache_enable();

    aligned_p = (volatile uint8_t *)
                (((unsigned long) &m[DCACHE_LINE_SIZE * 2])
                 & ~(DCACHE_LINE_SIZE - 1));


    aligned_p[0] = 43 + aligned_p[1]; // Load causes cache to be used!
    aligned_p[DCACHE_LINE_SIZE - 1] = 43;

    aligned_p[DCACHE_LINE_SIZE] = 42 + aligned_p[DCACHE_LINE_SIZE + 1];

    csi_dcache_invalid_range((uint32_t *)aligned_p, DCACHE_LINE_SIZE);


    ASSERT_TRUE(0 == aligned_p[0] &&
                0 == aligned_p[DCACHE_LINE_SIZE - 1]);
    ASSERT_TRUE(42 == aligned_p[DCACHE_LINE_SIZE]);

    csi_dcache_clean();
    csi_dcache_disable();

    ASSERT_TRUE(0 == aligned_p[0] &&
                0 == aligned_p[DCACHE_LINE_SIZE - 1]);
    ASSERT_TRUE(42 == aligned_p[DCACHE_LINE_SIZE]);

    csi_dcache_enable();
}
#endif
#endif

#if (__ICACHE_PRESENT == 1U)


#endif


int test_dcache_basic(void)
{
#if __CK80X >= 3
    printf("Testing cache api\n");
    csi_cache_set_range(0, 0x20000000, 0x13, 1);
#if (__DCACHE_PRESENT == 1U)
    test_dcache_operation();
    test_dsync();
    test_dflush();
    test_ddisable();
    test_dinvalidate_all();
    test_dinvalidate();
#endif

#if (__ICACHE_PRESENT == 1U)

#endif
    return 0;
#else
    printf("Unsupport Testing cache api\n");
    return 0;
#endif
}

int main(void)
{
    test_dcache_basic();
}
