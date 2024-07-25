# ADC设备
-----

## 设备说明

ADC是Analog-to-Digital Converter的缩写。指模/数转换器或者模拟/数字转换器。是指将连续变量的模拟信号转换为离散的数字信号的器件。

ADC的特点

- 支持单次转换、连续转换

## 接口列表
ADC的CSI接口说明如下所示：

|   函数                        |     说明               |
| ----------------------------- | ---------------------- |
| csi_adc_init                  | ADC设备初始化          |
| csi_adc_uninit                | ADC设备反初始化        |
| csi_adc_set_buffer            | 设置ADC数据接收缓存    |
| csi_adc_start                 | 启动ADC数据转换(同步)  |
| csi_adc_start_async           | 启动ADC数据转换(异步)  |
| csi_adc_stop                  | 停止ADC数据转换(同步)  |
| csi_adc_stop_async            | 停止ADC数据转换(异步)  |
| csi_adc_channel_enable        | 开启或者关闭一个ADC通道|
| csi_adc_channel_sampling_time | 设置通道采样时间       |
| csi_adc_sampling_time         | 设置ADC采样时间        |
| csi_adc_continue_mode         | 开启/关闭连续采样模式  |
| csi_adc_freq_div              | 设置ADC分频系数        |
| csi_adc_read                  | 读取ADC转换结果        |
| csi_adc_get_state             | 获取ADC状态            |
| csi_adc_attach_callback       | 注册回调函数           |
| csi_adc_detach_callback       | 注销回调函数           |
| csi_adc_link_dma              | 绑定/解除DMA通道       |

## 接口详细说明

### csi_adc_init

```c
csi_error_t csi_adc_init(csi_adc_t *adc, uint32_t idx)
```

- 功能描述:

- 通过设备ID初始化对应的ADC实例，返回结果值。

- 参数:

  - `adc`: 设备句柄（需要用户申请句柄空间）
  - `idx`: 设备ID
- 返回值: 

  - 错误码csi_error_t

* * * * *
#### csi_adc_t

| 成员         | 类型            | 说明                  |
| ------------ | --------------- | --------------------- |
| dev          | csi_dev_t       | 设备统一句柄          |
| callback     | void (*callback)(csi_adc_t *adc, csi_adc_event_t event, void *arg) | 用户回调函数 |
| arg          | *void            | 用户回调函数对应的传参|
| data         | *uint32_t        | 指向接收数据缓存的地址|
| size         | uint32_t        | 接收数据的大小        |
| start        | csi_error_t (*start)(csi_adc_t *adc)| 指向ADC开始转换函数(异步) |
| stop         | csi_error_t (*stop)(csi_adc_t *adc)| 指向ADC停止转换函数(异步)  |
| state        | csi_state_t     | 运行状态               |
| dma          | *csi_dma_ch_t    | 指向接收DMA通道        |
| priv         | *void            | 设备私有变量       |

### csi_adc_uninit

```c
void csi_adc_uninit(csi_adc_t *adc)
```

- 功能描述:

- ADC实例反初始化, 并且释放相关的软硬件资源。

- 参数:

  - `adc`: 实例句柄。 

- 返回值

  - 无

### csi_adc_set_buffer

```c
csi_error_t csi_adc_set_buffer(csi_adc_t *adc, uint32_t *data, uint32_t num)
```

- 功能描述:

- 设置ADC数据接收缓存。

- 参数:

  - `adc` : 实例句柄。
  - `data`: 指向数据接收缓存。
  - `num`: 接收缓存的大小。
- 返回值: 

  - 错误码csi_error_t


### csi_adc_start

```c
csi_error_t csi_adc_start(csi_adc_t *adc)
```

- 功能描述:

- 启动ADC数据转换(同步)。

- 参数:

  - `adc` : 实例句柄。
- 返回值: 

  - 错误码csi_error_t

### csi_adc_start_async

```c
csi_error_t csi_adc_start_async(csi_adc_t *adc)
```

- 功能描述:

- 启动ADC数据转换(异步)。

- 参数:

  - `adc` : 实例句柄。
- 返回值: 

  - 错误码csi_error_t

### csi_adc_stop

