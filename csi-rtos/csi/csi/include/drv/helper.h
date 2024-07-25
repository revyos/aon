
/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     drv/common.h
 * @brief    Header File for Common Driver
 * @version  V1.0
 * @date     31. March 2020
 * @model    common
 ******************************************************************************/

#ifndef _DRV_HELPER_H_
#define _DRV_HELPER_H_

#ifndef BIT
#define BIT(x)                   (1UL << (x))
#endif

#ifndef GENMASK
#define GENMASK(h, l)            (((~0UL) << (l)) & (~0UL >> (32 - 1 - (h))))
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x)            (sizeof(x) / sizeof((x)[0]))
#endif

#define DIV_ROUND_CLOSEST(x, divisor)(                \
{                                                     \
         typeof(divisor)__divisor = divisor;          \
         (((x)+ ((__divisor) / 2)) / (__divisor));    \
}                                                     \
)

#define __stringify_1(x...) #x
#define __stringify(x...) __stringify_1(x)

#define UNUSED(x) (void)(x)

#if !defined(MIN)
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#if !defined(MAX)
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif
#endif
