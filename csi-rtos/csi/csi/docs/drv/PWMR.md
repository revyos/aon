# pwmr

## 说明

脉冲宽度调制红外（Pulse Width Modulation infraRed，简称PWMR）是一种对红外信号进行数字编码的强大技术，它通过产生一系列占空比调制的方波来实现。

## 接口列表

PWMR的CSI接口如下所示：

| 函数                    | 说明         |
| ----------------------- | ------------ |
| csi_pwmr_init            | 初始化       |
| csi_pwmr_uninit          | 反初始化     |
| csi_pwmr_out_config      | 配置输出模式 |
| csi_pwmr_out_start       | 开始输出     |
| csi_pwmr_out_stop        | 停止输出     |
| csi_pwmr_attach_callback | 注册回调     |
| csi_pwmr_detach_callback | 注销回调     |

## 接口详细说明

### csi_pwmr_init

```c
csi_error_t csi_pwmr_init(csi_pwmr_t *pwmr, uint32_t idx)
```

* 功能说明：
  
  *  通过设备号初始化对于的PWMR实例。

* 参数：
  
  *  `pwmr`：设备句柄（需要用户申请句柄空间）。
  
  *  `idx`：设备号。

* 返回值：
  
  *  错误码csi_error_t。
  

---

#### csi_pwmr_t

| 成员     | 类型                                                         | 说明                       |
| -------- | ------------------------------------------------------------ | -------------------------- |
| dev      | csi_dev_t                                                    | csi设备统一句柄            |
| callback | void (*callback)(csi_pwmr_t *pwmr, csi_pwmr_event_t event, uint32_t ch, void *arg) | 用户回调函数               |
| arg      | void *                                                       | 回调函数参数（用户自定义） |
| priv     | void *                                                       | 设备私有变量               |



---

### csi_pwmr_uninit

```c
void csi_pwmr_uninit(csi_pwmr_t *pwmr)
```

* 功能说明：
  
  *  PWMR实例反初始化，该接口会停止PWMR实例正在进行的工作，并且释放相关的软硬件资源。

* 参数：
  
  *  `pwmr`: 设备句柄。

* 返回值：

  *  无。

---

### csi_pwmr_out_config

```c
csi_error_t csi_pwmr_out_config(csi_pwmr_t *pwmr,
                                uint32_t channel,
                                uint32_t duty,
                                uint32_t frequency);
```

* 功能说明：

  *  配置pwmr信号输出模式。

* 参数：

  * `pwmr`: 设备句柄。

  * `channel`：通道号（范围0）。

  * `duty`：信号高电平占空比1代表0.1%,999代表99.9%。

  * `frequency`：输出信号频率，1KHz ~7MHz。

* 返回值：

  *  错误码csi_error_t。

---

### csi_pwmr_out_start

```c
csi_error_t csi_pwmr_out_start(csi_pwmr_t *pwmr, uint32_t channel)
```

* 功能说明：

  *  pwmr信号开始输出。

* 参数：

  * `pwmr`: 设备句柄。

  * `channel`：通道号（范围0）。

* 返回值：

  * 错误码csi_error_t。

---

### csi_pwmr_out_stop

```c
void csi_pwmr_out_stop(csi_pwmr_t *pwmr, uint32_t channel)
```

* 功能说明：

  *  pwmr信号停止输出。

* 参数：

  * `pwmr`: 设备句柄。

  * `channel`：通道号（范围0）。

* 返回值：

  * 错误码csi_error_t。

---

### csi_pwmr_attach_callback

```c
csi_error_t csi_pwmr_attach_callback(csi_pwmr_t *pwmr, void *callback, void *arg)
```

* 功能说明：

  *  注册中断回调函数。

* 参数：

  * `pwmr`: 设备句柄。

  * `callback`:中断回调函数。

  * `arg`: 回调函数参数（可选，由用户定义）。

* 返回值：

  * 错误码csi_error_t。

####  callback

```c
void (*callback)(csi_pwmr_t *pwmr, csi_pwmr_event_t event, uint32_t ch, void *arg);
```

其中pwmr为设备句柄，event 为传给回调函数的事件类型，ch为pwmr中断响应通道号，time_us为事件触发间隔事件，arg为用户自定义的回调函数对应的参数。pwmr回调事件枚举类型csi_pwmr_event_t定义如下： 

| PWMR_EVENT_COUNT_ZERO          | 计数到0事件            |
| -------------------------- | -------------- |
| PWMR_EVENT_COUNT_EQU_TOP       | 计数到TOP事件 |
| PWMR_EVENT_COUNT_EQU_COMP_UP | 计数上升到比较值事件 |
| PWMR_EVENT_COUNT_EQU_COMP_DOWN | 计数到下降到比较值事件 |
| PWMR_EVENT_OVERTIME | 错误事件类型   |