```c
csi_error_t csi_adc_stop(csi_adc_t *adc)
```

- 功能描述:

- 停止ADC数据转换(同步)。

- 参数:

  - `adc` : 实例句柄。
- 返回值: 

  - 错误码csi_error_t

### csi_adc_stop_async

```c
csi_error_t csi_adc_stop_async(csi_adc_t *adc)
```

- 功能描述:

- 停止ADC数据转换(异步)。

- 参数:

  - `adc` : 实例句柄。
- 返回值: 

  - 错误码csi_error_t

### csi_adc_channel_enable

```c
csi_error_t csi_adc_channel_enable(csi_adc_t *adc, uint8_t ch_id, bool is_enable)
```

- 功能描述:

- 开启或者关闭一个ADC通道。

- 参数:

  - `adc` : 实例句柄。
  - `ch_id` : ADC通道ID。
  - `is_enable` : true->使能, false->禁止。
- 返回值: 

  - 错误码csi_error_t

### csi_adc_channel_sampling_time

```c
csi_error_t csi_adc_channel_sampling_time(csi_adc_t *adc, uint8_t ch_id, uint16_t clock_num)
```

- 功能描述:

- 设置ADC指定通道采样时间，单位：时钟周期个数。

- 参数:

  - `adc` : 实例句柄。
  - `ch_id` : ADC通道ID。
  - `clock_num` : 时钟周期个数。
- 返回值: 

  - 错误码csi_error_t

### csi_adc_sampling_time

```c
csi_error_t csi_adc_sampling_time(csi_adc_t *adc, uint16_t clock_num)
```

- 功能描述:

- 设置ADC控制器采样时间， 单位：时钟周期个数。

- 参数:

  - `adc` : 实例句柄。
  - `clock_num` : 时钟周期个数。
- 返回值: 

  - 错误码csi_error_t

### csi_adc_continue_mode

```c
csi_error_t csi_adc_continue_mode(csi_adc_t *adc, bool is_enable)
```

- 功能描述:

- 开启/关闭ADC连续采样模式。

- 参数:

  - `adc` : 实例句柄。
  - `is_enable` : true->使能， false->禁止。
- 返回值: 

  - 错误码csi_error_t

### csi_adc_freq_div

```c
uint32_t csi_adc_freq_div(csi_adc_t *adc, uint32_t div)
```

- 功能描述:

- 设置ADC分频系数。

- 参数:

  - `adc` : 实例句柄。
  - `div` : 分频系数。
- 返回值: 

  - 实际频率值，单位HZ。

### csi_adc_read

```c
int32_t csi_adc_read(csi_adc_t *adc)
```

- 功能描述:

- 读取ADC转换结果。

- 参数:

  - `adc` : 实例句柄。

- 返回值

  - 错误码csi_error_t

### csi_adc_get_state

```c
csi_error_t csi_adc_get_state(csi_adc_t *adc, csi_state_t *state)
```

- 功能描述:

- 获取ADC状态。

- 参数:

  - `adc` : 实例句柄。
  - `state` : 指向接收的状态值。
- 返回值: 

  - 错误码csi_error_t

#### csi_state_t

| 类型      | 说明     |
| --------- | ------- |
| readable  | 设备可读 |
| writeable | 设备可写 |
| error     | 错误状态 |

### csi_adc_attach_callback

```c
csi_error_t csi_adc_attach_callback(csi_adc_t *adc, void *callback, void *arg)
```

- 功能描述:

- 注册ADC事件回调函数。

- 参数:

  - `adc` : 实例句柄。
  - `callback` : 指向事件回调函数。
  - `arg` : 指向事件回调函数的参数。
- 返回值: 

  - 错误码csi_error_t

####  callback
```c
typedef void (*csi_adc_cb_t)(csi_adc_t *adc, csi_adc_event_t event, void *arg)
```

其中ADC为设备句柄，idx为设备ID，event 为传给回调函数的事件类型，arg为用户自定义的回调函数对应的参数。 ADC回调事件枚举类型csi_adc_event_t定义如下：


