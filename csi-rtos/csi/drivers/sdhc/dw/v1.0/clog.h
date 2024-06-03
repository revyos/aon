#ifndef _CLOG_H_
#define _CLOG_H_

#include "stdio.h"

#define CLOG_ON 0

#define CLOG_NONE   0
#define CLOG_ERR    1
#define CLOG_INFO   1
#define CLOG_WARN   2
#define CLOG_LEVEL3     3
#define CLOG_LEVEL4     4
#define CLOG_LEVEL5     5
#define CLOG_LEVEL6     6
#define CLOG_LEVEL7     7
#define CLOG_LEVEL8     8
#define CLOG_LEVEL9     9
#define CLOG_LEVEL10    10
#define CLOG_LEVEL11    11
#define CLOG_LEVEL12    12

#define CLOG_LEVEL      2

#if CLOG_ON
#define clog_print(flags, fmt, arg...) \
    do {                            \
        if (flags <= CLOG_LEVEL)    \
            printf(fmt, ##arg);     \
} while (0)
#else
#define clog_print(flags, fmt, arg...)
#endif

#endif
