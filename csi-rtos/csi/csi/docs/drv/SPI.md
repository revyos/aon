# SPI设备
-----

## 设备说明

SPI是串行外设接口(Serial Peripheral Interface)一种同步串行接口技术，是一种高速的，全双工，同步的通信总线。

## 接口列表
SPI的CSI接口说明如下所示：

|   函数                       |     说明             |
|-----------------------------|----------------------|
|csi_spi_init                 |SPI设备初始化          |
|csi_spi_uninit               |SPI设备反初始化        |
|csi_spi_attach_callback      |设置回调函数           |
|csi_spi_detach_callback      |注销回调函数           |
|csi_spi_mode                 |设置SPI主/从模式       |
|csi_spi_cp_format            |设置SPI相位模式        |
|csi_spi_frame_len            |设置SPI数据宽度        |
|csi_spi_baud                 |设置SPI波特率          |
|csi_spi_send                 |数据发送(同步模式)     |
|csi_spi_send_async           |数据发送(异步模式)     |
|csi_spi_receive              |数据接收(同步模式)     |
|csi_spi_receive_async        |数据接收(异步模式)     |
|csi_spi_send_receive         |数据发送/接收(同步模式) |
|csi_spi_send_receive_async   |数据发送/接收(异步模式) |
|csi_spi_select_slave         |选择指定从机           |
|csi_spi_link_dma             |绑定/解除DMA通道       |
|csi_spi_get_state            |获取SPI状态            |

## 接口详细说明

### csi_spi_init

```c
csi_error_t csi_spi_init(csi_spi_t *spi, uint32_t idx)
```

- 功能描述:

- 通过设备ID初始化对应的SPI实例，返回结果值。

- 参数:

  - `spi`: 设备句柄（需要用户申请句柄空间）
  - `idx`: 设备ID
- 返回值: 

  - 错误码csi_error_t

* * *
### csi_spi_t

| 成员         | 类型            | 说明                   |
| ------------ | --------------- | --------------------- |
| dev          | csi_dev_t       | 设备统一句柄           |
| callback     | void (*callback)(csi_spi_t *spi, csi_spi_event_t event, void *arg) | 用户回调函数 |
| arg          | *void            | 用户回调函数对应的传参  |
| tx_data      | *void            | 指向发送缓存的地址      |
| tx_size      | uint32_t        | 发送数据的大小          |
| rx_data      | *void            | 指向接收缓存的地址      |
| rx_size      | uint32_t        | 接收数据的大小          |
| send         | csi_error_t (*csi_spi_send_async_t)(csi_spi_t *spi, const void *data, uint32_t size) | 指向发送函数(异步)      |
| receive      | csi_error_t (*csi_spi_receive_async_t)(csi_spi_t *spi, void *data, uint32_t size) | 指向接收函数(异步) |
| send_receive | csi_error_t (*csi_spi_send_receive_async_t)(csi_spi_t *spi, const void *data_out, void *data_in, uint32_t size) | 指向发送/接收函数(异步) |
| state        | csi_spi_state_t | 运行状态               |
| tx_dma      | *csi_dma_ch_t    | 指向发送DMA通道        |
| rx_dma      | *csi_dma_ch_t    | 指向接收DMA通道        |
| priv        | *void            | 设备私有变量       |

### csi_spi_uninit

```c
void csi_spi_uninit(csi_spi_t *spi)
```

- 功能描述:

- SPI实例反初始化, 并且释放相关的软硬件资源。

- 参数:

  - `spi`: 实例句柄。 

- 返回值

  - 无

### csi_spi_attach_callback

```c
csi_error_t csi_spi_attach_callback(csi_spi_t *spi, void *callback, void *arg)
```

- 功能描述:

- 注册SPI事件回调函数。

- 参数:

  - `spi` : 实例句柄。
  - `callback` : 指向事件回调函数。
  - `arg` : 指向事件回调函数的参数。
- 返回值: 

  - 错误码csi_error_t

####  callback

```c
void (*callback)(csi_spi_t *spi, csi_spi_event_t event, void *arg);
```