#### csi_adc_event_t
| 类型                               | 说明                                                        |
| ---------------------------------- | ----------------------------------------------------------- |
| ADC_EVENT_CONVERT_COMPLETE         | 转换完成事件，当指定的数据转换完成时触发                    |
| ADC_EVENT_CONVERT_HALF_DONE        | 转换完成一半，当指定的数据转换完成时触发                    |
| ADC_EVENT_ERROR                    | 数据丢失事件，未及时读取数据时触发                          |


### csi_adc_detach_callback

```c
void csi_adc_detach_callback(csi_adc_t *adc)
```

- 功能描述:

- 解除ADC事件回调函数。

- 参数:

  - `adc` : 实例句柄。

- 返回值

  - 无

### csi_adc_link_dma

```c
csi_error_t csi_adc_link_dma(csi_adc_t *adc, csi_dma_ch_t *dma);
```

- 功能描述:

- 绑定/解除DMA通道。

- 参数:

  - `adc` : 实例句柄。
  - `dma` : 指向dma通道。
- 返回值: 

  - 错误码csi_error_t

## 同步模式使用示例
> ADC可采用单次采样模式或者连续采样模式。默认使用单次转换模式，通过调用csi_adc_continue_mode(true)后，可将ADC切换到连续采样模式

> 单次采样模式示例如下:


```c
#include <stdio.h>
#include <string.h>

#include <drv/adc.h>
#include <drv/tick.h>
#include <board_config.h>
#include <board_init.h>

#define ADC_CHECK_RETURN(ret)            \
do {                                     \
    if (ret != CSI_OK) {                 \
        return -1;                       \
    }                                    \
} while(0);

static csi_adc_t adc;

int main()
{
    int      ret;
    uint32_t data;

    board_init();

    /* GPIO init */
    csi_pin_set_mux(EXAMPLE_ADC_CHANNEL0_PIN,  EXAMPLE_ADC_CHANNEL0_PIN_FUNC);

    /* Initialize ADC peripheral */
    ret = csi_adc_init(&adc, 0);
    ADC_CHECK_RETURN(ret);

    /* Configure frequency divsion, this value can be one of(4 8 16 32 64 128) */
    ret = csi_adc_freq_div(&adc, 128);
    if(ret == 0){
        return -1;
    }

    /* Configure sampling time */
    ret = csi_adc_sampling_time(&adc, 2);
    ADC_CHECK_RETURN(ret);

    /* Enable channel */
    ret = csi_adc_channel_enable(&adc, 0, true);
    ADC_CHECK_RETURN(ret);

    /* Trigger new conversion */
    ret = csi_adc_start(&adc);
    ADC_CHECK_RETURN(ret);

    /* Read result */
    data = csi_adc_read(&adc);
    printf("get adc result: %d\n", data);

    /* Uninit adc */
    csi_adc_uninit(&adc);
    return 0;
}
```

> 连续采样模式

```c
#include <stdio.h>
#include <string.h>

#include <drv/adc.h>
#include <drv/tick.h>
#include <board_config.h>
#include <board_init.h>

#define ADC_CHECK_RETURN(ret)            \
do {                                     \
    if (ret != CSI_OK) {                 \
        return -1;                       \
    }                                    \
} while(0);

static csi_adc_t adc;

int main()
{
    int      ret;
    uint32_t data;
    uint8_t  i = 0;

    board_init();

    /* GPIO init */
    csi_pin_set_mux(EXAMPLE_ADC_CHANNEL0_PIN,  EXAMPLE_ADC_CHANNEL0_PIN_FUNC);

    /* Initialize ADC peripheral */
    ret = csi_adc_init(&adc, 0);
    ADC_CHECK_RETURN(ret);

    /* Configure frequency divsion, this value can be one of(4 8 16 32 64 128) */
    ret = csi_adc_freq_div(&adc, 128);
    if(ret == 0){
        return -1;
    }

    /* Configure sampling time */
    ret = csi_adc_sampling_time(&adc, 2);
    ADC_CHECK_RETURN(ret);

    /* Enable continue mode */
    ret = csi_adc_continue_mode(&adc, true);
    ADC_CHECK_RETURN(ret);

    /* Enable channel */
    ret = csi_adc_channel_enable(&adc, 0, true);
    ADC_CHECK_RETURN(ret);

    /* Trigger new conversion */
    ret = csi_adc_start(&adc);
    ADC_CHECK_RETURN(ret);

    /* Read result */
    for(i = 0; i < 10; i++){
        data = csi_adc_read(&adc);
        printf("get adc result: %d\n", data);
    }

    /* Uninit ADC */
    csi_adc_uninit(&adc);
    return 0;
}
```

