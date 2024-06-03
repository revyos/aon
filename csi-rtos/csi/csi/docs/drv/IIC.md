

# IIC
-----

## 说明

IIC（Inter Integrated Circuit）总线是 PHILIPS 公司开发的一种半双工、双向二线制同步串行总线。IIC 总线包含两根信号线，双向数据线 SDA，时钟线 SCL）。

## 接口列表

IIC的CSI接口如下所示：

| 函数                         | 说明                                          |
| ---------------------------- | --------------------------------------------- |
| csi_iic_init                 | IIC设备初始化                                 |
| csi_iic_uninit               | IIC设备反初始化                               |
| csi_iic_mode                 | IIC设备设置主从模式                           |
| csi_iic_addr_mode            | IIC设备设置地址模式（7位地址模式/10位地址模式 |
| csi_iic_speed                | IIC设备设置传输速度                           |
| csi_iic_own_addr             | IIC设备设置自身的地址（仅在从机模式下有效）   |
| csi_iic_attach_callback      | 注册回调函数                                  |
| csi_iic_detach_callback      | 注销回调函数                                  |
| csi_iic_link_dma             | 设置IIC设备与DMA设备的连接                    |
| csi_iic_master_send          | IIC设备主机模式下同步发送数据                 |
| csi_iic_master_send_async    | IIC设备主机模式下异步发送数据                 |
| csi_iic_master_receive       | IIC设备主机模式下同步接收数据                 |
| csi_iic_master_receive_async | IIC设备主机模式下异步接收数据                 |
| csi_iic_slave_send           | IIC设备从机模式下同步发送数据                 |
| csi_iic_slave_send_async     | IIC设备从机模式下异步发送数据                 |
| csi_iic_slave_receive        | IIC设备从机模式下同步接收数据                 |
| csi_iic_slave_receive_async  | IIC设备从机模式下异步接收数据                 |
| csi_iic_mem_send             | EEPROM同步发送数据                            |
| csi_iic_mem_receive          | EEPROM同步接收数据                            |
| csi_iic_xfer_pending         | IIC设备是否使能连续的发送                     |
| csi_iic_get_state            | 获取IIC设备的当前读写状态                     |




## 接口详细说明

### csi_iic_init

```c
csi_error_t csi_iic_init(csi_iic_t *iic, uint32_t idx)
```

* 功能描述:

  * 通过设备ID初始化对应的IIC实例。

* 参数:

  * `iic`: 设备句柄（需要用户申请句柄空间）。
  * `idx`: 设备ID。
  
* 返回值:

  * 错误码csi_error_t。
#### csi_iic_t

| 成员              | 类型      | 说明    |
| :------------- | :--------- | :--------- |
| dev | csi_dev_t | csi设备统一句柄     |
| callback | void(*callback)(csi_iic_t *iic, csi_iic_event_t event, void *arg) | 用户回调函数 |
| arg | void * | 回调函数对应传参 |
| data | uint8_t * | 用户传入的数据地址 |
| size | uint32_t | 用户传入的数据长度 |
| mode | csi_iic_mode_t | IIC设备工作模式 |
| tx_dma | csi_dma_ch_t * | 用于发送的DMA通道句柄 |
| rx_dma | csi_dma_ch_t * | 用于接收的DMA通道句柄 |
| send | void * | 用于异步发送的函数指针 |
| receive | void * | 用于异步接收的函数指针 |
| state | csi_state_t | IIC设备的当前状态 |
| priv | void * | 设备私有变量 |

### csi_iic_uninit

```c
void csi_iic_uninit(csi_iic_t *iic)
```

* 功能描述:

  * iic实例反初始化。
  * 该接口会清理并释放相关的软硬件资源。
* 参数:

  * `iic`: 实例句柄。
* 返回值：
  * 无。

### csi_iic_mode

```c
csi_error_t csi_iic_mode(csi_iic_t *iic, csi_iic_mode_t mode)
```

* 功能描述:

  * 设置iic主/从机模式。

* 参数:

  * `iic`: 实例句柄。
  * `mode`:主从模式。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_csi_t g_iic;
  csi_error_t ret;
  ret = csi_iic_mode(&g_iic, IIC_MODE_MASTER);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

#### csi_iic_mode_t

| 类型            | 说明        |
| --------------- | ----------- |
| IIC_MODE_MASTER | IIC主机模式 |
| IIC_MODE_SLAVE  | IIC从机模式 |

### csi_iic_addr_mode

```c
csi_error_t csi_iic_addr_mode(csi_iic_t *iic, csi_iic_addr_mode_t addr_mode)
```

* 功能描述:

  * iic  设置地址模式。
  
* 参数：

  * `iic`: 实例句柄。
  * `addr_mode`:地址模式设置。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_csi_t g_iic;
  csi_error_t ret;
  ret = csi_iic_addr_mode(&g_iic, IIC_ADDRESS_7BIT);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

#### csi_iic_addr_mode_t

