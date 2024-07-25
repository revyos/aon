
#ifndef __AON_COMMON_H_
#define __AON_COMMON_H_

#include "aon_monitor.h"
#include "aon_misc.h"
#include "aon_wdt.h"
#include "aon_rpc.h"
#include "aon_pmic.h"
#include "aon_pvt.h"
#include "aon_sys.h"
#include "aon_lpm.h"

#define I8(X)       ((int8_t) (X))
#define I16(X)      ((int16_t) (X))
#define I32(X)      ((int32_t) (X))
#define I64(X)      ((int64_t) (X))
#define U8(X)       ((uint8_t) (X))
#define U16(X)      ((uint16_t) (X))
#define U32(X)      ((uint32_t) (X))
#define U64(X)      ((uint64_t) (X))

typedef enum {
    AON_ERR_NONE       =  0U,
    AON_ERR_VERSION    =  1U,
    AON_ERR_CONFIG     =  2U,
    AON_ERR_PARM       =  3U,
    AON_ERR_NOACCESS  =  4U,
    AON_ERR_LOCKED     =  5U,
    AON_ERR_UNAVAILABLE = 6U,
    AON_ERR_NOTFOUND    = 7U,
    AON_ERR_NOPOWER   =   8U,
    AON_ERR_IPC      =   9U,
    AON_ERR_BUSY     =     10U,
    AON_ERR_FAIL     =    11U,
    AON_ERR_LAST     =     12U,
    AON_ERR_LPM      =    13U,
} aon_err_en;


#ifdef AON_TRACE_ON
#define aon_print(fmt,args...) LOG_D(fmt,##args)
#else
#define aon_print(fmt,...)
#endif





#endif