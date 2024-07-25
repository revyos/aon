/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     sys_console.c
 * @brief    CSI Source File for console
 * @version  V1.0
 * @date     2019-12-19
 ******************************************************************************/
#ifndef __TRCKERNELHOOKS_H__
#define __TRCKERNELHOOKS_H__

#if (USE_TRACEALYZER_RECORDER == 1)

#undef trcKERNEL_HOOKS_INCREMENT_TICK
#define trcKERNEL_HOOKS_INCREMENT_TICK() \
	{ extern uint32_t uiTraceTickCount; uiTraceTickCount++; }

#endif

#endif /* TRCKERNELHOOKS_H */