| 类型              | 说明         |
| :---------------- | :----------- |
| IIC_ADDRESS_7BIT  | 7位地址模式  |
| IIC_ADDRESS_10BIT | 10位地址模式 |

### csi_iic_speed

```c
csi_error_t csi_iic_speed(csi_iic_t *iic, csi_iic_speed_t speed)
```

* 功能描述:

  * 设置iic传输速度。
  
* 参数：
  * `iic`: 实例句柄。
  * `speed`:传输速度设置。
  
* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_csi_t g_iic;
  csi_error_t ret;
  ret = csi_iic_speed(&g_iic, IIC_BUS_SPEED_STANDARD);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

#### csi_iic_speed_t

| 类型 | 说明     |
| :------------- | :--------- |
| IIC_BUS_SPEED_STANDARD | standard Speed (<=100kHz) |
| IIC_BUS_SPEED_FAST | fast Speed   (<=400kHz) |
| IIC_BUS_SPEED_FAST_PLUS | fast+ Speed  (<= 1MHz) |
| IIC_BUS_SPEED_HIGH | high Speed   (<=3.4MHz) |

* * * * *

### csi_iic_own_addr

```c
csi_error_t csi_iic_own_addr(csi_iic_t *iic, uint32_t own_addr)
```

* 功能描述:

  * 从机模式下，IIC 设置自身地址。

* 参数:

  * `iic`: 实例句柄。
  * `own_addr`: 从机模式下，设置设备响应地址。

* 返回值:

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_csi_t g_iic;
  csi_error_t ret;
  ret = csi_iic_own_addr(&g_iic, 0x50);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_iic_attach_callback

```c
csi_error_t csi_iic_attach_callback(csi_iic_t *iic, void *callback, void *arg)
```

* 功能描述：
  * 注册回调函数。
* 参数：
  * `iic`:实例句柄。
  * `callback`：iic实例的事件回调函数（一般在中断上下文执行）。
  * `arg`: 回调函数参数（可选，由用户定义）。
* 返回值：
  * 错误码csi_error_t。

#### callback

```c
void (*callback)(csi_iic_t *iic, csi_iic_event_t event, void *arg)
```

其中 iic 为设备句柄，event 为传给回调函数的事件类型，arg 为用户自定义的回调函数对应的参数。 

iic  回调事件枚举类型csi_iic_event_t定义如下：

| 事件类型                    | 事件说明                 |
| :------------------------- | ------------------------ |
| IIC_EVENT_SEND_COMPLETE    | 数据发送完成事件          |
| IIC_EVENT_RECEIVE_COMPLETE | 数据接收完成事件          |
| IIC_EVENT_ERROR_OVERFLOW   | IIC总线产生FIFO溢出事件   |
| IIC_EVENT_ERROR_UNDERFLOW  | IIC总线产生FIFO下溢事件   |
| IIC_EVENT_ERROR            | 总线产生错误              |

>注意：
>在使用异步工作模式前，必须调用本函数来注册回调函数，否则将无法使用异步接口。

### csi_iic_detach_callback

```c
void csi_iic_detach_callback(csi_iic_t *iic)
```

* 功能描述：
  * 注销IIC设备的回调函数，并关闭异步读写功能。
* 参数：
  * `iic`：实例句柄。
* 返回值：
  * 无。

### csi_iic_link_dma

```c
csi_error_t csi_iic_link_dma(csi_iic_t *iic, csi_dma_ch_t *tx_dma, csi_dma_ch_t *rx_dma)
```

* 功能描述：

  * 将DMA通道句柄连入iic句柄中，并打开IIC的DMA发送与接收功能。

* 参数：

  * `iic`：实例句柄。
  * `tx_dma`：用于发送的DMA通道句柄，传NULL时会关闭DMA异步发送功能。
  * `rx_dma`：用于接收的DMA通道句柄，传NULL时会关闭DMA异步发送功能。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_iic_t g_iic;
  csi_error_t ret;
  csi_dma_ch_t g_dma_ch_tx;
  csi_dma_ch_t g_dma_ch_rx;
  /* 为发送和接收设置DMA通道 */
  ret = csi_iic_link_dma(&g_iic, &g_dma_ch_tx, &g_dma_ch_rx);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

  

### csi_iic_master_send

```c
int32_t csi_iic_master_send(csi_iic_t *iic,uint32_t devaddr,const void *data,uint32_t size,uint32_t timeout)
```

* 功能描述:

  * 主机模式下，IIC 以同步模式数据发送。

* 参数:

  * `iic`: 实例句柄。
  * `devaddr`: 从机设备地址。
  * `data`: 待发送数据缓冲区地址。
  * `size`: 待发送数据的长度。
  * `timeout`: 设置主机发送超时时间(在超时时间内，无从机响应，或传输过程中超时)，单位是毫秒。

