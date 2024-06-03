/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     sys_console.c
 * @brief    CSI Source File for console
 * @version  V1.0
 * @date     2019-12-19
 ******************************************************************************/
#ifndef __TRCCONFIG_H__
#define __TRCCONFIG_H__

/******************************************************************************
 * SELECTED_PORT
 *
 * Macro that specifies what hardware port that should be used. 
 * Available ports are:
 *
 * Port Name							Code
 * PORT_CSKY_ABIV1	   		1
 *****************************************************************************/

// Set the port setting here!
#define SELECTED_PORT PORT_CSKY_ABIV2

#if (SELECTED_PORT == PORT_NOT_SET)
	#error "You need to define SELECTED_PORT here!"
#endif

/*******************************************************************************
 * EVENT_BUFFER_SIZE
 *
 * Macro which should be defined as an integer value.
 *
 * This defines the capacity of the event buffer, i.e., the number of records
 * it may store. Most events use one record (4 byte), although some events
 * require multiple 4-byte records. You should adjust this to the amount of RAM
 * available in the target system.
 *
 * Default value is 1000, which means that 4000 bytes is allocated for the
 * event buffer.
 ******************************************************************************/
#define EVENT_BUFFER_SIZE 1000

/*******************************************************************************
 * SYMBOL_TABLE_SIZE
 *
 * Macro which should be defined as an integer value.
 *
 * This defines the capacity of the symbol table, in bytes. This symbol table
 * stores User Events labels and names of deleted tasks, queues, or other kernel
 * objects. If you don't use User Events or delete any kernel
 * objects you set this to a very low value. The minimum recommended value is 4.
 * A size of zero (0) is not allowed since a zero-sized array may result in a
 * 32-bit pointer, i.e., using 4 bytes rather than 0.
 *
 * Default value is 800.
 ******************************************************************************/
#define SYMBOL_TABLE_SIZE 800

#if (SYMBOL_TABLE_SIZE == 0)
#error "SYMBOL_TABLE_SIZE may not be zero!"
#endif

#define CONFIG_OS_TRACE_TRIGGER 1
#define CONFIG_TRACE_MAX_TRIGS 3

#endif

