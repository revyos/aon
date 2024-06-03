# CRC

## 设备说明

​		CRC即循环冗余校验码（Cyclic Redundancy Check）：数据通信领域中最常用的一种差错校验码，其信息字段和校验字段长度可以任意指定，但要求通信双方定义的CRC标准一致 。

​		CRC原理：在K位信息码（目标发送数据）后再拼接R位校验码，使整个编码长度为N位，因此这种编码也叫（N,K）码。通俗的说，就是在需要发送的信息后面附加一个数（即校验码），生成一个新的发送数据发送给接收端。这个数据要求能够使生成的新数据被一个特定的数整除。 		

## 接口列表

SHA的CSI接口如下所示：

| 函数            | 说明                               |
| --------------- | ---------------------------------- |
| csi_crc7_be     | 计算CRC7值                         |
| csi_crc8        | 计算CRC8值                         |
| csi_crc8_maxim  | 计算CRC8 MAXIM值                   |
| csi_crc16       | 计算CRC16值                        |
| csi_crc16_ccitt | 计算CRC16 CCITT值                  |
| csi_crc16_dnp   | 计算CRC16 DNP值                    |
| csi_crc16_itu   | 计算CRC16 ITU值                    |
| csi_crc32_le    | 计算CRC32值，多项式值为 0xEDB88320 |
| csi_crc32_be    | 计算CRC32值，多项式值为 0x04C11DB7 |


## 接口详细说明



### csi_crc7_be

```c
uint8_t csi_crc7_be(uint8_t crc, uint8_t* data, uint32_t size)
```

* 功能描述:

  * 计算CRC7值。

* 参数:

  * `crc`: 初始值。
  * data: 待计算数据地址。
  * size: 待计算数长度。

* 返回值:

  * CRC7值。

### csi_crc8

```c
uint8_t csi_crc8(uint8_t crc, uint8_t* data, uint32_t size)
```

* 功能描述:

  * 计算CRC8值。

* 参数:

  * `crc`: 初始值。
  * data: 待计算数据地址。
  * size: 待计算数长度。

* 返回值:

  * CRC8值。



### csi_crc8_maxim

```c
uint8_t csi_crc8_maxim(uint8_t crc, uint8_t* data, uint32_t size)
```

* 功能描述:

  * 计算CRC8 MAXIM值。

* 参数:

  * `crc`: 初始值。
  * data: 待计算数据地址。
  * size: 待计算数长度。

* 返回值:

  * CRC8 MAXIM值。



### csi_crc16

```c
uint16_t csi_crc16(uint16_t crc, uint8_t* data, uint32_t size)
```

* 功能描述:

  * 计算CRC16值。

* 参数:

  * `crc`: 初始值。
  * data: 待计算数据地址。
  * size: 待计算数长度。

* 返回值:

  * CRC16值。

  

### csi_crc16_ccitt

```c
uint16_t csi_crc16_ccitt(uint16_t crc, uint8_t* data, uint32_t size)
```

* 功能描述:

  * 计算CRC16 CCITT值。

* 参数:

  * `crc`: 初始值。
  * data: 待计算数据地址。
  * size: 待计算数长度。

* 返回值:

  * CRC16 CCITT值。

  

### csi_crc16_dnp

```c
uint16_t csi_crc16_dnp(uint16_t crc, uint8_t* data, uint32_t size)
```

* 功能描述:

  * 计算CRC16 DNP值。

* 参数:

  * `crc`: 初始值。
  * data: 待计算数据地址。
  * size: 待计算数长度。

* 返回值:

  * CRC16 DNP值。

  

### csi_crc16_itu

```c
uint16_t csi_crc16_itu(uint16_t crc, uint8_t* data, uint32_t size)
```

* 功能描述:

  * 计算CRC16 ITU值。

* 参数:

  * `crc`: 初始值。
  * data: 待计算数据地址。
  * size: 待计算数长度。

* 返回值:

  * CRC16 ITU值。



### csi_crc32_le

```c
uint32_t csi_crc32_le(uint32_t crc, uint8_t* data, uint32_t size)
```

* 功能描述:

  * 计算CRC32 值，多项式值为 0xEDB88320。