* 返回值:

  * 成功发送的字节数或错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_iic_t g_iic;
  csi_error_t ret;
  uint32_t devaddr = 0x50;
  int32_t num = 0;
  const uint8_t tx_test_msg[3];
  tx_test_msg[0] = 0x01;
  tx_test_msg[1] = 0x02;
  tx_test_msg[2] = 0x03;
  /* 超时时间请根据实际应用场景进行设置 */
  num = csi_iic_master_send(&g_iic, devaddr, tx_test_msg, sizeof(tx_test_msg), 50);
  if (num != sizeof(tx_test_msg)) {
      return -1;
  }
  ```
  

### csi_iic_master_send_async

```c
csi_error_t csi_iic_master_send_async(csi_iic_t *iic, uint32_t devaddr, void *data, uint32_t size)
```

* 功能描述:

  * 主机模式下，IIC 以异步模式启动数据发送

* 参数:

  * `iic`: 实例句柄
  * `devaddr`: 从机设备地址
  * `data`: 待发送数据缓冲区地址
  * `size`: 待发送数据的长度

* 返回值:

  * 错误码csi_error_t。

>注意：
>在调用此函数前，需要先调用csi_iic_attach_callback来注册回调函数。本函数会根据用户设置的
>不同，进入中断工作模式或DMA工作模式。具体使用方式会在文末用例中综合说明。


### csi_iic_master_receive

```c
int32_t csi_iic_master_receive(csi_iic_t *iic, uint32_t devaddr, void *data, uint32_t size, uint32_t timeout)
```

* 功能描述:

  * 主机模式下，IIC 以同步模式进行数据接收。

* 参数:

  * `iic`: 实例句柄。
  * `devaddr`: 从机设备地址。
  * `data`: 待接收数据缓冲区地址。
  * `size`: 待接收数据的长度。
  * `timeout`: 设置主机接收超时时间(在超时时间内，无从机响应,或传输过程中超时)，单位是毫秒。

* 返回值:

  * 成功接收的字节数或错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_iic_t g_iic;
  csi_error_t ret;
  uint32_t devaddr = 0x50;
  int32_t num = 0;
  const uint8_t rx_test_msg[3];
  /* 超时时间请根据实际应用场景进行设置 */
  num = csi_iic_master_receive(&g_iic, devaddr, rx_test_msg, sizeof(rx_test_msg), 50);
  if (num != sizeof(rx_test_msg)) {
      return -1;
  }
  ```


### csi_iic_master_receive_async

```c
csi_error_t csi_iic_master_receive_async(csi_iic_t *iic, uint32_t devaddr, void *data, uint32_t size)
```

* 功能描述:

  * 主机模式下，IIC 以异步模式进行数据读取。

* 参数:

  * `iic`: 实例句柄。
  * `devaddr`: 从机设备地址。
  * `data`: 待接收数据缓冲区地址。
  * `size`:数据的长度。

* 返回值:

  * 错误码csi_error_t。

>注意：
>在调用此函数前，需要先调用csi_iic_attach_callback来注册回调函数。本函数会根据用户设置的
不同，进入中断工作模式或DMA工作模式。具体使用方式会在文末用例中综合说明

### csi_iic_mem_send

```c
int32_t csi_iic_mem_send(csi_iic_t *iic, uint32_t devaddr,uint16_t memaddr, csi_iic_mem_addr_size_t memaddr_size,const void *data,uint32_t size,uint32_t timeout)
```

* 功能描述:

  * IIC 以同步模式对eeprom写数据。

* 参数:

  * `iic`: 实例句柄。
  * `devaddr`: 从机设备地址。
  * `memaddr`: 待写入的器件存储地址。
  * `memaddr_size`: 器件存储地址大小。
  * `data`: 待写入数据缓冲区地址。
  * `size`: 待写入数据的长度。
  * `timeout`: 超时时间，单位是毫秒。

* 返回值:

  * 成功写入的字节数或错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_iic_t g_iic;
  csi_error_t ret;
  uint32_t devaddr = 0x50;
  uint32_t memaddr = 0x0010;
  int32_t num = 0;
  const uint8_t tx_test_msg[3];
  tx_test_msg[0] = 0x01;
  tx_test_msg[1] = 0x02;
  tx_test_msg[2] = 0x03;
  /* 超时时间请根据实际应用场景进行设置 */
  num = csi_iic_mem_send(&g_iic, devaddr, memaddr, IIC_MEM_ADDR_SIZE_16BIT, tx_test_msg, sizeof(tx_test_msg), 50);
  if (num != sizeof(tx_test_msg)) {
      return -1;
  }
  ```

  #### csi_iic_mem_addr_size_t

  | 类型                    | 说明                   |
  | :---------------------- | :--------------------- |
  | IIC_MEM_ADDR_SIZE_8BIT  | EEPROM的地址模式是8位  |
  | IIC_MEM_ADDR_SIZE_16BIT | EEPROM的地址模式是16位 |

### csi_iic_mem_receive

```c
int32_t csi_iic_mem_receive(csi_iic_t *iic,uint32_t devaddr, uint16_t memaddr,csi_iic_mem_addr_size_t memaddr_size,void *data,uint32_t size,uint32_t timeout)
```

* 功能描述:

  * IIC  以同步模式对eeprom读数据。

* 参数:

  * `iic`: 实例句柄。
  * `devaddr`: 从机设备地址。
  * `memaddr`: 待读取的器件存储地址。
  * `memaddr_size`: 器件存储地址模式。
  * `data`: 待读取数据缓冲区地址。
  * `size`: 待读取数据的长度。
  * `timeout`: 超时时间，单位是毫秒。

* 返回值:

  * 成功读出的字节数或错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_iic_t g_iic;
  csi_error_t ret;
  uint32_t devaddr = 0x50;
  uint32_t memaddr = 0x0010;
  int32_t num = 0;
  const uint8_t rx_test_msg[3];
  /* 超时时间请根据实际应用场景进行设置 */
  num = csi_iic_mem_receive(&g_iic, devaddr, memaddr, IIC_MEM_ADDR_SIZE_16BIT, rx_test_msg, sizeof(rx_test_msg), 50);
  if (num != sizeof(rx_test_msg)) {
      return -1;
  }
  ```