## 异步模式使用示例

### DMA模式

> 使用异步模式时，推荐使用DMA模式。中断模式不建议使用，频繁中断会造成大量的CPU消耗，同时有可能导致中断响应跟不上ADC采样频率，导致采样结果丢失。

> 1. 需要调用csi_adc_attach_callback()绑定回调，否则无法使用异步模式。

> 2. 需要调用csi_adc_set_buffer()设置数据缓存，否则无法使用异步模式。

> 3. 需要调用csi_adc_continue()开启连续采样模式，否则可能无法达到预期的size。


```c
#include <stdio.h>
#include <string.h>

#include <drv/adc.h>
#include <drv/tick.h>
#include <board_config.h>
#include <board_init.h>

#define ADC_CHECK_RETURN(ret)            \
do {                                     \
    if (ret != CSI_OK) {                 \
        return -1;                       \
    }                                    \
} while(0);

static csi_adc_t adc;
static csi_dma_ch_t dma_ch;
static int adc_event = -1;

static void adc_event_callback(csi_adc_t *adc, csi_adc_event_t event, void *arg)
{
    adc_event = event;
}

static void dump_adc_data(char *prefix, uint32_t *data, uint16_t size)
{
    printf("================== %s ==================\n", prefix);

    while (size--) {
        printf("%d, ", *data++);
    }

    printf("\n=============================================\n");
}

int main()
{
    int      ret;
    uint32_t rx_data[10];
    uint32_t timestart;

    board_init();

    /* GPIO init */
    csi_pin_set_mux(EXAMPLE_ADC_CHANNEL0_PIN,  EXAMPLE_ADC_CHANNEL0_PIN_FUNC);

    /* Initialize ADC peripheral */
    ret = csi_adc_init(&adc, 0);
    ADC_CHECK_RETURN(ret);

    /* Attach user callback */
    ret = csi_adc_attach_callback(&adc, adc_event_callback, NULL);
    ADC_CHECK_RETURN(ret);

    /* Enable dma mode */
    ret = csi_adc_link_dma(&adc, &dma_ch);
    ADC_CHECK_RETURN(ret);

    /* Set continue mode */
    csi_adc_continue_mode(&adc, true);

    /* Set data buffer */
    ret = csi_adc_set_buffer(&adc, rx_data, 10);
    ADC_CHECK_RETURN(ret);

    /* Configure frequency divsion, this value can be one of(4 8 16 32 64 128) */
    ret = csi_adc_freq_div(&adc, 128);
    if(ret == 0){
        return -1;
    }

    /* Configure sampling time */
    ret = csi_adc_sampling_time(&adc, 2);
    ADC_CHECK_RETURN(ret);

    /* Enable channel */
    ret = csi_adc_channel_enable(&adc, 0, true);
    ADC_CHECK_RETURN(ret);

    ret = csi_adc_channel_enable(&adc, 1, true);
    ADC_CHECK_RETURN(ret);

    /* Trigger new conversion */
    adc_event = -1;
    ret = csi_adc_start_async(&adc);
    ADC_CHECK_RETURN(ret);

    /* Read result */
    timestart = csi_tick_get_ms();
    while (adc_event != ADC_EVENT_CONVERSION_COMPLETE) {
        if ((csi_tick_get_ms() - timestart) > 5000) {
            return -1;
        }
    }

    dump_adc_data("rx_data", rx_data, 10);

    /* Stop conversion */
    ret = csi_adc_stop_async(&adc);
    ADC_CHECK_RETURN(ret);

    ret = csi_adc_link_dma(&adc, NULL);
    ADC_CHECK_RETURN(ret);

    csi_adc_detach_callback(&adc);

    /* Uninit ADC */
    csi_adc_uninit(&adc);
    return 0;
}
```
