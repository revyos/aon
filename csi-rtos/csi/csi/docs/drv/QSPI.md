# QSPI
-----

## 简要说明
QSPI 是 Quad SPI 的简写，是 Motorola 公司推出的 SPI 接口的扩展，比 SPI 应用更加广泛。在 SPI 协议的基础上，Motorola 公司对其功能进行了增强，增加了队列传输机制，推出了队列串行外围接口协议（即 QSPI 协议）。使用该接口，用户可以一次性传输包含多达 16 个 8 位或 16 位数据的传输队列。一旦传输启动，直到传输结束，都不需要 CPU 干预，极大的提高了传输效率。与 SPI 相比，QSPI 的最大结构特点是以 80 字节的 RAM 代替了 SPI 的发送和接收数据寄存器。

## 接口列表
QSPI的CSI接口说明如下所示：

|   函数                      |     说明              |
|-----------------------------|----------------------|
|csi_qspi_init                |QSPI初始化            |
|csi_qspi_uninit              |QSPI反初始化           |
|csi_qspi_attach_callback     |注册回调函数           |
|csi_qspi_detach_callback     |注销回调函数           |
|csi_qspi_frequence           |配置QSPI频率           |
|csi_qspi_mode                |配置QSPI时钟模式       |
|csi_qspi_send                |发送数据（同步模式）    |
|csi_qspi_receive             |接收数据（同步模式）    |
|csi_qspi_send_receive        |发送接收数据（同步模式) |
|csi_qspi_send_async          |发送数据（异步模式）    |
|csi_qspi_receive_async       |接收数据（异步模式）    |
|csi_qspi_send_receive_async  |发送接收数据（异步模式) |
|csi_qspi_link_dma            |绑定/注销DMA通道       |
|csi_qspi_get_state           |获取QSPI状态          |
|csi_qspi_memory_mapped       |配置内存映射         |


## 接口详细说明

### csi_qspi_init

```c
csi_error_t csi_qspi_init(csi_qspi_t *qspi, uint32_t idx)
```

- 功能描述:

  通过设备ID初始化对应的QSPI实例，返回结果值。

- 参数:

  - `qspi`: 设备句柄（需要用户申请句柄空间）。
  - `idx` : 设备ID。
- 返回值: 

  - 错误码csi_error_t

* * *
### csi_qspi_t    

| 成员        | 类型           | 说明                   |
| ----------- | -------------  | ---------------------- |
| dev         | csi_dev_t      | 设备统一句柄           |
| cb          | void (*callback)(csi_qspi_t *qspi, csi_qspi_event_t event, void *arg) | 用户回调函数 |
| arg         | void*          | 用户回调函数对应的传参 |
| tx_data     | void*          | 指向发送缓存的地址     |
| tx_size     | uint32_t       | 发送数据的大小         |
| rx_data     | void*          | 指向接收缓存的地址     |
| rx_size     | uint32_t       | 接收缓存的大小         |
| send        | void*          | 指向发送函数(异步)     |
| receive     | void*          | 指向接收函数(异步)     |
| send_receive| void*          | 指向发送接收函数(异步)  |
| state       | csi_state_t    | 运行状态               |
| tx_dma      | csi_dma_ch_t*  | 指向发送DMA句柄        |
| rx_dma      | csi_dma_ch_t*  | 指向接收DMA句柄        |
| priv        | void*   | 设备私有变量       |


### csi_qspi_uninit

```c
void csi_qspi_uninit(csi_qspi_t *qspi)
```

- 功能描述:

- QSPI实例反初始化, 并且释放相关的软硬件资源。

- 参数:

  - `qspi`: 实例句柄。 

- 返回值

  - 无

### csi_qspi_attach_callback
```c
csi_error_t csi_qspi_attach_callback(csi_qspi_t *qspi, void *callback, void *arg)
```

- 功能描述:

  - 注册回调函数到指定QSPI控制器。

- 参数:

  - `qspi`: 实例句柄。
  - `callback`: 回调函数。
  - `arg`: 回调函数的参数。

- 参数: 

  - 错误码csi_error_t

### csi_qspi_detach_callback
```c
void csi_qspi_detach_callback(csi_qspi_t *qspi)
```
- 功能描述:

  - 注销回调函数。

- 参数:

  - `qspi`: 实例句柄。


### csi_qspi_frequence

```c
uint32_t csi_qspi_frequence(csi_qspi_t *qspi, uint32_t hz)
```

- 功能描述:

- 设置QSPI频率。

- 参数:

  - `qspi`: 实例句柄。 
  - `hz`: 工作频率。

- 返回值

  - 实际设置频率 


### csi_qspi_mode

