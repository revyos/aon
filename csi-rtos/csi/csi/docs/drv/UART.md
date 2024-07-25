# UART设备
-----

## 说明

UART(Universal Asynchronous Receiver/Transmitter)是一种同步或者异步的串行通信总线接口，发送和接收方按照严格的格式（波特率和数据帧格式）发送和接收。

UART的特点：

- 空闲时总线保持高电平状态
- 5~9位数据位，低位在前
- 一个起始位
- 可选奇偶检验位
- 可选1、1.5、2比特的停止位


## 接口列表

UART的CSI接口如下所示：

| 函数               | 说明                |
| :---------------- | :------------------ |
| csi_uart_init     | UART设备初始化        |
| csi_uart_uninit   | UART设备反初始化     |
| csi_uart_baud     | 设置波特率            |
| csi_uart_format | 设置数据位，停止位，校验位 |
| csi_uart_flowctrl | 设置流控              |
| csi_uart_attach_callback | 注册回调函数 |
| csi_uart_detach_callback | 注销回调函数 |
| csi_uart_link_dma | 设置与DMA设备的连接     |
| csi_uart_send |  同步发送数据              |
| csi_uart_send_async | 异步发送数据         |
| csi_uart_receive | 同步接收数据            |
| csi_uart_receive_async | 异步接收数据      |
| csi_uart_putc |  发送字节（同步）           |
| csi_uart_getc |  接收字节（同步）           |
| csi_uart_get_state | 获取UART设备的当前的读写状态 |

## 接口详细说明

### csi_uart_init

```c
csi_error_t csi_uart_init(csi_uart_t *uart, uint32_t idx);
```

* 功能描述:

  * 通过设备号初始化对应的uart实例。

* 参数:

  * `uart`: 设备句柄（需要用户申请句柄空间）
  * `idx`: 设备号

* 返回值:

  * 错误码csi_error_t

#### csi_uart_t

| 成员              | 类型      | 说明    |
| :------------- | :--------- | :--------- |
| dev | csi_dev_t | csi设备统一句柄     |
| callback | void (*callback)(csi_uart_t *uart, csi_uart_event_t event, void *arg) | 用户回调函数   |
| arg | void* | 回调函数参数（用户自定义）    |
| tx_data | uint8_t* | 发送缓存区地址      |
| tx_size | uint32_t | 发送数据字节数 |
| rx_data | uint8_t* | 接收缓存区地址 |
| rx_size | uint32_t | 接收数据字节数 |
| tx_dma | csi_dma_ch_t | 用于发送的DMA通道句柄 |
| rx_dma | csi_dma_ch_t | 用于接收的DMA通道句柄 |
| send | csi_error_t (*send)(csi_uart_t *uart, const void *data, uint32_t size) | 异步发送函数指针 |
| receive | csi_error_t (*receive )(csi_uart_t *uart, void *data, uint32_t size) | 异步接收函数指针 |
| state | csi_state_t | UART设备读写状态 |
| priv | void* | 设备私有变量               |



### csi_uart_uninit

```c
void csi_uart_uninit(csi_uart_t *uart);
```

* 功能描述:

  * uart实例反初始化。
  * 该接口会停止uart实例正在进行的传输，并且释放相关的软硬件资源。

* 参数:

  * `uart`: 实例句柄。

### csi_uart_baud

```c
csi_error_t csi_uart_baud(csi_uart_t *uart, uint32_t baud);
```
* 功能描述:

  * uart设备配置波特率

* 参数

  * `uart` : 实例句柄
  * `baud` : 波特率

* 返回值:

  * 错误码csi_error_t
  
* 使用示例：

```c
/* 句柄使用前请先初始化 */
static csi_uart_t g_uart;
csi_error_t ret;
ret = csi_uart_baud(&g_uart, 115200);
if (ret != CSI_OK) {
    return -1;
}
```

### csi_uart_format

```c
csi_error_t csi_uart_format(csi_uart_t *uart,  csi_uart_data_bits_t data_bits, csi_uart_parity_t parity, csi_uart_stop_bits_t stop_bits);
```
* 功能描述:
  
  * uart设备配置数据位，奇偶校验位和停止位

* 参数:

  * `uart` : 实例句柄
  * `data_bits`：数据位
  * `parity`：校验位
  * `stop_bits`：停止位