### csi_iic_slave_send

```c
int32_t csi_iic_slave_send(csi_iic_t *iic,const void *data,uint32_t size,uint32_t timeout)
```

* 功能描述:

  * 从机模式下，IIC 以同步模式数据发送。

* 参数:

  * `iic`: 实例句柄。
  * `data`: 待发送数据缓冲区地址。
  * `size`: 待发送数据的长度。
  * `timeout`: 设置从机发送数据超时时间(在超时时间内，无主机对从机读取数据操作，或传输过程中超时)，单位是毫秒。

* 返回值:

  * 成功发送的字节数或错误码csi_error_t。

* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_iic_t g_iic;
  csi_error_t ret;
  int32_t num = 0;
  const uint8_t tx_test_msg[3];
  tx_test_msg[0] = 0x01;
  tx_test_msg[1] = 0x02;
  tx_test_msg[2] = 0x03;
  /* 超时时间请根据实际应用场景进行设置 */
  num = csi_iic_slave_send(&g_iic, tx_test_msg, sizeof(tx_test_msg), 50);
  if (num != sizeof(tx_test_msg)) {
      return -1;
  }
  ```


### csi_iic_slave_send_async

```c
csi_error_t csi_iic_slave_send_async(csi_iic_t *iic, void *data, uint32_t size)
```

* 功能描述:

  * 从机模式下，IIC 以异步模式启动数据发送。

* 参数:

  * `iic`: 实例句柄。
  * `data`: 待发送数据缓冲区地址。
  * `size`: 待发送数据的长度。

* 返回值:

  * 错误码csi_error_t。

>注意：
>在调用此函数前，需要先调用csi_iic_attach_callback来注册回调函数。本函数会根据用户设置的
>不同，进入中断工作模式或DMA工作模式。具体使用方式会在文末用例中综合说明。

### csi_iic_slave_receive

```c
int32_t csi_iic_slave_receive(csi_iic_t *iic, void *data, uint32_t size, uint32_t timeout)
```

* 功能描述:

  * 从机模式下，IIC 以同步模式进行数据接收。

* 参数:

  * `iic`: 实例句柄。
  * `data`: 待接收数据缓冲区地址。
  * `size`: 待接收数据的长度。
  * `timeout`: 设置从机接收超时时间(在超时时间内，无主机对从机进行写数据操作，或传输过程中超时)，单位是毫秒。

* 返回值:

  * 成功接收的字节数或错误码csi_error_t。

* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_iic_t g_iic;
  csi_error_t ret;
  uint32_t devaddr = 0x50;
  int32_t num = 0;
  const uint8_t rx_test_msg[3];
  /* 超时时间请根据实际应用场景进行设置 */
  num = csi_iic_slave_receive(&g_iic, rx_test_msg, sizeof(rx_test_msg), 50);
  if (num != sizeof(rx_test_msg)) {
      return -1;
  }
  ```


### csi_iic_slave_receive_async

```c
csi_error_t csi_iic_slave_receive_async(csi_iic_t *iic, void *data, uint32_t size)
```

* 功能描述:

  * 从机模式下，IIC 以异步模式进行数据读取。

* 参数:

  * `iic`: 实例句柄。
  * `data`: 待接收数据缓冲区地址。
  * `size`:数据的长度。

* 返回值:

  * 错误码csi_error_t。

>注意：
>在调用此函数前，需要先调用csi_iic_attach_callback来注册回调函数。本函数会根据用户设置的
>不同，进入中断工作模式或DMA工作模式。具体使用方式会在文末用例中综合说明



### csi_iic_xfer_pending

```c
csi_error_t csi_iic_xfer_pending(csi_iic_t *iic, bool enable)
```

* 功能描述：

  * IIC是否使能连续的发送。

* 参数：

  * `iic`: 实例句柄。
  * `enable`: 是否使能连续的发送。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_iic_t g_iic;
  csi_error_t ret;
  ret = csi_iic_xfer_pending(&g_iic, true);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_iic_get_state

