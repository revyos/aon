# PWM

## 说明

脉冲宽度调制（Pulse width modulation，简称PWM）是一种强大的模拟信号数字编码技术，它利用高分辨率计数器产生方波，并通过调制方波占空比对模拟信号进行编码。典型应用包括开关电源和电机控制。

## 接口列表

PWM的CSI接口如下所示：

| 函数                    | 说明         |
| ----------------------- | ------------ |
| csi_pwm_init            | 初始化       |
| csi_pwm_uninit          | 反初始化     |
| csi_pwm_out_config      | 配置输出模式 |
| csi_pwm_out_start       | 开始输出     |
| csi_pwm_out_stop        | 停止输出     |
| csi_pwm_capture_config  | 配置捕获模式 |
| csi_pwm_capture_start   | 开始捕获     |
| csi_pwm_capture_stop    | 停止捕获     |
| csi_pwm_attach_callback | 注册回调     |
| csi_pwm_detach_callback | 注销回调     |

## 接口详细说明

### csi_pwm_init

```c
csi_error_t csi_pwm_init(csi_pwm_t *pwm, uint32_t idx)
```

* 功能说明：
  
  *  通过设备号初始化对于的PWM实例。

* 参数：
  
  *  `pwm`：设备句柄（需要用户申请句柄空间）。
  
  *  `idx`：设备号。

* 返回值：
  
  *  错误码csi_error_t。
  

---

#### csi_pwm_t

| 成员     | 类型                                                         | 说明                       |
| -------- | ------------------------------------------------------------ | -------------------------- |
| dev      | csi_dev_t                                                    | csi设备统一句柄            |
| callback | void (*callback)(csi_pwm_t *pwm, csi_pwm_event_t event, uint32_t ch, uint32_t time_us, void *arg) | 用户回调函数               |
| arg      | void *                                                       | 回调函数参数（用户自定义） |
| priv     | void *                                                       | 设备私有变量               |



---

### csi_pwm_uninit

```c
void csi_pwm_uninit(csi_pwm_t *pwm)
```

* 功能说明：
  
  *  PWM实例反初始化，该接口会停止PWM实例正在进行的工作，并且释放相关的软硬件资源。

* 参数：
  
  *  `pwm`: 设备句柄。

* 返回值：

  *  无。

---

### csi_pwm_out_config

```c
csi_error_t csi_pwm_out_config(csi_pwm_t *pwm,
                             uint32_t channel,
                             uint32_t period_us,
                             uint32_t pulse_width_us,
                             csi_pwm_polarity_t polarity)
```

* 功能说明：

  *  配置PWM信号输出模式。

* 参数：

  * `pwm`: 设备句柄。

  * `channel`：通道号（范围0~5）。

  * `period_us`：信号周期时间（单位us）。

  * `pulse_width_us`：有效电平时间（单位us）。

  * `polarity`：有效电平极性。

* 返回值：

  *  错误码csi_error_t。

---

#### csi_pwm_polarity_t

| 类型              | 说明   |
| ----------------- | ------ |
| PWM_POLARITY_HIGH | 高电平 |
| PWM_POLARITY_LOW  | 低电平 |

---

### csi_pwm_out_start

```c
csi_error_t csi_pwm_out_start(csi_pwm_t *pwm, uint32_t channel)
```

* 功能说明：

  *  PWM信号开始输出。

* 参数：

  * `pwm`: 设备句柄。

  * `channel`：通道号（范围0~5）。

* 返回值：

  * 错误码csi_error_t。

---

### csi_pwm_out_stop

```c
void csi_pwm_out_stop(csi_pwm_t *pwm, uint32_t channel)
```

* 功能说明：

  *  PWM信号停止输出。

* 参数：

  * `pwm`: 设备句柄。

  * `channel`：通道号（范围0~5）。

* 返回值：

  * 错误码csi_error_t。

---

### csi_pwm_capture_config

```c
csi_error_t csi_pwm_capture_config(csi_pwm_t *pwm,
                                   uint32_t channel,
                                   csi_pwm_capture_polarity_t polarity,
                                   uint32_t count)
```

* 功能说明：

  *  配置PWM输入捕获模式。

* 参数：

  * `pwm`: 设备句柄。
  
  * `channel`：通道号（范围0~5）。

  * `polarity`：配置捕获方式（上升沿/下降沿/边沿）。

  * `count`：配置捕获次数（捕获达到count次以后产生中断）。

* 返回值：

  * 错误码csi_error_t。