其中SPI为设备句柄，idx为设备ID，event 为传给回调函数的事件类型，arg为用户自定义的回调函数对应的参数。 SPI回调事件枚举类型csi_spi_event_t定义如下：


#### csi_spi_event_t

| 类型                                 | 说明                  |
| ---------------------                | --------              |
| SPI_EVENT_TRANSMIT_COMPLETE          | 数据发送完成事件      |
| SPI_EVENT_RECEIVE_COMPLETE           | 数据接收完成事件      |
| SPI_EVENT_TRANSMIT_RECEIVE_COMPLETE  | 数据发送/接收完成事件 |
| SPI_EVENT_ERROR_OVERFLOW             | 数据溢出事件          |
| SPI_EVENT_ERROR_UNDERFLOW            | 数据下溢事件          |
| SPI_EVENT_ERROR                      | 总线错误事件          |


### csi_spi_detach_callback

```c
void csi_spi_detach_callback(csi_spi_t *spi)
```

- 功能描述:

- 注销SPI事件回调函数。

- 参数:

  - `spi` : 实例句柄。
- 返回值: 

  - 无。

### csi_spi_mode

```c
csi_error_t csi_spi_mode(csi_spi_t *spi, csi_spi_mode_t mode)
```

- 功能描述:

- 设置SPI主/从模式。

- 参数:

  - `spi` : 实例句柄。
  - `mode` : 主/从模式。
- 返回值: 

  - 错误码csi_error_t

* * *
#### csi_spi_mode_t

| 类型            | 说明       |
| --------------- | ---------- |
| SPI_MASTER      | 主机模式   |
| SPI_SLAVE       | 从机模式   |


### csi_spi_cp_format

```c
csi_error_t csi_spi_cp_format(csi_spi_t *spi, csi_spi_cp_format_t format)
```

- 功能描述:

- 设置SPI相位模式。

- 参数:

  - `spi` : 实例句柄。
  - `format` : 相位格式。
- 返回值: 

  - 错误码csi_error_t

* * *
#### csi_spi_cp_format_t

| 类型                  | 说明           |
| --------------------- | --------       |
| SPI_FORMAT_CPOL0_CPHA0| CPOL=0  CPHA=0 |
| SPI_FORMAT_CPOL0_CPHA1| CPOL=0  CPHA=1 |
| SPI_FORMAT_CPOL1_CPHA0| CPOL=1  CPHA=0 |
| SPI_FORMAT_CPOL1_CPHA1| CPOL=1  CPHA=1 |


### csi_spi_frame_len

```c
csi_error_t csi_spi_frame_len(csi_spi_t *spi, csi_spi_frame_len_t length)
```

- 功能描述:

- 设置SPI数据宽度。

- 参数:

  - `spi` : 实例句柄。
  - `length` : 数据宽度。
- 返回值: 

  - 错误码csi_error_t

* * *
#### csi_spi_frame_len_t

| 类型                  | 说明           |
| --------------------- | --------       |
| SPI_FRAME_LEN_4       | 数据宽度：4bit |
| SPI_FRAME_LEN_5       | 数据宽度：5bit |
| SPI_FRAME_LEN_6       | 数据宽度：6bit |
| SPI_FRAME_LEN_7       | 数据宽度：7bit |
| SPI_FRAME_LEN_8       | 数据宽度：8bit |
| SPI_FRAME_LEN_9       | 数据宽度：9bit |
| SPI_FRAME_LEN_10      | 数据宽度：10bit|
| SPI_FRAME_LEN_11      | 数据宽度：11bit|
| SPI_FRAME_LEN_12      | 数据宽度：12bit|
| SPI_FRAME_LEN_13      | 数据宽度：13bit|
| SPI_FRAME_LEN_14      | 数据宽度：14bit|
| SPI_FRAME_LEN_15      | 数据宽度：15bit|
| SPI_FRAME_LEN_16      | 数据宽度：16bit|

### csi_spi_baud

```c
uint32_t  csi_spi_baud(csi_spi_t *spi, uint32_t baud)
```

