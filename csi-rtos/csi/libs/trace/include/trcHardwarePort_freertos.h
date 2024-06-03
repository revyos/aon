/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     sys_console.c
 * @brief    CSI Source File for console
 * @version  V1.0
 * @date     2019-12-19
 ******************************************************************************/

/*******************************************************************************
 * IRQ_PRIORITY_ORDER
 *
 * Macro which should be defined as an integer of 0 or 1.
 *
 * This should be 0 if lower IRQ priority values implies higher priority
 * levels, such as on ARM Cortex M. If the opposite scheme is used, i.e.,
 * if higher IRQ priority values means higher priority, this should be 1.
 *
 * This setting is not critical. It is used only to sort and colorize the
 * interrupts in priority order, in case you record interrupts using
 * the trace_store_isr_begin and trace_store_isr_end routines.
 *
 ******************************************************************************
 *
 * HWTC Macros
 *
 * These four HWTC macros provides a hardware isolation layer representing a
 * generic hardware timer/counter used for driving the operating system tick
 *
 * HWTC_COUNT: The current value of the counter. This is expected to be reset
 * a each tick interrupt. Thus, when the tick handler starts, the counter has
 * already wrapped.  *
 *
 * HWTC_PERIOD: The number of increments or decrements of HWTC_COUNT between
 * two OS tick interrupts. This should preferably be mapped to the reload
 * register of the hardware timer, to make it more portable between chips in the
 * same family. The macro should in most cases be (reload register + 1).
 * For FreeRTOS, this can in most cases be defined as
 * #define HWTC_PERIOD (configCPU_CLOCK_HZ / configTICK_RATE_HZ)
 *
 * The HWTC macros and trace_port_get_time_stamp is the main porting issue
 * or the trace recorder library. Typically you should not need to change
 * the code of trace_port_get_time_stamp if using the HWTC macros.
 *
 ******************************************************************************/
#include <stdint.h>
extern uint32_t csi_coret_get_value(void);

#if (SELECTED_PORT == PORT_CSKY_ABIV1)
    #define HWTC_PERIOD                         (configCPU_CLOCK_HZ / configTICK_RATE_HZ)
    #define HWTC_COUNT                          (HWTC_PERIOD - csi_coret_get_value())
    #define HWTC_COUNT_FREQ                     SYSTEM_CLOCK

#elif (SELECTED_PORT == PORT_CSKY_ABIV2)
    #define HWTC_PERIOD                         (configCPU_CLOCK_HZ / configTICK_RATE_HZ)
    #define HWTC_COUNT                          (HWTC_PERIOD - csi_coret_get_value())
    #define HWTC_COUNT_FREQ                     SYSTEM_CLOCK

#elif (SELECTED_PORT == PORT_APPLICATION_DEFINED)

	#if !( defined (HWTC_COUNT_DIRECTION) && defined (HWTC_COUNT) && defined (HWTC_PERIOD) && defined (HWTC_DIVISOR) && defined (IRQ_PRIORITY_ORDER) )
		#error SELECTED_PORT is PORT_APPLICATION_DEFINED but not all of the necessary constants have been defined.
	#endif

#elif (SELECTED_PORT != PORT_NOT_SET)

	#error "SELECTED_PORT had unsupported value!"
	#define SELECTED_PORT PORT_NOT_SET

#endif