---

#### csi_pwm_capture_polarity_t

| 类型                          | 说明           |
| ----------------------------- | -------------- |
| PWM_CAPTURE_POLARITY_POSEDGE  | 上升沿捕获类型 |
| PWM_CAPTURE_POLARITY_NEGEDGE  | 下降沿捕获类型 |
| PWM_CAPTURE_POLARITY_BOTHEDGE | 边沿捕获类型   |

---

### csi_pwm_capture_start

```c
csi_error_t csi_pwm_capture_start(csi_pwm_t *pwm, uint32_t channel)
```

* 功能说明：

  *  PWM开始捕获。

* 参数：

  * `pwm`: 设备句柄。

  * `channel`：通道号（范围0~5）。

* 返回值：

  * 错误码csi_error_t。

---

### csi_pwm_capture_stop

```c
void csi_pwm_capture_stop(csi_pwm_t *pwm, uint32_t channel)
```

* 功能说明：

  *  PWM停止捕获。

* 参数：

  * `pwm`: 设备句柄。

  * `channel`：通道号（范围0~5）。

* 返回值：

  * 无。

---

### csi_pwm_attach_callback

```c
csi_error_t csi_pwm_attach_callback(csi_pwm_t *pwm, void *callback, void *arg)
```

* 功能说明：

  *  注册中断回调函数。

* 参数：

  * `pwm`: 设备句柄。

  * `callback`:中断回调函数。

  * `arg`: 回调函数参数（可选，由用户定义）。

* 返回值：

  * 错误码csi_error_t。

####  callback

```c
void (*callback)(csi_pwm_t *pwm, csi_pwm_event_t event, uint32_t ch, uint32_t time_us, void *arg);
```

其中pwm为设备句柄，event 为传给回调函数的事件类型，ch为pwm中断响应通道号，time_us为事件触发间隔事件，arg为用户自定义的回调函数对应的参数。pwm回调事件枚举类型csi_pwm_event_t定义如下： 

| 事件类型                   | 事件说明       |
| -------------------------- | -------------- |
| PWM_EVENT_CAPTURE_POSEDGE  | 上升沿事件类型 |
| PWM_EVENT_CAPTURE_NEGEDGE  | 下降沿事件类型 |
| PWM_EVENT_CAPTURE_BOTHEDGE | 边沿事件类型   |
| PWM_EVENT_ERROR            | 错误事件类型   |

---

### csi_pwm_detach_callback

```c
void csi_pwm_detach_callback(csi_pwm_t *pwm)
```

* 功能说明：

  *  注销中断回调函数。

* 参数：

  * `pwm`: 设备句柄。
  
* 返回值：

  * 无。

---



## 初始化示例

```c
#include <stdio.h>
#include <soc.h>
#include <drv/pwm.h>
        
static csi_pwm_t g_pwm;

int main(void)
{
    csi_error_t ret = 0;

    ret = csi_pwm_init(&g_pwm, 0);

    return ret;
}
```

## 信号输出模式

```c
#include <stdio.h>
#include <soc.h>
#include <drv/pwm.h>
#include <drv/pin.h>

#define EXAMPLE_PWM_IDX                     0U
/**
 * PWM has 6 channels in total
*/
#define EXAMPLE_PWM_CH_IDX                  0U

#define EXAMPLE_PWM_CH                      PA0
#define EXAMPLE_PWM_CH_FUNC                 PA0_PWM_CH0

#define CHECK_RETURN(ret)                           \
        do {                                        \
            if (ret != 0) {                         \
                return -1;                          \
            }                                       \
        } while(0);


static csi_pwm_t g_pwm;

int main(void)
{
    csi_error_t ret = 0;

    /* STEP 1: pwm port */
    csi_pin_set_mux(EXAMPLE_PWM_CH, EXAMPLE_PWM_CH_FUNC);

    /* STEP 2: pwm init */
    ret = csi_pwm_init(&g_pwm, EXAMPLE_PWM_IDX);
    CHECK_RETURN(ret);

    /** 
     * STEP 3: pwm channel 0 configure operating mode and signal period 
     * period time: 30us
     * low level time: 10us
     */
    ret = csi_pwm_out_config(&g_pwm, EXAMPLE_PWM_CH_IDX, 30, 10, PWM_POLARITY_LOW);
    CHECK_RETURN(ret);

    /* STEP 4: pwm channel 0 start ouput */
    ret = csi_pwm_out_start(&g_pwm, EXAMPLE_PWM_CH_IDX);
    CHECK_RETURN(ret);

    mdelay(10000);

    /* STEP 5: pwm channel 0 stop output */
    csi_pwm_out_stop(&g_pwm, EXAMPLE_PWM_CH_IDX);

    /* STEP 6: pwm uninit */
    csi_pwm_uninit(&g_pwm);

    return ret;
}
```