- 功能描述:

- 设置SPI波特率。

- 参数:

  - `spi` : 实例句柄。
  - `baud` : 波特率。
- 返回值: 

  - 实际频率值，单位HZ。


### csi_spi_send

```c
int32_t csi_spi_send(csi_spi_t *spi, const void *data, uint32_t size, uint32_t timeout)
```

- 功能描述: 

- 以阻塞模式进行数据发送，接收数据将被忽略。当发送成功或者错误时，函数返回。

- 参数:

  - `spi`: 实例句柄。
  - `data`: 指向发送数据的缓存。
  - `size`: 发送数据的长度。
  - `timeout`: 发送超时时间，单位ms。

- 返回值
  - 发送成功，返回实际发送长度。
    发送失败，返回错误码。

- 错误码

### csi_spi_send_async

```c
csi_error_t csi_spi_send_async(csi_spi_t *spi, const void *data, uint32_t size)
```

- 功能描述: 

- 以中断或者DMA模式进行数据发送，接收数据将被忽略。当发送成功时，会收到SPI_EVENT_SEND_COMPLETE事件。

- 参数:

  - `spi`: 实例句柄。
  - `data`: 指向发送数据的缓存。
  - `size`: 发送数据的长度。

- 返回值

  - 错误码csi_error_t

### csi_spi_receive

```c
uint32_t csi_spi_receive(csi_spi_t *spi, void *data, uint32_t size, uint32_t timeout)
```

- 功能描述: 

- 以阻塞模式进行数据接收。当接收成功或者错误时，函数返回。

- 参数:

  - `spi`: 实例句柄。
  - `data`: 指向接收数据的缓存。
  - `size`: 接收数据的长度。
  - `timeout`: 发送超时时间，单位ms。

- 返回值
  - 接收成功，返回实际接收长度。
    接收失败，返回错误码。

- 错误码

### csi_spi_receive_async

```c
csi_error_t csi_spi_receive_async(csi_spi_t *spi, void *data, uint32_t size)
```

- 功能描述: 

- 以中断或者DMA模式进行数据接收。当接收成功时，会收到SPI_EVENT_RECEIVE_COMPLETE事件。

- 参数:

  - `spi`: 实例句柄。
  - `data`: 指向接收数据的缓存。
  - `size`: 需要接收的长度。

- 返回值

  - 错误码csi_error_t

### csi_spi_send_receive

```c
uint32_t csi_spi_send_receive(csi_spi_t *spi, const void *data_out, void *data_in, uint32_t size, uint32_t timeout)
```

- 功能描述: 

- 以阻塞模式进行数据发送/接收。当传输成功或者错误时，函数返回。

- 参数:

  - `spi`: 实例句柄。
  - `data_out`: 指向发送数据的缓存。
  - `data_in`: 指向接收数据的缓存。
  - `size`: 需要传输的数据长度。
  - `timeout`: 传输超时，单位ms。

- 返回值
  - 发送/接收成功，返回实际发送/接收长度。
    发送/接收失败，返回错误码。

- 错误码

  - 错误码csi_error_t


### csi_spi_send_receive_async

```c
csi_error_t csi_spi_send_receive_async(csi_spi_t *spi, const void *data_out, void *data_in, uint32_t size)
```

- 功能描述: 

- 以中断或者DMA模式进行数据发送/接收。当传输成功时，会收到SPI_EVENT_SEND_RECEIVE_COMPLETE事件。

- 参数:

  - `spi`: 实例句柄。
  - `data_out`: 指向发送数据的缓存。
  - `data_in`: 指向接收数据的缓存。
  - `size`: 需要传输的数据长度。

- 返回值

  - 错误码csi_error_t

### csi_spi_select_slave

```c
void csi_spi_select_slave(csi_spi_t *spi, int slave_num)
```

- 功能描述:

- 选择指定从机，仅在master模式下有效。

- 参数:

  - `spi`: 实例句柄。
  - `slave_num`: 指定从机号。
- 返回值
  - 无

### csi_spi_link_dma