```c
csi_error_t csi_iic_get_state(csi_iic_t *iic, csi_state_t *state)
```

* 功能描述;
  * 获取`iic`的状态。通过此函数来判断IIC设备在获取状态的时刻是否可以进行send和receive操作。
* 参数：
  * iic：实例句柄。
  * state: 用于返回状态信息的参数地址。
* 返回值：
  * 错误码csi_error_t。

## 初始化示例

```c
/* 句柄空间一般使用静态空间 */
static csi_iic_t g_iic;

int main(void) {
    csi_error_t ret;
    /* init函数的idx参数，请根据soc的实际情况进行选择 */
    ret = csi_iic_init(&g_iic, 0);
    if (ret != CSI_OK) {
        return -1;
    }
}
```

## 同步模式使用示例

#### 主机模式发送接收使用示例

```c
#include "stdio.h"
#include "drv/iic.h"

#define IIC_SLAVE_ADDR      0x50
#define IIC_IDX             0
static csi_iic_t master_iic;

int example_iic(void)
{
  uint8_t write_data[10];
  uint8_t read_data[10];
  csi_error_t ret;
  uint32_t i;
  int32_t num;
  ret = csi_iic_init(&master_iic, IIC_IDX);
  if (ret != CSI_OK) {
	printf("csi_iic_initialize error\n");  
    return -1;
  }
  /* config iic master mode */  
  ret = csi_iic_mode(&master_iic, IIC_MODE_MASTER);
  if (ret != CSI_OK) {
      printf("csi_iic_set_mode error\n");
      return -1;
  }
  /* config iic 7bit address mode */  
  ret = csi_iic_addr_mode(&master_iic, IIC_ADDRESS_7BIT);
  if (ret != CSI_OK) {
      printf("csi_iic_set_addr_mode error\n");
      return -1;
  }
  /* config iic standard speed*/  
  ret = csi_iic_speed(&master_iic, IIC_BUS_SPEED_STANDARD);
  if (ret != CSI_OK) {
      printf("csi_iic_set_speed error\n");
      return -1;
  }

  for (i = 0; i < sizeof(write_data); i++) { 
      write_data[i] = i;           ///< init write_data value
  }
 
  num = csi_iic_master_send(&master_iic, IIC_SLAVE_ADDR, write_data, sizeof(write_data), 100000);
  if (num != sizeof(write_data)) {
      printf("csi_iic_master_send error\n");
      return -1;
  }
    
  num = csi_iic_master_receive(&master_iic, IIC_SLAVE_ADDR, read_data, sizeof(read_data), 100000);
  if (num != sizeof(read_data)) {
      printf("csi_iic_master_receive error\n");
      return -1;
    }
  csi_iic_uninit(&master_iic);
  return 0;
}
```

>注意：
>本示例包含了IIC的主机初始化，配置，主机同步收发接口的调用示例。实际使用还需要GPIO的初始化，以及和从设备的上拉连接，主机收发需要从机配合读取。

#### 从机模式发送接收使用示例

```c
#include "stdio.h"
#include "drv/iic.h"

#define IIC_OWN_ADDR      0x50
#define IIC_IDX           0
static csi_iic_t slave_iic;

int example_iic(void)
{
  uint8_t write_data[10];
  uint8_t read_data[10];
  csi_error_t ret;
  uint32_t i;
  int32_t num;
  ret = csi_iic_init(&slave_iic, IIC_IDX);
  if (ret != CSI_OK) {
	printf("csi_iic_initialize error\n");  
    return -1;
  }
  /* config iic slave mode */  
  ret = csi_iic_mode(&slave_iic, IIC_MODE_SLAVE);
  if (ret != CSI_OK) {
      printf("csi_iic_set_mode error\n");
      return -1;
  }
  /* config iic 7bit address mode */  
  ret = csi_iic_addr_mode(&slave_iic, IIC_ADDRESS_7BIT);
  if (ret != CSI_OK) {
      printf("csi_iic_set_addr_mode error\n");
      return -1;
  }
  /* config iic standard speed*/  
  ret = csi_iic_speed(&slave_iic, IIC_BUS_SPEED_STANDARD);
  if (ret != CSI_OK) {
      printf("csi_iic_set_speed error\n");
      return -1;
  }
  /* config iic slave own address*/  
  ret = csi_iic_own_addr(&slave_iic, IIC_OWN_ADDR);
  if (ret != CSI_OK) {
      printf("csi_iic_own_addr error\n");
      return -1;
  }

  for (i = 0; i < sizeof(write_data); i++) { 
      write_data[i] = i;           ///< init write_data value
  }

  num = csi_iic_salve_send(&slave_iic, write_data, sizeof(write_data), 100000);
  if (num != sizeof(write_data)) {
      printf("csi_iic_salve_send error\n");
      return -1;
  }
    
  num = csi_iic_salve_receive(&slave_iic, read_data, sizeof(read_data), 100000);
  if (num != sizeof(read_data)) {
      printf("csi_iic_salve_receive error\n");
      return -1;
    }
  csi_iic_uninit(&salve_iic);
  return 0;
}
```