```c
csi_error_t csi_qspi_mode(csi_qspi_t *qspi, csi_qspi_mode_t mode)
```

- 功能描述:

- 设置QSPI时钟模式。

- 参数:

  - `qspi`: 实例句柄。 
  - `mode`: 时钟模式。

- 返回值

  - 错误码csi_error_t

* * *
#### csi_qspi_clock_mode_t

| 类型                  | 说明                      |
| --------------------- | ------------------------- |
| QSPI_CLOCK_MODE_0     | 模式0: CPOL = 0, CPHA = 0 |
| QSPI_CLOCK_MODE_3     | 模式3: CPOL = 1, CPHA = 1 |


### csi_qspi_send
```c
int32_t csi_qspi_send(csi_qspi_t *qspi, csi_qspi_command_t *cmd, const void *data, uint32_t size, uint32_t timeout)
```

- 功能描述:

- 以轮询模式进行数据发送。

- 参数:

  - `qspi`    : 实例句柄。 
  - `cmd`     : 指向command配置信息。
  - `data`    : 指向发送数据缓存。
  - `size`    : 指定需要发送的数据长度。 
  - `timeout` : 发送超时时间，单位ms。 

- 返回值
  - 发送成功，返回实际发送长度。
    发送失败，返回错误码。

- 错误码

* * *
#### csi_qspi_command_t

| 成员                  | 类型                    | 说明              |
| --------------------- | ----------------------- | ----------------- |
| instruction.bus_width | csi_qspi_bus_width_t    | 指令阶段总线线数  |
| instruction.value     | uint8_t                 | 指令值            |
| instruction.disabled  | bool                    | 指令使能/禁能     |
| address.bus_width     | csi_qspi_bus_width_t    | 地址阶段总线线数  |
| address.size          | csi_qspi_address_size_t | 地址字节数        |
| address.value         | uint32_t                | 地址值            |
| address.disabled      | bool                    | 地址使能/禁能     |
| alt.bus_width         | csi_qspi_bus_width_t    | 交替字节总线数    |
| alt.size              | csi_qspi_alt_size_t     | 交替字节字节数    |
| alt.value             | uint32_t                | 交替字节数值      |
| alt.disabled          | bool                    | 交替字节使能/禁能 |
| dummy_count           | uint8_t                 | dummy数量         |
| data.bus_width        | csi_qspi_bus_width_t    | data总线数        |



* * *
### csi_qspi_alt_size_t

| 类型                         | 说明  |
| ---------------------------- | ----- |
| QSPI_ALTERNATE_BYTES_8_BITS  | 1字节 |
| QSPI_ALTERNATE_BYTES_16_BITS | 2字节 |
| QSPI_ALTERNATE_BYTES_24_BITS | 3字节 |
| QSPI_ALTERNATE_BYTES_32_BITS | 4字节 |

* * *
#### csi_qspi_bus_width_t 

| 类型                 | 说明         |
| -------------------- | ----------- |
| QSPI_CFG_BUS_SINGLE  | 单线        |
| QSPI_CFG_BUS_DUAL    | 双线        |
| QSPI_CFG_BUS_QUAD    | 四线        |

* * *
#### csi_qspi_address_size_t

| 类型                 | 说明      |
| -------------------- | -------- |
| QSPI_ADDRESS_8_BITS  | 8比特    |
| QSPI_ADDRESS_16_BITS | 16比特   |
| QSPI_ADDRESS_24_BITS | 24比特   |
| QSPI_ADDRESS_32_BITS | 32比特   |

* * *


### csi_qspi_receive
```c
uint32_t csi_qspi_receive(csi_qspi_t *qspi, csi_qspi_command_t *cmd, void *data, uint32_t size, uint32_t timeout)
```
- 功能描述:

- 以轮询模式进行数据接收。

- 参数:

  - `qspi`    : 实例句柄。 
  - `cmd`     : 指向command配置信息。
  - `data`    : 指向接收数据缓存。
  - `size`     : 指定需要接收的数据长度。 
  - `timeout` : 数据接收超时时间，单位ms。 

- 返回值

  - 错误码csi_error_t


### csi_qspi_send_receive

```c
uint32_t csi_qspi_send_receive(csi_qspi_t *qspi, csi_qspi_command_t *cmd, const void *tx_data, void *rx_data, uint32_t size, uint32_t timeout)
```
- 功能描述:

- 以轮询模式进行数据发送/接收。

- 参数:

  - `qspi`    : 实例句柄。 
  - `cmd`     : 指向command配置信息。
  - `tx_data` : 指向发送数据缓存。
  - `rx_data` : 指向接收数据缓存。
  - `size`    : 指定发送/接收的数据长度。 
  - `timeout` : 数据接收超时时间，单位ms。 