```c
csi_error_t csi_spi_link_dma(csi_spi_t *spi, csi_dma_ch_t *tx_dma, csi_dma_ch_t *rx_dma)
```

- 功能描述:

- 绑定/解除DMA通道。

- 参数:

  - `spi` : 实例句柄。
  - `tx_dma` : 指向发送dma通道。
  - `rx_dma` : 指向接收dma通道。
- 返回值: 

  - 错误码csi_error_t


```c
csi_error_t csi_spi_get_state(csi_spi_t *spi, csi_state_t *state)
```

- 功能描述:

- 获取SPI状态。

- 参数:

  - `spi` : 实例句柄。
  - `state` : 指向接收的状态值。
- 返回值: 

  - 错误码csi_error_t

## 同步模式使用示例

示例展示了如何使用同步模式的数据发送，需要使用两个SPI。一个SPI作为MASTER同步发送数据，另一个作为SLAVE异步接收数据。使用前需要检查IO口是否连接。

```c
#include <stdio.h>
#include <string.h>

#include <soc.h>
#include <drv/spi.h>
#include <drv/tick.h>
#include <csi_config.h>
#include <board_config.h>
#include <board_init.h>

#define DEFAULT_TRANSFER_TIMEOUT 5000
#define SPI_CHECK_RETURN(ret)           \
    do {                                \
        if (ret != CSI_OK) {            \
                return -1;              \
            }                           \
    } while(0);
    

csi_spi_event_t spi_master_event, spi_slave_event;
csi_spi_t spi_master, spi_slave;

static void slave_spi_event_callback(csi_spi_t *spi, csi_spi_event_t event, void *arg)
{
    spi_slave_event = event;
}

void example_pin_spi_init(void)
{
    csi_pin_set_mux(EXAMPLE_PIN_SPI_MISO,       EXAMPLE_PIN_SPI_MISO_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_SPI_MOSI,       EXAMPLE_PIN_SPI_MOSI_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_SPI_CS,         EXAMPLE_PIN_SPI_CS_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_SPI_SCK,        EXAMPLE_PIN_SPI_SCK_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_SPI_SLAVE_MISO, EXAMPLE_PIN_SPI_SLAVE_MISO_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_SPI_SLAVE_MOSI, EXAMPLE_PIN_SPI_SLAVE_MOSI_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_SPI_SLAVE_CS,   EXAMPLE_PIN_SPI_SLAVE_CS_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_SPI_SLAVE_SCK,  EXAMPLE_PIN_SPI_SLAVE_SCK_FUNC);
}

int main(void)
{
    int      ret;
    uint8_t  rx_data[11];
    uint32_t timestart;

    example_pin_spi_init();
    board_init();

    /* Initialize master SPI */
    ret = csi_spi_init(&spi_master, EXAMPLE_SPI_IDX);
    SPI_CHECK_RETURN(ret);

    /* Set master SPI as master mode */
    ret = csi_spi_mode(&spi_master, SPI_MASTER);
    SPI_CHECK_RETURN(ret);

    ret = csi_spi_cp_format(&spi_master, SPI_FORMAT_CPOL0_CPHA0);
    SPI_CHECK_RETURN(ret);

    ret = csi_spi_frame_len(&spi_master, SPI_FRAME_LEN_8);
    SPI_CHECK_RETURN(ret);

    ret = csi_spi_baud(&spi_master, 100000);
    if(ret == 0){
        return -1;
    }

    /* Select salve 0 of master SPI */
    csi_spi_select_slave(&spi_master, 0);

    /* Initialize slave spi */
    ret = csi_spi_init(&spi_slave, EXAMPLE_SPI_SLAVE_IDX);
    SPI_CHECK_RETURN(ret);

    ret = csi_spi_attach_callback(&spi_slave, slave_spi_event_callback, NULL);
    SPI_CHECK_RETURN(ret);

    ret = csi_spi_mode(&spi_slave, SPI_SLAVE);
    SPI_CHECK_RETURN(ret);

    ret = csi_spi_cp_format(&spi_slave, SPI_FORMAT_CPOL1_CPHA0);
    SPI_CHECK_RETURN(ret);

    ret = csi_spi_frame_len(&spi_slave, SPI_FRAME_LEN_8);
    SPI_CHECK_RETURN(ret);

    ret = csi_spi_baud(&spi_slave, 100000);
    if(ret == 0){
        return -1;
    }

    /* Slave SPI start receive */
    spi_slave_event = -1;
    ret = csi_spi_receive_async(&spi_slave, rx_data, 11);
    SPI_CHECK_RETURN(ret);

    /* Master SPI send data */
    ret = csi_spi_send(&spi_master, "hello word", 11, 1000);
    printf("send: %s\n", "hello word");
    if (ret != 11) {
        return -1;
    }

    /* Wait transfer complete */
    timestart = csi_tick_get();
    while (spi_slave_event != SPI_EVENT_RECEIVE_COMPLETE) {
        if ((csi_tick_get() - timestart) > 5000) {
            return -1;
        }
    }
    printf("receive: %s\n", rx_data);

    if (0 != strcmp((char *)rx_data, "hello word")) {
        return -1;
    }

    /* Uninit SPI */
    csi_spi_detach_callback(&spi_master);

    csi_spi_detach_callback(&spi_slave);

    csi_spi_uninit(&spi_master);
    csi_spi_uninit(&spi_slave);
    return 0;
}
```
## 中断模式使用示例

