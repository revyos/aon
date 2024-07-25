/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     w25q64fv.h
 * @brief    header file for w25q64fv driver
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#ifndef __W25Q64FV_H_
#define __W25Q64FV_H_

#define W25Q64FV_PAGE_SIZE 0x100

#define W25Q64FV_ADDR_START   0x0
#define W25Q64FV_ADDR_END     0x10000
#define W25Q64FV_CLK_RATE     2000000        /* 2MHZ */

#define CONFIG_CHIP_ZX297100

#define IS_FLASH_ADDR(addr) \
    ((addr >= W25Q64FV_ADDR_START) && (addr < W25Q64FV_ADDR_END))

#endif /* __W25Q64FV_H_ */