* 返回值:

  * 错误码csi_error_t
  
* 使用示例:

```c
/* 句柄使用前请先初始化 */
static csi_uart_t g_uart;
csi_error_t ret;
ret = csi_uart_format(&g_uart, UART_DATA_BITS_8, UART_PARITY_NONE, UART_STOP_BITS_1);
if (ret != CSI_OK) {
    return -1;
}
```

* * * * *
#### csi_uart_data_bits_t

| 类型 | 说明     |
| :------------- | :--------- |
| UART_DATA_BITS_5 | 5位数据位宽 |
| UART_DATA_BITS_6 | 6位数据位宽 |
| UART_DATA_BITS_7 | 7位数据位宽 |
| UART_DATA_BITS_8 | 8位数据位宽 |
| UART_DATA_BITS_9 | 9位数据位宽 |

* * * * *
#### csi_uart_parity_t

| 类型 | 说明     |
| :------------- | :--------- |
| UART_PARITY_NONE | 无校验 |
| UART_PARITY_EVEN | 偶校验 |
| UART_PARITY_ODD | 奇校验 |

* * * * *
#### csi_uart_stop_bits_t

| 类型 | 说明     |
| :------------- | :--------- |
| UART_STOP_BITS_1 | 1停止位 |
| UART_STOP_BITS_2 | 2停止位 |
| UART_STOP_BITS_1_5 | 1.5停止位 |

* * * * *
### csi_uart_flowctrl

```c
csi_error_t csi_uart_flowctrl(csi_uart_t *uart,  csi_uart_flowctrl_t flowctrl);
```
* 功能描述:

  * 设置uart设备的流控功能。

* 参数:

  * `uart`: 实例句柄。
  * `flowctrl`: 流控模式

* 返回值:

  * 错误码csi_error_t

#### csi_uart_flowctrl_t

| 类型 | 说明     |
| :------------- | :--------- |
| UART_FLOWCTRL_NONE | 无流控 |
| UART_FLOWCTRL_RTS | 发送请求（Require ToSend）|
| UART_FLOWCTRL_CTS | 发送允许（Clear ToSend）|
| UART_FLOWCTRL_RTS_CTS | 发送请求与发送允许功能同时打开 |

> 注意：
> 由于并不是每个UART设备都支持流控功能，因此设备默认为UART_FLOWCTRL_NONE，
> 不需要特地调用csi_uart_flowctrl将uart设置为UART_FLOWCTRL_NONE。

### csi_uart_attach_callback

```c
csi_error_t csi_uart_attach_callback(csi_uart_t *uart, void *callback, void *arg);
```

* 功能描述:

  * 设置回调函数，并打开中断的异步模式读写功能

* 参数:

  * `uart`: 实例句柄。
  * `callback`: uart实例的事件回调函数（一般在中断上下文执行）。
  * `arg`: 回调函数参数（可选，由用户定义）。

* 返回值:

  * 错误码csi_error_t。

####  callback

```c
void (*callback)(csi_uart_t *uart, csi_uart_event_t event, void *arg);
```

其中uart为设备句柄，idx为设备号，event 为传给回调函数的事件类型，arg为用户自定义的回调函数对应的参数。 uart 回调事件枚举类型csi_uart_event_t定义如下：

| 事件类型                           | 事件说明                 |
| :--------------------------------- | ------------------------ |
| UART_EVENT_SEND_COMPLETE           | 数据发送完成事件         |
| UART_EVENT_RECEIVE_COMPLETE        | 数据接收完成事件         |
| UART_EVENT_RECEIVE_FIFO_READABLE   | FIFO残留数据等待接收事件 |
| UART_ENENT_BREAK_INTR              | 数据接收中断事件         |
| UART_EVENT_ERROR_OVERFLOW          | 数据接收溢出事件         |
| UART_EVENT_ERROR_PARITY            | 数据校验位错误事件       |
| UART_EVENT_ERROR_FRAMING           | 数据无有效停止位事件     |

>注意：
>在使用异步工作模式前，必须调用本函数来注册回调函数，否则将无法使用异步接口。


### csi_uart_detach_callback

```c
void csi_uart_detach_callback(csi_uart_t *uart);
```

* 功能描述:

  * 注销UART设备的回调函数，并关闭异步读写功能。

* 参数:

  * `uart`: 实例句柄。

### csi_uart_link_dma