>注意：
>本示例包含了IIC的从机初始化，配置，从机同步收发接口的调用示例。实际使用还需要GPIO的初始化，以及和主设备的上拉连接，从机发送需要主机配合读取。

#### EEPROM读写使用示例

```c
#include "stdio.h"
#include "drv/iic.h"

#define IIC_SLAVE_ADDR      0x50
#define IIC_MEM_ADDR        0x0010
#define IIC_IDX             0
static csi_iic_t master_iic;

int example_iic(void)
{
  uint8_t write_data[10];
  uint8_t read_data[10];
  csi_error_t ret;
  uint32_t i;
  int32_t num;
  ret = csi_iic_init(&master_iic, IIC_IDX);
  if (ret != CSI_OK) {
	printf("csi_iic_initialize error\n");  
    return -1;
  }
  /* config iic master mode */  
  ret = csi_iic_mode(&master_iic, IIC_MODE_MASTER);
  if (ret != CSI_OK) {
      printf("csi_iic_set_mode error\n");
      return -1;
  }
  /* config iic 7bit address mode */  
  ret = csi_iic_addr_mode(&master_iic, IIC_ADDRESS_7BIT);
  if (ret != CSI_OK) {
      printf("csi_iic_set_addr_mode error\n");
      return -1;
  }
  /* config iic standard speed*/  
  ret = csi_iic_speed(&master_iic, IIC_BUS_SPEED_STANDARD);
  if (ret != CSI_OK) {
      printf("csi_iic_set_speed error\n");
      return -1;
  }

  for (i = 0; i < sizeof(write_data); i++) { 
      write_data[i] = i;           ///< init write_data value
  }

  num = csi_iic_mem_send(&master_iic, IIC_SLAVE_ADDR, IIC_MEM_ADDR, IIC_MEM_ADDR_SIZE_16BIT, write_data, sizeof(write_data), 100000);
  if (num != sizeof(write_data)) {
      printf("csi_iic_mem_send error\n");
      return -1;
  }
    
  num = csi_iic_mem_receive(&master_iic, IIC_SLAVE_ADDR, IIC_MEM_ADDR, IIC_MEM_ADDR_SIZE_16BIT, read_data, sizeof(read_data), 100000);
  if (num != sizeof(read_data)) {
      printf("csi_iic_mem_receive error\n");
      return -1;
    }
  csi_iic_uninit(&master_iic);
  return 0;
}
```

>注意：
>本示例包含了IIC的主机初始化，配置，EEPROM同步收发接口的调用示例。实际使用还需要GPIO的初始化，以及和EEPROM的上拉连接，本示例选用的是16bit地址宽度的EEPROM。

## 异步模式使用示例

>中断模式和DMA模式的使用流程基本一致，差别仅在是否调用了csi_iic_link_dma接口。

#### DMA模式的主机收发使用示例

```c
#include "stdio.h"
#include "drv/iic.h"

#define IIC_SLAVE_ADDR      0x50
#define IIC_IDX             0
static csi_iic_t master_iic;
static csi_dma_ch_t dma_ch_tx_handle;
static volatile uint8_t cb_transfer_flag = 0;
static void iic_event_cb_fun(csi_iic_t *iic, csi_iic_event_t event, void *arg)
{
    if (event == IIC_EVENT_RECEIVE_COMPLETE) {
        cb_transfer_flag = 1;
    }

    if (event == IIC_EVENT_SEND_COMPLETE) {
        cb_transfer_flag = 1;
    }
}

int example_iic(void)
{
  uint8_t write_data[10];
  uint8_t read_data[10];
  csi_error_t ret;
  uint32_t i;
  ret = csi_iic_init(&master_iic, IIC_IDX);
  if (ret != CSI_OK) {
	printf("csi_iic_initialize error\n");  
    return -1;
  }
  /* iic attach callback */  
  ret = csi_iic_attach_callback(&master_iic, iic_event_cb_fun, NULL);
  if (ret != CSI_OK) {
      printf("csi_iic_attach_callback error\n");
      return -1;
  }
    
  /* config iic master mode */  
  ret = csi_iic_mode(&master_iic, IIC_MODE_MASTER);
  if (ret != CSI_OK) {
      printf("csi_iic_set_mode error\n");
      return -1;
  }
  /* config iic 7bit address mode */  
  ret = csi_iic_addr_mode(&master_iic, IIC_ADDRESS_7BIT);
  if (ret != CSI_OK) {
      printf("csi_iic_set_addr_mode error\n");
      return -1;
  }
  /* config iic standard speed*/  
  ret = csi_iic_speed(&master_iic, IIC_BUS_SPEED_STANDARD);
  if (ret != CSI_OK) {
      printf("csi_iic_set_speed error\n");
      return -1;
  }
  
  /* use this interface connect iic tx with dma */  
  ret = csi_iic_link_dma(&master_iic, &dma_ch_tx_handle, NULL);    
  if (ret != CSI_OK) {
        printf("csi_iic_link_dma fail \n");
        return -1;
  }

  for (i = 0; i < sizeof(write_data); i++) { 
      write_data[i] = i;           ///< init write_data value
  }

  ret = csi_iic_master_send_async(&master_iic, IIC_SLAVE_ADDR, write_data, sizeof(write_data), 100000);
  if (ret != CSI_OK) {
      printf("csi_iic_master_send error\n");
      return -1;
  }
    
  ret = csi_iic_master_receive_async(&master_iic, IIC_SLAVE_ADDR, read_data, sizeof(read_data), 100000);
  if (ret != CSI_OK) {
      printf("csi_iic_master_receive error\n");
      return -1;
    }
  /* use this interface disconnect iic with dma */ 
  csi_iic_link_dma(&master_iic, NULL, NULL);
  /* use this interface disconnect iic callback */ 
  csi_iic_detach_callback(&master_iic);
  csi_iic_uninit(&master_iic);
  return 0;
}
```