---

### csi_pwmr_detach_callback

```c
void csi_pwmr_detach_callback(csi_pwmr_t *pwmr)
```

* 功能说明：

  *  注销中断回调函数。

* 参数：

  * `pwmr`: 设备句柄。
  
* 返回值：

  * 无。

---



## 示例代码：

​         演示了 调节频率，调节占空比和 中断控制输出100个脉冲例子

```c
/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     example_pwmr.c
 * @brief    the main function for the PWMR driver
 * @version  V1.0
 * @date     26. March 2020
 ******************************************************************************/
#include <stdio.h>
#include <soc.h>
#include <drv/pwmr.h>
#include <drv/pin.h>
#include <board_init.h>
#include <board_config.h>


#define SEND_PULSE_NUM   100

static csi_pwmr_t g_pwmr_handle;
static uint32_t pwmr_flag = 0, pwmr_count;
void pwmr_callback(csi_pwmr_t *pwmr, csi_pwmr_event_t event, uint32_t ch, void *arg)
{
    switch (event) {
        case PWMR_EVENT_COUNT_ZERO:
            break;

        case PWMR_EVENT_COUNT_EQU_TOP:
            if (pwmr_count++ > SEND_PULSE_NUM) {
                pwmr_flag = 0;
                csi_pwmr_out_stop(pwmr, EXAMPLE_PWMR_CH_IDX);
            }
            break;

        case PWMR_EVENT_COUNT_EQU_COMP_UP  :
            break;

        case PWMR_EVENT_COUNT_EQU_COMP_DOWN:
            break;

        case PWMR_EVENT_OVERTIME:
            break;
        default:
            break;
    }
}

void  pwmr_output_pinmux_init(void)
{
    csi_pin_set_mux(EXAMPLE_PWMR_PIN, EXAMPLE_PWMR_PIN_FUNC);
}



int32_t  pwmr_signal_output(void)
{
    int32_t state;
    int32_t ret = 0, i, freq;

    state = csi_pwmr_init(&g_pwmr_handle, EXAMPLE_PWMR_IDX);

    if (state < 0) {
        printf("csi_pwmr_init error\n");
        ret = -1;
    }

    printf("Change freq from 100k t0 ~7M.Duty cycle is 10\n");

    for (i = 1; i < 70; i++) {
        freq = 100000 * i;
        state = csi_pwmr_out_config(&g_pwmr_handle, EXAMPLE_PWMR_CH_IDX, 100, freq);

        if (state < 0) {
            printf("first csi_pwmr_config error,i=%d\n",i);
            ret = -1;
        }

        state = csi_pwmr_out_start(&g_pwmr_handle, EXAMPLE_PWMR_CH_IDX);

        if (state < 0) {
            printf("first csi_pwmr_out_start error\n");
            ret = -1;
        }

        csi_pwmr_out_stop(&g_pwmr_handle, EXAMPLE_PWMR_CH_IDX);
    }

    printf("Change duty cycle from 1.0 to 99.9.Frequency is 10k.\n");

    for (i = 1; i < 99; i++) {
        printf("Start...using duty cycle :%0.1f%c\n", i / 1.0, '%');
        state = csi_pwmr_out_config(&g_pwmr_handle, EXAMPLE_PWMR_CH_IDX, i * 10, 10000);

        if (state < 0) {
            printf("first csi_pwmr_config error\n");
            ret = -1;
        }

        state = csi_pwmr_out_start(&g_pwmr_handle, EXAMPLE_PWMR_CH_IDX);

        if (state < 0) {
            printf("first csi_pwmr_out_start error\n");
            ret = -1;
        }

        csi_pwmr_out_stop(&g_pwmr_handle, EXAMPLE_PWMR_CH_IDX);
    }

    printf("Output %d pulse.\n", SEND_PULSE_NUM);
    csi_pwmr_attach_callback(&g_pwmr_handle, pwmr_callback, NULL);
    pwmr_flag = 1;
    pwmr_count = 0;
    state = csi_pwmr_out_config(&g_pwmr_handle, EXAMPLE_PWMR_CH_IDX, 500, 10000);
    state = csi_pwmr_out_start(&g_pwmr_handle, EXAMPLE_PWMR_CH_IDX);

    while (1) {
        if (pwmr_flag == 0) {
            break;
        }
    }

    csi_pwmr_uninit(&g_pwmr_handle);

    return ret;
}

int example_pwmr()
{
    int ret = 0;
    int32_t state;

    pwmr_output_pinmux_init();
    state = pwmr_signal_output();

    if (state < 0) {
        printf("pwmr_signal_output error\n");
        ret = -1;
    } else {
        printf("pwmr_signal_output successful\n");
    }

    return ret;
}

int main(void)
{
    board_init();

    return example_pwmr();
}

```