```c
csi_error_t csi_uart_link_dma(csi_uart_t *uart, csi_dma_ch_t *tx_dma, csi_dma_ch_t *rx_dma);
```

* 功能描述:

  * 将DMA通道句柄连入UART句柄中，并打开UART的DMA发送与接收功能

* 参数:

  * `uart`: 实例句柄。
  * `tx_dma`: 用于发送的DMA通道句柄，传NULL时会关闭DMA异步发送功能。
  * `rx_dma`: 用于接收的DMA通道句柄，传NULL时会关闭DMA异步接收功能。

* 返回值:

  * CSI_ERROR: 调用失败。

* 使用示例：

```c
/* 句柄使用前请先初始化 */
static csi_uart_t g_uart;
csi_error_t ret;
csi_dma_ch_t g_dma_ch_tx;
csi_dma_ch_t g_dma_ch_rx;
/* 为发送和接收设置DMA通道 */
ret = csi_uart_link_dma(&g_uart, &g_dma_ch_tx, &g_dma_ch_rx);
if (ret != CSI_OK) {
    return -1;
}
```

>注意：
>在调用此函数前，需要先调用csi_uart_attach_callback来注册回调函数，否则将调用失败。

### csi_uart_send

```c
int32_t csi_uart_send(csi_uart_t *uart, const void *data, uint32_t size, uint32_t timeout);
```

* 功能描述:

  * uart以同步模式启动数据发送。

* 参数:

  * `uart`: 实例句柄。
  * `data`: 待发送数据的缓冲区地址。
  * `size`: 待发送数据的长度。
  * `timeout`: 超时时间，单位是毫秒ms，此超时时间指的是字节与字节的间隔时间，即发送上一个字节与下一个字节的间隔时间超过timeout，则会从函数退出。

* 返回值:

  * 成功发送的字节数或者CSI_ERROR。

* 使用示例：

```c
/* 句柄使用前请先初始化 */
static csi_uart_t g_uart;
int32_t ret_num;
const uint8_t tx_test_msg[] = "I am uart";
/* 超时时间请根据实际应用场景进行设置 */
ret_num = csi_uart_send(&g_uart, tx_test_msg, sizeof(tx_test_msg), 50);
```

### csi_uart_send_async

```c
csi_error_t csi_uart_send_async(csi_uart_t *uart, const void *data, uint32_t size);
```

* 功能描述:

  * uart以异步模式启动数据发送。

* 参数:

  * `uart`: 实例句柄。
  * `data`: 待发送数据的缓冲区地址。
  * `size`: 待发送数据的长度

* 返回值:

  * 错误码csi_error_t

>注意：
>在调用此函数前，需要先调用csi_uart_attach_callback来注册回调函数。本函数根据用户设置的
>不同，会进入中断工作模式或DMA工作模式。具体使用方式会在文末用例中综合说明。

### csi_uart_receive

```c
int32_t csi_uart_receive(csi_uart_t *uart, void *data, uint32_t size, uint32_t timeout);
```

* 功能描述:

  * UART以同步模式启动数据接收。

* 参数:

  * `uart`: 实例句柄。
  * `data`: 待接收数据的缓冲区地址。
  * `size`: 待接收数据的长度。
  * `timeout`: 超时时间，单位是毫秒ms，此超时时间指的是字节与字节的间隔时间，即接收上一个字节与下一个字节的间隔时间超过timeout，则会从函数退出。

* 返回值:

  * 成功接收的字节数或者CSI_ERROR。

* 使用示例：

```c
/* 句柄使用前请先初始化 */
static csi_uart_t g_uart;
int32_t ret_num;
uint8_t rx_test_msg[16];
/* 超时时间请根据实际应用场景进行设置 */
ret_num = csi_uart_receive(&g_uart, rx_test_msg, sizeof(rx_test_msg), 0xffffffff);
```

### csi_uart_receive_async

```c
csi_error_t csi_uart_receive_async(csi_uart_t *uart, void *data, uint32_t size);
```

* 功能描述:

  * UART以异步模式读取数据。

* 参数:

  * `uart`: 实例句柄。
  * `data`: 待接收数据的缓冲区地址。
  * `size`: 预计接收数据的长度。

* 返回值:

  * 错误码csi_error_t

