/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     sys_console.c
 * @brief    CSI Source File for console
 * @version  V1.0
 * @date     2019-12-19
 ******************************************************************************/
#include "csi_config.h"
#include "trcConfig.h"
#include <stdint.h>
#include <trcUser.h>
#include <stdarg.h>
#include <stdio.h>

#include "trcUser.h"
#include "trcKernelPort.h"
#include "trcTrig.h"

typedef struct
{
    uint8_t event_data[EVENT_BUFFER_SIZE * sizeof(trc_event_t)];
    uint32_t event_data_used_size;
    uint8_t name_data[SYMBOL_TABLE_SIZE];
    uint32_t name_data_used_size;

    char error_msg[128];
    uint16_t name_count;
    uint8_t is_trace_start;     // 1 is start, 0 is stop
    uint8_t is_data_ready;
} trc_mgr_t;

typedef struct
{
    uint16_t event_count;
    uint16_t name_buffer_size;
    uint32_t timer_frequency;
} buffer_size_t;

static trc_mgr_t g_trc_mgr;

TRACE_STOP_HOOK trace_stopHookPtr = NULL;

static uint8_t _trace_store_name(const char *name);
static uint32_t _trace_get_dts();

static uint16_t _u16_to_little_endian(uint16_t dts);
static trc_event_t _trace_create_event(uint8_t type, const char *name);
static void _trace_add_event(void *event);
static uint32_t _u32_to_little_endian(uint32_t i);
static void _trace_recorder_memory_event(uint8_t type, const char *name, void *addr, uint32_t size);
static void _trace_store_rest_dts(uint32_t dts, uint16_t *pRestDts);
static uint8_t _trace_find_name(const char *name);

static uint16_t _trace_calculate_event_size(uint8_t type, uint32_t dts);
#define CHECK_TRACE_RUNNING()     \
    {if (!g_trc_mgr.is_trace_start) return;}
void trace_init_data(void)
{
    TRACE_SR_ALLOC_CRITICAL_SECTION();
    trace_clear();

    TRACE_ENTER_CRITICAL_SECTION();

    g_trc_mgr.is_trace_start = 0;
    g_trc_mgr.error_msg[0] = 0;
    TRACE_EXIT_CRITICAL_SECTION();
}

void trace_clear(void)
{
    TRACE_SR_ALLOC_CRITICAL_SECTION();
    TRACE_ENTER_CRITICAL_SECTION();

    g_trc_mgr.event_data_used_size = 0;
    g_trc_mgr.name_data_used_size = 0;
    g_trc_mgr.name_count = 0;
    g_trc_mgr.is_data_ready = 0;
    _trace_store_name("NULL");
    TRACE_EXIT_CRITICAL_SECTION();
}

void trace_start(void)
{
    if (g_trc_mgr.is_trace_start == 1) {
        return;
    }
    TRACE_SR_ALLOC_CRITICAL_SECTION();
    TRACE_ENTER_CRITICAL_SECTION();

    g_trc_mgr.is_trace_start = 1;
    trace_clear();
    // clear the trace tick count
    {
        trace_time_init();
        trace_get_time_dts(0);
    }

    TRACE_EXIT_CRITICAL_SECTION();
}
void trace_stop(void)
{
    TRACE_SR_ALLOC_CRITICAL_SECTION();

    TRACE_ENTER_CRITICAL_SECTION();

    g_trc_mgr.is_data_ready = 1;
    if (trace_stopHookPtr && g_trc_mgr.is_trace_start == 1) {
        g_trc_mgr.is_trace_start = 0;
        trace_stopHookPtr();
    }

    TRACE_EXIT_CRITICAL_SECTION();

}
char *trace_get_last_error(void)
{
    return g_trc_mgr.error_msg;
}
void trace_clear_error()
{
    g_trc_mgr.error_msg[0] = 0;
}
void *trace_get_event_buffer(void)
{
    return g_trc_mgr.event_data;
}
uint32_t trace_get_event_buffer_size(void)
{
    return g_trc_mgr.event_data_used_size;
}
void *trace_get_name_buffer(void)
{
    return g_trc_mgr.name_data;
}
uint32_t trace_get_name_buffer_size(void)
{
    return g_trc_mgr.name_data_used_size;
}

