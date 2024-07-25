# Timer

## 说明

硬件定时器为自动加载计数器，通过API设定超时时间，定时产生中断并进入用户回调函数，以实现定时响应、定时控制。

**特点**

- 独立可编程定时器
- 可编程超时范围周期
- 达到预定值触发中断事件

---

## 接口列表

TIMER的CSI接口如下所示：

| 函数                          | 说明                 |
| ----------------------------- | -------------------- |
| csi_timer_init                | 初始化               |
| csi_timer_uninit              | 反初始化            |
| csi_timer_start               | 开始定时             |
| csi_timer_stop                | 停止定时             |
| csi_timer_get_remaining_value | 获取定时中断剩余计数值 |
| csi_timer_get_load_value      | 获取定时器加载值     |
| csi_timer_is_running          | 检测定时器工作状态   |
| csi_timer_attach_callback     | 注册回调函数         |
| csi_timer_detach_callback     | 注销回调函数         |

---

## 接口详细说明

### csi_timer_init

```c
csi_error_t csi_timer_init(csi_timer_t *timer, uint32_t idx)
```

* 功能描述:

  * 通过设备ID初始化对应的TIMER实例。

* 参数:
  
  * `timer`: 设备句柄（需要用户申请句柄空间）。

  * `idx`: 设备ID。

* 返回值:

  * 错误码csi_error_t。
  
---

#### csi_timer_t

| 成员              | 类型      | 说明    |
| :------------- | :--------- | :--------- |
| dev | csi_dev_t | csi设备统一句柄     |
| callback | void (*callback)(csi_timer_t *timer, void *arg) | 用户回调函数     |
| arg | void * | 回调函数对应传参      |
| priv | void * | 设备私有变量 |

------

### csi_timer_uninit

```c
void csi_timer_uninit(csi_timer_t *timer)
```

* 功能描述:

  * TIMER实例反初始化，该接口会停止TIMER实例正在进行的工作，并且释放相关的软硬件资源。

* 参数:

  * `timer`: 设备句柄。

* 返回值:

  * 无。

------

### csi_timer_start

```c
csi_error_t csi_timer_start(csi_timer_t *timer, uint32_t timeout_us)
```

* 功能描述:

  * TIMER计时启动。

* 参数:

  * `timer`: 设备句柄。

  * `timeout_us`:设置超时（单位us）。

* 返回值:

  * 错误码csi_error_t。

------

### csi_timer_stop

```c
void csi_timer_stop(csi_timer_t *timer)
```

* 功能描述:

  * TIMER计时停止。

* 参数:

  * `timer`: 设备句柄。

* 返回值:

  * 无。

------

### csi_timer_get_remaining_value

```c
uint32_t csi_timer_get_remaining_value(csi_timer_t *timer)
```

* 功能描述:

  * 获取TIMER距离超时中断的剩余计时值。

* 参数:

  * `timer`: 设备句柄。

* 返回值:

  * TIMER剩余计时值。

------

### csi_timer_get_load_value

```c
uint32_t csi_timer_get_load_value(csi_timer_t *timer)
```

* 功能描述:

  * 获取TIMER加载值。

* 参数:

  * `timer`: 设备句柄。

* 返回值:

  * TIMER加载值。

------

### csi_timer_is_running

```c
bool csi_timer_is_running(csi_timer_t *timer)
```

* 功能描述:

  * 检测TIMER是否正在工作。

* 参数:

  * `timer`: 设备句柄。

* 返回值:

  * ture代表TIMER正在工作。

  * false代表TIMER工作停止。

------

### csi_timer_attach_callback

```c
csi_error_t csi_timer_attach_callback(csi_timer_t *timer, void *callback, void *arg)
```

* 功能描述:

  * 注册回调函数。

* 参数:

  * `timer`: 设备句柄。
  
  * `callback`: 中断回调函数。
  
  * `arg`: 回调函数对应的传参，由用户自定义。

* 返回值:

  * 错误码csi_error_t。


####  callback

```c
void (*callback)(csi_timer_t *timer, void *arg);
```

其中TIMER为设备句柄，arg为用户自定义的回调函数对应的参数。 

------

### csi_timer_detach_callback

```c
void csi_timer_detach_callback(csi_timer_t *timer)
```

* 功能描述:

  * 注销回调函数。

* 参数:

  * `timer`: 设备句柄。

* 返回值：

  * 无。

---

## 初始化示例

```c
#include <stdio.h>
#include <soc.h>
#include <drv/timer.h>

static csi_timer_t g_timer;

int main(void)
{
    csi_error_t ret = 0;

    /* init timer 0 */
    ret = csi_timer_init(&g_timer, 0);
    
    return ret;
}
```

## 定时中断模式

```c
#include <stdio.h>
#include <soc.h>
#include <drv/timer.h>

#define CHECK_RETURN(ret)                           \
        do {                                        \
            if (ret != 0) {                         \
                return -1;                          \
            }                                       \
        } while(0);
        
static csi_timer_t g_timer;
static uint8_t cb_timer_flag = 0;

static void timer_event_cb_reload_fun(csi_timer_t *timer_handle, void *arg)
{
    (void)arg;
    cb_timer_flag = 1;
}

int main(void)
{
    csi_error_t ret = 0;

    /* STEP 1：init timer 0 */
    ret = csi_timer_init(&g_timer, 0);
    CHECK_RETURN(ret);

    /* STEP 2: register callback func */
    ret = csi_timer_attach_callback(&g_timer, timer_event_cb_reload_fun, NULL);
    CHECK_RETURN(ret);

    /* STEP 3：start timer 0 */
    ret = csi_timer_start(&g_timer, 10000000);
    CHECK_RETURN(ret);

    /* STEP 4：clear flag and delay */
    cb_timer_flag = 0;
    mdelay(11000);

    /* STEP 5：judge whether the system enters the interrupt function */
    if (0 == cb_timer_flag) {
        ret = -1;
    }

    /* STEP 6：stop timer 0 */
    csi_timer_stop(&g_timer);

    /* STEP 7：cancel timer */
    csi_timer_detach_callback(&g_timer);

    /* STEP 8：uninit timer */
    csi_timer_uninit(&g_timer);

    return ret;
}

```