>注意：
>在调用此函数前，需要先调用csi_uart_attach_callback来注册回调函数。本函数根据用户设置的
>不同，会进入中断工作模式或DMA工作模式。具体使用方式会在文末用例中综合说明。

### csi_uart_getc

```c
uint8_t  csi_uart_getc(csi_uart_t *uart);
```

* 功能描述:

  * 从uart 读取一个字节。

* 参数:

  * `uart`: 实例句柄。

* 返回值:

  * 返回读取到的字节内容。

### csi_uart_putc

```c
void csi_uart_putc(csi_uart_t *uart, uint8_t ch);
```

* 功能描述:

  * 从uart 发送一个字节。

* 参数:

  * `uart`: 实例句柄。
  * `ch`: 需发送的字节内容。

### csi_uart_get_state

```c
csi_error_t csi_uart_get_state(csi_uart_t *uart, csi_state_t *state);
```

* 功能描述:

  * 获取UART的状态。通过此函数来判断UART设备在获取状态的时刻是否可以进行send和receive操作。

* 参数:

  * `uart`: 实例句柄。
  * `state`：返回读写状态信息

* 返回值:

  * 错误码csi_error_t

#### csi_state_t
| 类型 | 说明     |
| :------------- | :--------- |
| readable | 设备可读 |
| writeable | 设备可写 |
| error | 错误状态 |

------

## 初始化示例

```c
/* 句柄空间一般使用静态空间 */
static csi_uart_t g_uart;

int main() {
    csi_error_t ret;
    /* init函数的idx参数，请根据soc的实际情况进行选择 */
    ret = csi_uart_init(&g_uart, 0);
    if (ret != CSI_OK) {
        return -1;
    }
}

```

## 同步模式使用示例

```c
static csi_uart_t g_uart;
static uint8_t recv_buf[16];
static char send_buf[128];
#define EXAMPLE_UART_IDX        0
#define EXAMPLE_UART_BAUDRATE   115200
#define UART_CHECK_RETURN(ret)                      \
        do {                                        \
            if (ret != CSI_OK) {                    \
                return -1;                          \
            }                                       \
        } while(0);

int example_uart(void)
{
    csi_error_t ret;
    int32_t ret_num;

    /* init uart */
    ret = csi_uart_init(&g_uart, EXAMPLE_UART_IDX);
    UART_CHECK_RETURN(ret);

    /* set uart baudrate */
    ret = csi_uart_baud(&g_uart, EXAMPLE_UART_BAUDRATE);
    UART_CHECK_RETURN(ret);

    /* set uart format */
    ret = csi_uart_format(&g_uart, UART_DATA_BITS_8, UART_PARITY_NONE, UART_STOP_BITS_1);
    UART_CHECK_RETURN(ret);

    strcpy(send_buf, "hello world\n\r");

    ret_num = csi_uart_send(&g_uart, send_buf, strlen(send_buf), 50);
    if (ret_num != strlen(send_buf)) {
        return -1;
    }

    ret_num = csi_uart_receive(&g_uart, recv_buf, sizeof(recv_buf), 0xffffffff);
    if (ret_num != sizeof(recv_buf)) {
        return -1;
    }

    /* Uninit the uart device */
    csi_uart_uninit(&g_uart);
    return 0;
}
```

##  异步模式使用示例

> 中断模式和DMA模式的使用流程基本一致，差别仅在是否调用了csi_uart_link_dma接口。

### DMA模式的使用示例