示例展示了如何使用中断模式的数据发送，需要使用两个SPI。一个SPI作为MASTER异步发送数据，另一个作为SLAVE异步接收数据。使用前需要检查IO口是否连接。
> 使用中断模式需要调用csi_spi_attach_callback(),并使用异步传输接口csi_spi_xxxx_async()

```c
#include <stdio.h>
#include <string.h>

#include <soc.h>
#include <drv/spi.h>
#include <drv/tick.h>
#include <csi_config.h>
#include <board_config.h>
#include <board_init.h>

#define DEFAULT_TRANSFER_TIMEOUT 5000
#define SPI_CHECK_RETURN(ret)           \
    do {                                \
        if (ret != CSI_OK) {            \
                return -1;              \
            }                           \
    } while(0);
    

csi_spi_event_t spi_master_event, spi_slave_event;
csi_spi_t spi_master, spi_slave;

static void master_spi_event_callback(csi_spi_t *spi, csi_spi_event_t event, void *arg)
{
    spi_master_event = event;
}

static void slave_spi_event_callback(csi_spi_t *spi, csi_spi_event_t event, void *arg)
{
    spi_slave_event = event;
}

void example_pin_spi_init(void)
{
    csi_pin_set_mux(EXAMPLE_PIN_SPI_MISO,       EXAMPLE_PIN_SPI_MISO_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_SPI_MOSI,       EXAMPLE_PIN_SPI_MOSI_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_SPI_CS,         EXAMPLE_PIN_SPI_CS_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_SPI_SCK,        EXAMPLE_PIN_SPI_SCK_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_SPI_SLAVE_MISO, EXAMPLE_PIN_SPI_SLAVE_MISO_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_SPI_SLAVE_MOSI, EXAMPLE_PIN_SPI_SLAVE_MOSI_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_SPI_SLAVE_CS,   EXAMPLE_PIN_SPI_SLAVE_CS_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_SPI_SLAVE_SCK,  EXAMPLE_PIN_SPI_SLAVE_SCK_FUNC);
}

int main(void)
{
    int      ret;
    uint8_t  rx_data[41];
    uint32_t timestart;

    example_pin_spi_init();
    board_init();

    /* Initialize master SPI */
    ret = csi_spi_init(&spi_master, EXAMPLE_SPI_IDX);
    SPI_CHECK_RETURN(ret);

    /* Master SPI attach callback */
    ret = csi_spi_attach_callback(&spi_master, master_spi_event_callback, NULL);
    SPI_CHECK_RETURN(ret);

    /* Set master SPI as master mode */
    ret = csi_spi_mode(&spi_master, SPI_MASTER);
    SPI_CHECK_RETURN(ret);

    ret = csi_spi_cp_format(&spi_master, SPI_FORMAT_CPOL0_CPHA0);
    SPI_CHECK_RETURN(ret);

    ret = csi_spi_frame_len(&spi_master, SPI_FRAME_LEN_8);
    SPI_CHECK_RETURN(ret);

    ret = csi_spi_baud(&spi_master, 10000000);
    if(ret == 0){
        return -1;
    }

    /* Select salve 0 of master SPI */
    csi_spi_select_slave(&spi_master, 0);

    /* Initialize slave SPI */
    ret = csi_spi_init(&spi_slave, EXAMPLE_SPI_SLAVE_IDX);
    SPI_CHECK_RETURN(ret);

    ret = csi_spi_attach_callback(&spi_slave, slave_spi_event_callback, NULL);
    SPI_CHECK_RETURN(ret);

    ret = csi_spi_mode(&spi_slave, SPI_SLAVE);
    SPI_CHECK_RETURN(ret);

    ret = csi_spi_cp_format(&spi_slave, SPI_FORMAT_CPOL1_CPHA0);
    SPI_CHECK_RETURN(ret);

    ret = csi_spi_frame_len(&spi_slave, SPI_FRAME_LEN_8);
    SPI_CHECK_RETURN(ret);

    ret = csi_spi_baud(&spi_slave, 10000000);
    if(ret == 0){
        return -1;
    }

    /* Slave SPI start receive */
    spi_slave_event = -1;
    ret = csi_spi_receive_async(&spi_slave, rx_data, 11);
    SPI_CHECK_RETURN(ret);

    /* Master SPI send data */
    ret = csi_spi_send_async(&spi_master, "hello word", 11);
    printf("send: %s\n", "hello word");
    SPI_CHECK_RETURN(ret);

    /* Wait transfer complete */
    timestart = csi_tick_get();
    while (( spi_slave_event != SPI_EVENT_RECEIVE_COMPLETE  ) || ( spi_master_event != SPI_EVENT_SEND_COMPLETE )) {
        if ((csi_tick_get() - timestart) > 5000) {
            return -1;
        }
    }
    printf("receive: %s\n", rx_data);

    if (0 != strcmp((char *)rx_data, "hello word")) {
        return -1;
    }

    /* Uninit SPI */
    csi_spi_detach_callback(&spi_master);

    csi_spi_detach_callback(&spi_slave);

    csi_spi_uninit(&spi_master);
    csi_spi_uninit(&spi_slave);
    return 0;
}
```
## DMA模式使用示例