## 输入捕获模式

```c
#include <stdio.h>
#include <soc.h>
#include <drv/pwm.h>
#include <drv/pin.h>

#define EXAMPLE_PWM_IDX                           0U

#define EXAMPLE_PWM_CH_IDX                        1U
#define EXAMPLE_PWM_CAPTURE_CH_IDX                0U

#define EXAMPLE_PWM_CH                            PA2
#define EXAMPLE_PWM_CH_FUNC                       PA2_PWM_CH2
#define EXAMPLE_PWM_CAPTURE_CH                    PA0
#define EXAMPLE_PWM_CAPTURE_CH_FUNC               PA0_PWM_CH0

#define CHECK_RETURN(ret)                           \
        do {                                        \
            if (ret != 0) {                         \
                return -1;                          \
            }                                       \
        } while(0);

        
static csi_pwm_t g_pwm;
static uint32_t cb_pwm_time = 0;
static csi_pwm_event_t cb_pwm_flag = PWM_EVENT_ERROR;


static void  pwm_event_cb_fun(csi_pwm_t *pwm_handle, csi_pwm_event_t event, uint8_t ch, uint32_t time_us, void *arg)
{
    switch (event) {
        case PWM_EVENT_CAPTURE_POSEDGE:
            cb_pwm_flag = PWM_EVENT_CAPTURE_POSEDGE;
            cb_pwm_time = time_us;
            break;
        case PWM_EVENT_CAPTURE_NEGEDGE:
            cb_pwm_flag = PWM_EVENT_CAPTURE_NEGEDGE;
            cb_pwm_time = time_us;
            break;
        case PWM_EVENT_CAPTURE_BOTHEDGE:
            cb_pwm_flag = PWM_EVENT_CAPTURE_BOTHEDGE;
            cb_pwm_time = time_us;
            break;
        default:
            break;
    }
}

int main(void)
{
    csi_error_t ret = 0;

    /** 
     * STEP 1: pwm port mux
     * enable channel 0/1
    */
    csi_pin_set_mux(EXAMPLE_PWM_CH, EXAMPLE_PWM_CH_FUNC);
    csi_pin_set_mux(EXAMPLE_PWM_CAPTURE_CH, EXAMPLE_PWM_CAPTURE_CH_FUNC);

    /* STEP 2: pwm init */
    ret = csi_pwm_init(&g_pwm, EXAMPLE_PWM_IDX);
    CHECK_RETURN(ret);

    /** 
     * STEP 3: pwm channel 1 to output 
     * period time: 5ms    
     * low time:    2.5ms 
     */
    csi_pwm_out_config(&g_pwm, EXAMPLE_PWM_CH_IDX, 5000, 2500, PWM_POLARITY_LOW);
    csi_pwm_out_start(&g_pwm, EXAMPLE_PWM_CH_IDX);

    /* STEP 4: register callback func */
    ret = csi_pwm_attach_callback(&g_pwm, pwm_event_cb_fun, NULL);
    CHECK_RETURN(ret);

    /** 
     * STEP 5: config channel 0 to input capture
     * posedge trigger 10 times into callback func
    */
    ret = csi_pwm_capture_config(&g_pwm, EXAMPLE_PWM_CAPTURE_CH_IDX, PWM_CAPTURE_POLARITY_POSEDGE, 10);
    CHECK_RETURN(ret);

    /* STEP 6: start capture channel 0 */
    ret = csi_pwm_capture_start(&g_pwm, EXAMPLE_PWM_CAPTURE_CH_IDX);
    CHECK_RETURN(ret);

    cb_pwm_flag = PWM_EVENT_ERROR;
    /* Verify data after waiting for 10 posedges */
    while(PWM_EVENT_CAPTURE_POSEDGE != cb_pwm_flag);

    /* STEP 7: stop capture */
    csi_pwm_capture_stop(&g_pwm, EXAMPLE_PWM_CAPTURE_CH_IDX);

    /* STEP 8: cancel callback */
    csi_pwm_detach_callback(&g_pwm);

    csi_pwm_uninit(&g_pwm);

    return ret;
}
```