void trace_recorder_event(uint8_t type, const char *name)
{
	//printf("trace_recorder_event!\n");


    CHECK_TRACE_RUNNING();

    TRACE_SR_ALLOC_CRITICAL_SECTION();
    TRACE_ENTER_CRITICAL_SECTION();

    static uint32_t dts ;
    static uint16_t event_size;
    static uint16_t restDts = 0;
    static trc_event_t event;

    dts = _trace_get_dts();
    event_size = _trace_calculate_event_size(type, dts);
    if (g_trc_mgr.event_data_used_size + event_size >= EVENT_BUFFER_SIZE * sizeof(trc_event_t)) {
        trace_stop();
        return;
    }

    restDts = 0;
    _trace_store_rest_dts(dts, &restDts);
    restDts = _u16_to_little_endian(restDts);

    event = _trace_create_event(type, name);
    event.dts = restDts;

    _trace_add_event(&event);

    TRACE_EXIT_CRITICAL_SECTION();
}
void trace_recorder_malloc(void *addr, uint32_t size)
{
    CHECK_TRACE_RUNNING();

    TRACE_SR_ALLOC_CRITICAL_SECTION();
    TRACE_ENTER_CRITICAL_SECTION();

    _trace_recorder_memory_event(TRACE_TYPE_MEM_MALLOC, "memory alloc", addr, size);

    TRACE_EXIT_CRITICAL_SECTION();
}
void trace_recorder_malloc_failed(uint32_t size)
{
    CHECK_TRACE_RUNNING();

    TRACE_SR_ALLOC_CRITICAL_SECTION();
    TRACE_ENTER_CRITICAL_SECTION();

    _trace_recorder_memory_event(TRACE_TYPE_MEM_MALLOC_FAILED, "memory alloc failed", NULL, size);

    TRACE_EXIT_CRITICAL_SECTION();
}
void trace_recorder_mfree(void *addr, uint32_t size)
{
    CHECK_TRACE_RUNNING();

    TRACE_SR_ALLOC_CRITICAL_SECTION();
    TRACE_ENTER_CRITICAL_SECTION();

    _trace_recorder_memory_event(TRACE_TYPE_MEM_FREE, "memory free", addr, size);

    TRACE_EXIT_CRITICAL_SECTION();
}
void trace_recorder_mfree_failed(void *addr)
{
    CHECK_TRACE_RUNNING();

    TRACE_SR_ALLOC_CRITICAL_SECTION();
    TRACE_ENTER_CRITICAL_SECTION();

    _trace_recorder_memory_event(TRACE_TYPE_MEM_FREE_FAILED, "memory free failed", addr, 0);

    TRACE_EXIT_CRITICAL_SECTION();
}
void trace_set_stop_hook(TRACE_STOP_HOOK stopHookFunction)
{
    trace_stopHookPtr = stopHookFunction;
}
uint8_t trace_is_running(void)
{
    return g_trc_mgr.is_trace_start;
}
uint8_t trace_data_is_ready()
{
    return g_trc_mgr.is_data_ready;
}
void *trace_get_buffer_info()
{
    static buffer_size_t bufferSize;
    bufferSize.event_count = _u16_to_little_endian((uint16_t) (trace_get_event_buffer_size() / sizeof(trc_event_t)));
    bufferSize.name_buffer_size = _u16_to_little_endian((uint16_t) trace_get_name_buffer_size());
    bufferSize.timer_frequency = _u32_to_little_endian((uint32_t) trace_get_timer_freq());
    return &bufferSize;
}
uint16_t trace_get_buffer_info_size()
{
    return sizeof(buffer_size_t);
}
void trace_printf(const char *formatStr, ...)
{
#if (ENABLE_DEBUG_PRINT == 1)
    va_list vl;
    va_start(vl, formatStr);
    printf(formatStr, vl);
    va_end(vl);
#endif
}
void _trace_recorder_memory_event(uint8_t type, const char *name, void *addr, uint32_t size)
{
	//printf("trace_recorder_memory_even!\n");
    uint32_t dts = _trace_get_dts();
    uint16_t event_size = _trace_calculate_event_size(type, dts);
    if (g_trc_mgr.event_data_used_size + event_size >= EVENT_BUFFER_SIZE * sizeof(trc_event_t)) {
        trace_stop();
        return;
    }

    uint16_t restDts = 0;
    _trace_store_rest_dts(dts, &restDts);
    restDts = _u16_to_little_endian(restDts);

    trc_event_t event = _trace_create_event(type, name);
    event.dts = restDts;

    _trace_add_event(&event);

    uint32_t base_addr = _u32_to_little_endian((uint32_t) addr);
    size = _u32_to_little_endian(size);

    _trace_add_event(&base_addr);
    _trace_add_event(&size);
}
void _trace_add_event(void *event)
{
    if (g_trc_mgr.event_data_used_size >= (uint32_t) (EVENT_BUFFER_SIZE * sizeof(trc_event_t))) {
        return;
    }
    memcpy(&(g_trc_mgr.event_data[0]) + g_trc_mgr.event_data_used_size, event, sizeof(trc_event_t));
    g_trc_mgr.event_data_used_size += sizeof(trc_event_t);
}
trc_event_t _trace_create_event(uint8_t type, const char *name)
{
    static trc_event_t trc_event;

    trc_event.type = type;
    trc_event.obj_handle = _trace_store_name(name);
    trc_event.dts = 0;

    return trc_event;
}

