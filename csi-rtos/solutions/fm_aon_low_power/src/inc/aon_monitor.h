#ifndef __AON_BOARD_H
#define __AON_BOARD_H


#define I8(X)       ((int8_t) (X))
#define I16(X)      ((int16_t) (X))
#define I32(X)      ((int32_t) (X))
#define I64(X)      ((int64_t) (X))
#define U8(X)       ((uint8_t) (X))
#define U16(X)      ((uint16_t) (X))
#define U32(X)      ((uint32_t) (X))
#define U64(X)      ((uint64_t) (X))

#define AON_FALSE        ((uint8_t) 0U)
#define AON_TRUE         ((uint8_t) 1U)

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define AON_BMU_TIMER_IDX       (3U)
typedef const char* const strings[];
void aon_dbg_monitor(void);


#endif