#### DMA模式的从机收发使用示例

```c
#include "stdio.h"
#include "drv/iic.h"

#define IIC_OWN_ADDR      0x50
#define IIC_IDX           0
static csi_iic_t slave_iic;
static csi_dma_ch_t dma_ch_rx_handle;
static volatile uint8_t slave_cb_transfer_done_flag = 0;
static void slave_iic_event_cb_fun(csi_iic_t *iic, csi_iic_event_t event, void *arg)
{
    if (event == IIC_EVENT_RECEIVE_COMPLETE) {
        slave_cb_transfer_done_flag = 1;
    }

    if (event == IIC_EVENT_SEND_COMPLETE) {
        slave_cb_transfer_done_flag = 1;
    }
}

int example_iic(void)
{
  uint8_t write_data[10];
  uint8_t read_data[10];
  csi_error_t ret;
  uint32_t i;
  ret = csi_iic_init(&slave_iic, IIC_IDX);
  if (ret != CSI_OK) {
	printf("csi_iic_initialize error\n");  
    return -1;
  }
  /* iic attach callback */   
  ret = csi_iic_attach_callback(&slave_iic, slave_iic_event_cb_fun, NULL);
  if (ret != CSI_OK) {
      printf("csi_iic_attach_callback error\n");
      return -1;
  }
  /* config iic slave mode */  
  ret = csi_iic_mode(&slave_iic, IIC_MODE_SLAVE);
  if (ret != CSI_OK) {
      printf("csi_iic_set_mode error\n");
      return -1;
  }
  /* config iic 7bit address mode */  
  ret = csi_iic_addr_mode(&slave_iic, IIC_ADDRESS_7BIT);
  if (ret != CSI_OK) {
      printf("csi_iic_set_addr_mode error\n");
      return -1;
  }
  /* config iic standard speed*/  
  ret = csi_iic_speed(&slave_iic, IIC_BUS_SPEED_STANDARD);
  if (ret != CSI_OK) {
      printf("csi_iic_set_speed error\n");
      return -1;
  }
  /* config iic slave own address*/  
  ret = csi_iic_own_addr(&slave_iic, IIC_OWN_ADDR);
  if (ret != CSI_OK) {
      printf("csi_iic_own_addr error\n");
      return -1;
  }
  /* use this interface connect iic rx with dma */  
  ret = csi_iic_link_dma(&slave_iic, NULL, &dma_ch_rx_handle);    
  if (ret != CSI_OK) {
        printf("csi_iic_link_dma fail \n");
        return -1;
  }
  for (i = 0; i < sizeof(write_data); i++) { 
      write_data[i] = i;           ///< init write_data value
  }

  ret = csi_iic_salve_send(&slave_iic, write_data, sizeof(write_data), 100000);
  if (ret != CSI_OK) {
      printf("csi_iic_salve_send error\n");
      return -1;
  }
    
  ret = csi_iic_salve_receive(&slave_iic, read_data, sizeof(read_data), 100000);
  if (ret != CSI_OK) {
      printf("csi_iic_salve_receive error\n");
      return -1;
  }
  /* use this interface disconnect iic with dma */ 
  csi_iic_link_dma(&slave_iic, NULL, NULL);
  /* use this interface disconnect iic callback */ 
  csi_iic_detach_callback(&slave_iic);
  csi_iic_uninit(&salve_iic);
  return 0;
}
```



#### 中断模式的主机收发使用示例