* 参数:

  * `crc`: 初始值。
  * data: 待计算数据地址。
  * size: 待计算数长度。

* 返回值:

  * CRC32值。

  

### csi_crc32_be

```c
uint32_t csi_crc32_be(uint32_t crc, uint8_t* data, uint32_t size)
```

* 功能描述:

  * 计算CRC32 值，多项式值为 0x04C11DB7。

* 参数:

  * `crc`: 初始值。
  * data: 待计算数据地址。
  * size: 待计算数长度。

* 返回值:

  * CRC32值。





## 使用示例

### 各接口使用实例

```c
/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     example_crc.c
 * @brief    the main function for the CRC driver
 * @version  V1.0
 * @date     04. Mar 2020
 ******************************************************************************/
#include <stdio.h>
#include "soc.h"
#include "drv/crc.h"

void crc_modbus()
{
    uint8_t crc_input[] = {0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34, 0x56};
    uint32_t crc_input1[] = {0x44332211, 0x44332211, 0x44332211, 0x44332211};
    uint32_t expect_out[] = {0x10, 0x76, 0x98, 0x7e44, 0x8efa, 0x2598, 0xa900, 0xe3c8, 0x0, 0x39C45259};
    uint32_t out;
    out = csi_crc7_be(0, (uint8_t *)crc_input, 5);

    if (out == expect_out[0]) {
        printf("test csi_crc7_be successfully\n");
    } else {
        printf("test csi_crc7_be faild %x\n", out);
    }

    out = csi_crc8(0xff, (uint8_t *)crc_input, 4);

    if (out == expect_out[1]) {
        printf("test csi_crc8 successfully\n");
    } else {
        printf("test csi_crc8 faild %x\n", out);
    }


    out = csi_crc8_maxim(0, (uint8_t *)crc_input, 4);

    if (out == expect_out[2]) {
        printf("test csi_crc8_maxim successfully\n");
    } else {
        printf("test csi_crc8_maxim faild %x\n", out);
    }

    out = csi_crc16(0, (uint8_t *)crc_input1, 16);

    if (out == expect_out[3]) {
        printf("test csi_crc16/0 successfully\n");
    } else {
        printf("test csi_crc16/0 faild %x\n", out);
    }

    out = csi_crc16(0xffff, (uint8_t *)crc_input1, 16);

    if (out == expect_out[4]) {
        printf("test csi_crc16/0xffff successfully\n");
    } else {
        printf("test csi_crc16/0xffff  faild %x\n", out);
    }

    out = csi_crc16_ccitt(0, (uint8_t *)crc_input1, 16);

    if (out == expect_out[5]) {
        printf("test csi_crc16_ccitt successfully\n");
    } else {
        printf("test csi_crc16_ccitt faild %x\n", out);
    }

    out = csi_crc16_dnp(0, (uint8_t *)crc_input1, 16);

    if (out == expect_out[6]) {
        printf("test csi_crc16_dnp successfully\n");
    } else {
        printf("test csi_crc16_dnp faild %x\n", out);
    }

    out = csi_crc16_itu(0, (uint8_t *)crc_input1, 16);

    if (out == expect_out[7]) {
        printf("test csi_crc16_itu successfully\n");
    } else {
        printf("test csi_crc16_itu faild %x\n", out);
    }

    out = csi_crc32_le(0, (uint8_t *)crc_input1, 16);

    if (out == expect_out[8]) {
        printf("test csi_crc32_le successfully\n");
    } else {
        printf("test csi_crc32_le faild %x\n", out);
    }

    out = csi_crc32_be(0, (uint8_t *)crc_input1, 16);

    if (out == expect_out[9]) {
        printf("test csi_crc32_be successfully\n");
    } else {
        printf("test csi_crc32_be faild %x\n", out);
    }

    printf("test crc finish\n");
}

/*****************************************************************************
test_main: main function of the &crc test

INPUT: NULL

RETURN: NULL

*****************************************************************************/
int example_crc(void)
{
    crc_modbus();
    return 0;
}
extern int board_init(void);
int main(void)
{
    board_init();
    printf("hello\n");
    return example_crc();
}

```