```c
static csi_uart_t g_uart;
static csi_dma_ch_t g_dma_ch_tx;
static csi_dma_ch_t g_dma_ch_rx;
static volatile uint8_t rx_async_flag = 0;
static volatile uint8_t tx_async_flag = 0;
static uint8_t recv_buf[16];
static uint8_t send_buf[128];

#define EXAMPLE_UART_BAUDRATE   115200
#define UART_CHECK_RETURN(ret)                      \
        do {                                        \
            if (ret != CSI_OK) {                    \
                return -1;                          \
            }                                       \
        } while(0);

static void uart_event_cb(csi_uart_t *uart, csi_uart_event_t event, void *arg)
{
    switch (event) {
        case UART_EVENT_SEND_COMPLETE:
            tx_async_flag = 1;
            break;

        case UART_EVENT_RECEIVE_COMPLETE:
            rx_async_flag = 1;
            break;

        default:
            break;
    }
}

/* use console uart to show how to use dma mode */
int example_uart_dma()
{
    csi_error_t ret;

    /* init uart, EXAMPLE_UART_IDX == 0 */
    ret = csi_uart_init(&g_uart, EXAMPLE_UART_IDX);
    UART_CHECK_RETURN(ret);

    /* set uart baudrate */
    ret = csi_uart_baud(&g_uart, EXAMPLE_UART_BAUDRATE);
    UART_CHECK_RETURN(ret);

    /* set uart format */
    ret = csi_uart_format(&g_uart, UART_DATA_BITS_8, UART_PARITY_NONE, UART_STOP_BITS_1);
    UART_CHECK_RETURN(ret);

    /* attach callback to uart device */
    ret = csi_uart_attach_callback(&g_uart, uart_event_cb, NULL);
    UART_CHECK_RETURN(ret);

    /* Link DMA */
    ret = csi_uart_link_dma(&g_uart, &g_dma_ch_tx, &g_dma_ch_rx);
    UART_CHECK_RETURN(ret);

    strcpy(send_buf, "hello world\n\r");

    ret = csi_uart_send_async(&g_uart, send_buf, strlen(send_buf));
    UART_CHECK_RETURN(ret);

    while(1) {
        if (tx_async_flag) {
            tx_async_flag = 0;
            break;
        }
    }

    ret = csi_uart_receive_async(&g_uart, recv_buf, sizeof(recv_buf));
    UART_CHECK_RETURN(ret);

    while(1) {
        if (rx_async_flag) {
            rx_async_flag = 0;
            break;
        }
    }

    /* Unlink DMA */
    ret = csi_uart_link_dma(&g_uart, NULL, NULL);
    UART_CHECK_RETURN(ret);

    /* Detach the uart callback */
    csi_uart_detach_callback(&g_uart);

    /* Uninit the uart device */
    csi_uart_uninit(&g_uart);
    return 0;
}
```

### 中断模式的使用示例

```c
static csi_uart_t g_uart;
static volatile uint8_t rx_async_flag = 0;
static volatile uint8_t tx_async_flag = 0;
static uint8_t recv_buf[16];
static uint8_t send_buf[128];

#define EXAMPLE_UART_BAUDRATE   115200
#define UART_CHECK_RETURN(ret)                      \
        do {                                        \
            if (ret != CSI_OK) {                    \
                return -1;                          \
            }                                       \
        } while(0);

static void uart_event_cb(csi_uart_t *uart, csi_uart_event_t event, void *arg)
{
    switch (event) {
        case UART_EVENT_SEND_COMPLETE:
            tx_async_flag = 1;
            break;

        case UART_EVENT_RECEIVE_COMPLETE:
            rx_async_flag = 1;
            break;

        default:
            break;
    }
}

/* use console uart to show how to use intr mode */
int example_uart_intr()
{
    csi_error_t ret;

    /* init uart, EXAMPLE_UART_IDX == 0 */
    ret = csi_uart_init(&g_uart, EXAMPLE_UART_IDX);
    UART_CHECK_RETURN(ret);

    /* set uart baudrate */
    ret = csi_uart_baud(&g_uart, EXAMPLE_UART_BAUDRATE);
    UART_CHECK_RETURN(ret);

    /* set uart format */
    ret = csi_uart_format(&g_uart, UART_DATA_BITS_8, UART_PARITY_NONE, UART_STOP_BITS_1);
    UART_CHECK_RETURN(ret);

    /* attach callback to uart device */
    ret = csi_uart_attach_callback(&g_uart, uart_event_cb, NULL);
    UART_CHECK_RETURN(ret);

    strcpy(send_buf, "hello world\n\r");

    ret = csi_uart_send_async(&g_uart, send_buf, strlen(send_buf));
    UART_CHECK_RETURN(ret);

    while(1) {
        if (tx_async_flag) {
            tx_async_flag = 0;
            break;
        }
    }

    ret = csi_uart_receive_async(&g_uart, recv_buf, sizeof(recv_buf));
    UART_CHECK_RETURN(ret);

    while(1) {
        if (rx_async_flag) {
            rx_async_flag = 0;
            break;
        }
    }

    /* Detach the uart callback */
    csi_uart_detach_callback(&g_uart);

    /* Uninit the uart device */
    csi_uart_uninit(&g_uart);
    return 0;
}
```