示例展示了如何使用DMA模式的数据发送，需要使用两个SPI。一个SPI作为MASTER DMA发送数据，另一个作为SLAVE中断接收数据。使用前需要检查IO口是否连接。
> 使用DMA模式需要调用csi_spi_link_dma()

```c

#include <stdio.h>
#include <string.h>

#include <soc.h>
#include <drv/spi.h>
#include <drv/tick.h>
#include <csi_config.h>
#include <board_config.h>
#include <board_init.h>

#define DEFAULT_TRANSFER_TIMEOUT 5000
#define SPI_CHECK_RETURN(ret)           \
    do {                                \
        if (ret != CSI_OK) {            \
                return -1;              \
            }                           \
    } while(0);
    

csi_spi_event_t spi_master_event, spi_slave_event;
csi_spi_t spi_master, spi_slave;
csi_dma_ch_t dma_tx_ch;
csi_dma_ch_t dma_rx_ch;

static void master_spi_event_callback(csi_spi_t *spi, csi_spi_event_t event, void *arg)
{
    spi_master_event = event;
}

static void slave_spi_event_callback(csi_spi_t *spi, csi_spi_event_t event, void *arg)
{
    spi_slave_event = event;
}

void example_pin_spi_init(void)
{
    csi_pin_set_mux(EXAMPLE_PIN_SPI_MISO,       EXAMPLE_PIN_SPI_MISO_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_SPI_MOSI,       EXAMPLE_PIN_SPI_MOSI_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_SPI_CS,         EXAMPLE_PIN_SPI_CS_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_SPI_SCK,        EXAMPLE_PIN_SPI_SCK_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_SPI_SLAVE_MISO, EXAMPLE_PIN_SPI_SLAVE_MISO_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_SPI_SLAVE_MOSI, EXAMPLE_PIN_SPI_SLAVE_MOSI_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_SPI_SLAVE_CS,   EXAMPLE_PIN_SPI_SLAVE_CS_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_SPI_SLAVE_SCK,  EXAMPLE_PIN_SPI_SLAVE_SCK_FUNC);
}

int main(void)
{
    int      ret;
    uint8_t  rx_data[41];
    uint32_t timestart;

    example_pin_spi_init();
    board_init();

    /* Initialize master SPI */
    ret = csi_spi_init(&spi_master, EXAMPLE_SPI_IDX);
    SPI_CHECK_RETURN(ret);

    /* Master spi attach callback */
    ret = csi_spi_attach_callback(&spi_master, master_spi_event_callback, NULL);
    SPI_CHECK_RETURN(ret);

    ret = csi_spi_link_dma(&spi_master, &dma_tx_ch, &dma_rx_ch);
    SPI_CHECK_RETURN(ret);

    /* Set master spi as master mode */
    ret = csi_spi_mode(&spi_master, SPI_MASTER);
    SPI_CHECK_RETURN(ret);

    ret = csi_spi_cp_format(&spi_master, SPI_FORMAT_CPOL0_CPHA0);
    SPI_CHECK_RETURN(ret);

    ret = csi_spi_frame_len(&spi_master, SPI_FRAME_LEN_8);
    SPI_CHECK_RETURN(ret);

    ret = csi_spi_baud(&spi_master, 1000000);
    if(ret == 0){
        return -1;
    }

    /* Select salve 0 of master SPI */
    csi_spi_select_slave(&spi_master, 0);

    /* Initialize slave SPI */
    ret = csi_spi_init(&spi_slave, EXAMPLE_SPI_SLAVE_IDX);
    SPI_CHECK_RETURN(ret);

    ret = csi_spi_attach_callback(&spi_slave, slave_spi_event_callback, NULL);
    SPI_CHECK_RETURN(ret);

    ret = csi_spi_mode(&spi_slave, SPI_SLAVE);
    SPI_CHECK_RETURN(ret);

    ret = csi_spi_cp_format(&spi_slave, SPI_FORMAT_CPOL1_CPHA0);
    SPI_CHECK_RETURN(ret);

    ret = csi_spi_frame_len(&spi_slave, SPI_FRAME_LEN_8);
    SPI_CHECK_RETURN(ret);

    ret = csi_spi_baud(&spi_slave, 1000000);
    if(ret == 0){
        return -1;
    }

    /* Slave SPI start receive */
    spi_slave_event = -1;
    ret = csi_spi_receive_async(&spi_slave, rx_data, 11);
    SPI_CHECK_RETURN(ret);

    /* Master SPI send data */
    ret = csi_spi_send_async(&spi_master, "hello word", 11);
    printf("send: %s\n", "hello word");
    SPI_CHECK_RETURN(ret);

    /* Wait transfer complete */
    timestart = csi_tick_get();
    while (( spi_slave_event != SPI_EVENT_RECEIVE_COMPLETE  ) || ( spi_master_event != SPI_EVENT_SEND_COMPLETE )) {
        if ((csi_tick_get() - timestart) > 5000) {
            return -1;
        }
    }
    printf("receive: %s\n", rx_data);

    if (0 != strcmp((char *)rx_data, "hello word")) {
        return -1;
    }

    /* Uninit SPI */
    csi_spi_detach_callback(&spi_master);

    csi_spi_detach_callback(&spi_slave);

    ret = csi_spi_link_dma(&spi_master, NULL, NULL);
    SPI_CHECK_RETURN(ret);

    csi_spi_uninit(&spi_master);
    csi_spi_uninit(&spi_slave);
    return 0;
}
```