```c
#include "stdio.h"
#include "drv/iic.h"

#define IIC_SLAVE_ADDR      0x50
#define IIC_IDX             0
static csi_iic_t master_iic;
static volatile uint8_t cb_transfer_flag = 0;
static void iic_event_cb_fun(csi_iic_t *iic, csi_iic_event_t event, void *arg)
{
    if (event == IIC_EVENT_RECEIVE_COMPLETE) {
        cb_transfer_flag = 1;
    }

    if (event == IIC_EVENT_SEND_COMPLETE) {
        cb_transfer_flag = 1;
    }

}

int example_iic(void)
{
  uint8_t write_data[10];
  uint8_t read_data[10];
  csi_error_t ret;
  uint32_t i;
  ret = csi_iic_init(&master_iic, IIC_IDX);
  if (ret != CSI_OK) {
	printf("csi_iic_initialize error\n");  
    return -1;
  }
  /* iic attach callback */  
  ret = csi_iic_attach_callback(&master_iic, iic_event_cb_fun, NULL);
  if (ret != CSI_OK) {
      printf("csi_iic_attach_callback error\n");
      return -1;
  }
    
  /* config iic master mode */  
  ret = csi_iic_mode(&master_iic, IIC_MODE_MASTER);
  if (ret != CSI_OK) {
      printf("csi_iic_set_mode error\n");
      return -1;
  }
  /* config iic 7bit address mode */  
  ret = csi_iic_addr_mode(&master_iic, IIC_ADDRESS_7BIT);
  if (ret != CSI_OK) {
      printf("csi_iic_set_addr_mode error\n");
      return -1;
  }
  /* config iic standard speed*/  
  ret = csi_iic_speed(&master_iic, IIC_BUS_SPEED_STANDARD);
  if (ret != CSI_OK) {
      printf("csi_iic_set_speed error\n");
      return -1;
  }

  for (i = 0; i < sizeof(write_data); i++) { 
      write_data[i] = i;           ///< init write_data value
  }

  ret = csi_iic_master_send_async(&master_iic, IIC_SLAVE_ADDR, write_data, sizeof(write_data), 100000);
  if (ret != CSI_OK) {
      printf("csi_iic_master_send error\n");
      return -1;
  }
    
  ret = csi_iic_master_receive_async(&master_iic, IIC_SLAVE_ADDR, read_data, sizeof(read_data), 100000);
  if (ret != CSI_OK) {
      printf("csi_iic_master_receive error\n");
      return -1;
    }
  /* use this interface disconnect iic callback */ 
  csi_iic_detach_callback(&master_iic);
  csi_iic_uninit(&master_iic);
  return 0;
}
```



#### 中断模式的从机收发使用示例

```c
#include "stdio.h"
#include "drv/iic.h"

#define IIC_OWN_ADDR      0x50
#define IIC_IDX           0
static csi_iic_t slave_iic;
static volatile uint8_t slave_cb_transfer_done_flag = 0;
static void slave_iic_event_cb_fun(csi_iic_t *iic, csi_iic_event_t event, void *arg)
{
    if (event == IIC_EVENT_RECEIVE_COMPLETE) {
        slave_cb_transfer_done_flag = 1;
    }

    if (event == IIC_EVENT_SEND_COMPLETE) {
        slave_cb_transfer_done_flag = 1;
    }

}

int example_iic(void)
{
  uint8_t write_data[10];
  uint8_t read_data[10];
  csi_error_t ret;
  uint32_t i;
  ret = csi_iic_init(&slave_iic, IIC_IDX);
  if (ret != CSI_OK) {
	printf("csi_iic_initialize error\n");  
    return -1;
  }
  /* iic attach callback */   
  ret = csi_iic_attach_callback(&slave_iic, slave_iic_event_cb_fun, NULL);
  if (ret != CSI_OK) {
      printf("csi_iic_attach_callback error\n");
      return -1;
  }
  /* config iic slave mode */  
  ret = csi_iic_mode(&slave_iic, IIC_MODE_SLAVE);
  if (ret != CSI_OK) {
      printf("csi_iic_set_mode error\n");
      return -1;
  }
  /* config iic 7bit address mode */  
  ret = csi_iic_addr_mode(&slave_iic, IIC_ADDRESS_7BIT);
  if (ret != CSI_OK) {
      printf("csi_iic_set_addr_mode error\n");
      return -1;
  }
  /* config iic standard speed*/  
  ret = csi_iic_speed(&slave_iic, IIC_BUS_SPEED_STANDARD);
  if (ret != CSI_OK) {
      printf("csi_iic_set_speed error\n");
      return -1;
  }
  /* config iic slave own address*/  
  ret = csi_iic_own_addr(&slave_iic, IIC_OWN_ADDR);
  if (ret != CSI_OK) {
      printf("csi_iic_own_addr error\n");
      return -1;
  }
  for (i = 0; i < sizeof(write_data); i++) { 
      write_data[i] = i;           ///< init write_data value
  }

  ret = csi_iic_salve_send(&slave_iic, write_data, sizeof(write_data), 100000);
  if (ret != CSI_OK) {
      printf("csi_iic_salve_send error\n");
      return -1;
  }
    
  ret = csi_iic_salve_receive(&slave_iic, read_data, sizeof(read_data), 100000);
  if (ret != CSI_OK) {
      printf("csi_iic_salve_receive error\n");
      return -1;
  }
  /* use this interface disconnect iic callback */ 
  csi_iic_detach_callback(&slave_iic);
  csi_iic_uninit(&salve_iic);
  return 0;
}
```