#define DTS_MAX_SIZE    0xffffu
void _trace_store_rest_dts(uint32_t dts, uint16_t *pRestDts)
{
    if (dts <= DTS_MAX_SIZE) {
        *pRestDts = (uint16_t) dts;
        return;
    }
    *pRestDts = (uint16_t) (dts % 0xffffu);
    if (*pRestDts == 0) {
        *pRestDts = DTS_MAX_SIZE;
    }

    dts -= *pRestDts;
    trc_event_t trc_event;
    trc_event.type = TRACE_TYPE_EVT_DIV;
    trc_event.obj_handle = 0;
    trc_event.dts = 0;
    _trace_add_event(&trc_event);
    _trace_add_event(&dts);
}

uint16_t _trace_calculate_event_size(uint8_t type, uint32_t dts)
{
    uint16_t size = 1;
    if (dts > DTS_MAX_SIZE) {
        size += 2;
    }

    switch (type) {
    case TRACE_TYPE_MEM_MALLOC:
    case TRACE_TYPE_MEM_MALLOC_FAILED:
    case TRACE_TYPE_MEM_FREE:
    case TRACE_TYPE_MEM_FREE_FAILED:
        size += 2;
        break;
    default:break;
    }

    return size * sizeof(trc_event_t);
}

union
{
    char c[4];
    unsigned mylong;
} endian_test = {{'l', '?', '?', 'b'}};

#define ENDIAN_IS_LITTLE     ((char)endian_test.mylong == 'l')
uint16_t _u16_to_little_endian(uint16_t dts)
{
    if (ENDIAN_IS_LITTLE) {
        return dts;
    }
    return (uint16_t) ((dts & 0xffu) << 8 | (dts & 0xff00u) >> 8);
}
uint32_t _u32_to_little_endian(uint32_t value)
{
    if (ENDIAN_IS_LITTLE) {
        return value;
    }
    return (value & 0x000000FFU) << 24 | (value & 0x0000FF00U) << 8 | (value & 0x00FF0000U) >> 8
        | (value & 0xFF000000U) >> 24;
}
#define NAME_NOT_FOUND  0xffu
static uint8_t _trace_store_name(const char *name)
{
    static uint8_t index;
    static size_t len;
    if (name == NULL) {
        name = "NULL";
    }
    index = _trace_find_name(name);
    if (index != NAME_NOT_FOUND) {
        return index;
    }
    len = strlen(name) + 1;
    if (g_trc_mgr.name_data_used_size + len > SYMBOL_TABLE_SIZE) {
        return _trace_find_name("NULL");
    }
    strcpy((char *) (g_trc_mgr.name_data + g_trc_mgr.name_data_used_size), name);
    g_trc_mgr.name_data_used_size += len;
    g_trc_mgr.name_count++;
    return (uint8_t) (g_trc_mgr.name_count - 1);
}
static uint8_t _trace_find_name(const char *name)
{
    static uint8_t index;
    index = NAME_NOT_FOUND;
    const char *pData = (const char *) g_trc_mgr.name_data;
    for (size_t i = 0; i < g_trc_mgr.name_count; ++i) {
        if (strcmp(name, pData) == 0) {
            index = (uint8_t) i;
            break;
        }
        for (; *pData != '\0'; pData++);
        pData++;
    }

    return index;
}

uint32_t _trace_get_dts()
{
    static uint32_t dts;
    dts = 0;
    trace_get_time_dts(&dts);
    return dts;
}