- 返回值

  - 错误码csi_error_t

### csi_qspi_send_async
```c
csi_error_t csi_qspi_send_async(csi_qspi_t *qspi, csi_qspi_command_t *cmd, const void *data, uint32_t size)
```

- 功能描述:

- 以异步模式进行数据发送。

- 参数:

  - `qspi`    : 实例句柄。 
  - `cmd`     : 指向command配置信息。
  - `data`    : 指向发送数据缓存。
  - `size`    : 指定需要发送的数据长度。 

- 返回值

  - 错误码csi_error_t

### csi_qspi_receive_async

```c
csi_error_t csi_qspi_receive_async(csi_qspi_t *qspi, csi_qspi_command_t *cmd, void *data, uint32_t size)
```

- 功能描述:

- 以异步模式进行数据接收。

- 参数:

  - `qspi` : 实例句柄。 
  - `cmd`  : 指向command配置信息。
  - `data` : 指向数据数据缓存。
  - `size`  : 指定需要接收的数据长度。 

- 返回值

  - 错误码csi_error_t

### csi_qspi_send_receive_async 
```c
csi_error_t csi_qspi_send_receive_async(csi_qspi_t *qspi, csi_qspi_command_t *cmd, const void *tx_data, void *rx_data, uint32_t size)
```
- 功能描述:

- 以异步模式进行数据发送/接收。

- 参数:

  - `qspi`    : 实例句柄。 
  - `cmd`     : 指向command配置信息。
  - `tx_data` : 指向发送数据缓存。
  - `rx_data` : 指向接收数据缓存。
  - `size`    : 指定发送接收的数据长度。 


- 返回值

  - 错误码csi_error_t


### csi_qspi_link_dma
```c
csi_error_t csi_qspi_link_dma(csi_qspi_t *qspi, csi_dma_ch_t *tx_dma, csi_dma_ch_t *rx_dma)
```
- 功能描述:

- 绑定/注销DMA通道。当传入参数为NULL时注销通道，当参数不为NULL时绑定通道。

- 参数:

  - `qspi` : 实例句柄。
  - `tx_dma` : 指向发送dma通道。
  - `rx_dma` : 指向接收dma通道。
- 返回值: 

  - 错误码csi_error_t


### csi_qspi_get_state
```c
csi_error_t csi_qspi_get_state(csi_qspi_t *qspi, csi_state_t *state)
```
- 功能描述:

- 获取QSPI状态。

- 参数:

  - `qspi` : 实例句柄。
  - `state` : 指向接收的状态值。
- 返回值: 

  - 错误码csi_error_t

#### csi_state_t

| 类型 | 说明     |
| ------------- | --------- |
| readable | 设备可读 |
| writeable | 设备可写 |
| error | 错误状态 |

### csi_qspi_memory_mapped
```c
csi_error_t csi_qspi_memory_mapped(csi_qspi_t *qspi, csi_qspi_command_t *cmd)
```
- 功能描述:

- 设置QSPI内存映射模式。

- 参数:

  - `qspi` : 实例句柄。
  - `cmd`     : 指向command配置信息。
- 返回值: 

  - 错误码csi_error_t



## 使用示例
```c

示例展示了如何使用QSPI读取SPIFLASH的JEDEC ID

#include <stdio.h>
#include <string.h>

#include <soc.h>
#include <drv/qspi.h>
#include <drv/tick.h>
#include <csi_config.h>
#include <board_config.h>
#include <board_init.h>

#define W25Q64FV_READ_JEDEC_ID            0x9F

static csi_qspi_t     qspi_handle; 
static csi_qspi_command_t  command; 
int main(void)
{
    int ret;
    uint8_t device_id[3];

    board_init();

    ret = csi_qspi_init(&qspi_handle, 0);

    if (ret != CSI_OK) {
        return -1;
    }

    /* Read device id operations */
    command.instruction.value     = W25Q64FV_READ_DEVICE_ID;
    command.instruction.bus_width = QSPI_CFG_BUS_SINGLE;
    command.instruction.disabled  = false;
    command.alt.disabled          = true;
    command.address.disabled      = true;
    command.address.value         = 0;
    command.address.size          = 0;
    command.address.bus_width     = QSPI_CFG_BUS_SINGLE;
    command.data.bus_width        = QSPI_CFG_BUS_SINGLE;
    ret = csi_qspi_receive(&qspi_handle, &command, device_id, 3, 1000);
    if (ret != 3) {
        return -1;
    }

    printf("device id: %x%x%x", device_id[0], device_id[1], device_id[2]);
    return 0;
}


```
