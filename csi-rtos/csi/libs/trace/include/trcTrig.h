/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     sys_console.c
 * @brief    CSI Source File for console
 * @version  V1.0
 * @date     2019-12-19
 ******************************************************************************/
#ifndef __TRCTRIG_H__
#define __TRCTRIG_H__

#include <stdint.h>
#include <trcConfig.h>

#ifndef CONFIG_TRACE_MAX_TRIGS
#define CONFIG_TRACE_MAX_TRIGS 3
#endif

struct trig
{
  uint8_t id;                /* Trigger ID.  */
  uint8_t enable;             /* Flag set by PC to enable/disable trigger.  */
};

struct trig_manager
{
  struct trig trigs[CONFIG_TRACE_MAX_TRIGS];   /* All triggers infomations.  */
  uint8_t cur_trig;                            /* Current trigger index.  */
};


#if CONFIG_OS_TRACE_TRIGGER > 0
void trig_init(void);
void trig_start(uint8_t id);
void trig_set_enable(uint8_t id, uint8_t enable);
struct trig* trig_get_trigs(void);
size_t trig_count(void);
#else
#define trig_init()
#define trig_start(id)
#define trig_set_enable(id, enable)
#define trig_disable_all(id)
#define trig_get_trigs()    NULL
#define trig_count()    0
#endif

#endif /* __TRCTRIG_H__ */
