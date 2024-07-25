# WDT

## 说明

WatchDog（看门狗）本质上是一个定时器，这个定时器可用来监控程序的运行。看门狗可以设置一个预定的时间，在指定时间内若未对定时器进行喂狗操作将会导致系统复位。程序设计时通过在程序的关键点预埋喂狗动作，当程序由于某种原因（软件或硬件故障）未按指定的逻辑运行时可复位系统，保证系统的可用性。



## 接口列表

WDT的CSI接口如下所示：

| 函数                       | 说明                   |
| -------------------------- | ---------------------- |
| csi_wdt_init               | 初始化                 |
| csi_wdt_uninit             | 反初始化               |
| csi_wdt_set_timeout        | 设置超时时间           |
| csi_wdt_start              | 看门狗开始工作         |
| csi_wdt_stop               | 看门狗停止工作         |
| csi_wdt_feed               | 看门狗喂狗             |
| csi_wdt_get_remaining_time | 获取看门狗复位剩余时间 |
| csi_wdt_is_running         | 检测看门狗工作状态     |
| csi_wdt_attach_callback    | 注册回调函数           |
| csi_wdt_detach_callback    | 注销回调函数           |

---

## 接口详细说明

### csi_wdt_init

```c
csi_error_t csi_wdt_init(csi_wdt_t *wdt, uint32_t idx)
```

* 功能描述:

  * 通过设备ID初始化对应的WDT实例。

* 参数:
  
  * `wdt`: 设备句柄（需要用户申请句柄空间）。

  * `idx`: 设备ID。

* 返回值:

  * 错误码csi_error_t。

#### csi_wdt_t

| 成员              | 类型      | 说明    |
| :------------- | :--------- | :--------- |
| dev | csi_dev_t | csi设备统一句柄     |
| callback | void (*callback)(csi_timer_t *timer, void *arg) | 用户回调函数     |
| arg | void * | 回调函数对应传参      |
| priv | void * | 设备私有变量 |

------

### csi_wdt_uninit

```c
void csi_wdt_uninit(csi_wdt_t *wdt)
```

* 功能描述:

  * WDT实例反初始化，该接口会停止WDT实例正在进行的工作，并且释放相关的软硬件资源。

* 参数:

  * `wdt`: 实例句柄。

* 返回值:

  * 无。

------

### csi_wdt_set_timeout

```c
csi_error_t csi_wdt_set_timeout(csi_wdt_t *wdt, uint32_t ms)
```

* 功能描述:

	* 设置看门狗超时时间。

* 参数:
	
	* `wdt`:设备句柄。

	* `ms`:设置超时时间（单位ms）。

* 返回值:

  * 错误码csi_error_t。

------

### csi_wdt_start

```c
csi_error_t csi_wdt_start(csi_wdt_t *wdt)
```

* 功能描述:

  * WDT计时启动。

* 参数:

  * `wdt`: 设备句柄。

* 返回值:

  * 错误码csi_error_t。

------

### csi_wdt_stop

```c
void csi_wdt_stop(csi_wdt_t *wdt)
```

* 功能描述:

  * WDT计时停止。

* 参数:

  * `wdt`: 设备句柄。

* 返回值:

  * 无。

------

### csi_wdt_feed

```c
csi_error_t csi_wdt_feed(csi_wdt_t *wdt)
```

* 功能描述:

  * 看门狗喂狗函数（喂狗会清除中断标志）。

* 参数:

  * `wdt`: 设备句柄。

* 返回值:

  * 错误码csi_error_t。

------

### csi_wdt_get_remaining_time

```c
uint32_t csi_wdt_get_remaining_time(csi_wdt_t *wdt)
```

* 功能描述:

  * 获取WDT距离超时中断的剩余计时值。

* 参数:

  * `wdt`: 设备句柄。

* 返回值:

  * WDT剩余计时值。

------

### csi_wdt_is_running

```c
bool csi_wdt_is_running(csi_wdt_t *wdt)
```

* 功能描述:

  * 检测WDT是否正在工作。

* 参数:

  * `wdt`: 设备句柄。

* 返回值:

  * ture代表WDT正在工作。

  * false代表WDT工作停止。

------

### csi_wdt_attach_callback

```c
csi_error_t csi_wdt_attach_callback(csi_wdt_t *wdt, void *callback, void *arg)
```

* 功能描述:

  * 注册回调函数。

* 参数:

  * `wdt`: 设备句柄。
  
  * `callback`: 中断回调函数。
  
  * `arg`: 回调函数对应的传参，由用户自定义。

* 返回值:

  * 错误码csi_error_t。


####  callback

```c
void (*callback)(csi_wdt_t *wdt, void *arg);
```

其中wdt为设备句柄，arg为用户自定义的回调函数对应的参数。 

------

### csi_wdt_detach_callback

```c
void csi_wdt_detach_callback(csi_wdt_t *wdt)
```

* 功能描述:

  * 注销回调函数。

* 参数:

  * `wdt`: 设备句柄。
  
* 返回值：

  * 无。

------

## 初始化示例

```c
#include <stdio.h>
#include <soc.h>
#include <drv/wdt.h>

static csi_wdt_t g_wdt;

int main(void)
{
    csi_error_t ret = 0;

    /* init wdt */
    ret = csi_wdt_init(&g_wdt, 0);

    return ret;
}
```

## 正常喂狗示例

```c
#include <stdio.h>
#include <soc.h>
#include <drv/wdt.h>

/**
 * The exact time-out can be calculated according to the formula
 * freq = 67500000 Hz
 * user changeable para is 10 (range is 0 ~ 15)
 * 994ms = ((0x10000 << 10)/ (67500000 / 1000))
 */
#define WDT_TIMEOUT_MS                  (994U)               ///< timeout: 0.94s

#define CHECK_RETURN(ret)                           \
        do {                                        \
            if (ret != 0) {                         \
                return -1;                          \
            }                                       \
        } while(0);

static csi_wdt_t g_wdt;
static uint8_t cb_wdt_flag = 0;

static void wdt_event_cb_fun(csi_wdt_t *wdt, void *arg)
{
    (void)arg;
    cb_wdt_flag = 1;
}

int main(void)
{
    csi_error_t ret = 0;

    /* STEP 1: init wdt */
    ret = csi_wdt_init(&g_wdt, 0);
    CHECK_RETURN(ret);

    /* STEP 2: register callback func */
    ret = csi_wdt_attach_callback(&g_wdt, wdt_event_cb_fun, NULL);
    CHECK_RETURN(ret);

    /* STEP 3: set timeout time(994ms) */
    ret = csi_wdt_set_timeout(&g_wdt, WDT_TIMEOUT_MS);
    CHECK_RETURN(ret);

    cb_wdt_flag = 0;

    /* STEP 4: start work */
    ret = csi_wdt_start(&g_wdt);
    CHECK_RETURN(ret);

    /* STEP 5: delay 993ms */
    mdelay(WDT_TIMEOUT_MS - 1);

    /* STEP 6:  system should not enters the interrupt function */
    if (cb_wdt_flag) {
        ret = -1;
    }

    /* STEP 7:  feed wdt */
    ret = csi_wdt_feed(&g_wdt);
    CHECK_RETURN(ret);

    return ret;
}
```

## 系统复位示例

```c
#include <stdio.h>
#include <soc.h>
#include <drv/wdt.h>

/**
 * The exact time-out can be calculated according to the formula
 * freq = 67500000 Hz
 * user changeable para is 10 (range is 0 ~ 15)
 * 994ms = ((0x10000 << 10)/ (67500000 / 1000))
 */
#define WDT_TIMEOUT_MS                  (994U)               ///< timeout: 0.94s

#define CHECK_RETURN(ret)                           \
        do {                                        \
            if (ret != 0) {                         \
                return -1;                          \
            }                                       \
        } while(0);

static csi_wdt_t g_wdt;
static uint8_t cb_wdt_flag = 0;

static void wdt_event_cb_fun(csi_wdt_t *wdt, void *arg)
{
    (void)arg;
    cb_wdt_flag = 1;
    csi_wdt_feed(wdt);
}

int main(void)
{
    csi_error_t ret = 0;

    /* STEP 1: init wdt */
    ret = csi_wdt_init(&g_wdt, 0);
    CHECK_RETURN(ret);

    /* STEP 2: register callback func */
    ret = csi_wdt_attach_callback(&g_wdt, wdt_event_cb_fun, NULL);
    CHECK_RETURN(ret);

    /* STEP 3: set timeout time(994ms) */
    ret = csi_wdt_set_timeout(&g_wdt, WDT_TIMEOUT_MS);
    CHECK_RETURN(ret);

    cb_wdt_flag = 0;

    /* STEP 4: start work */
    ret = csi_wdt_start(&g_wdt);
    CHECK_RETURN(ret);

    /* STEP 5: delay 1194ms */
    mdelay(WDT_TIMEOUT_MS + 200);

    /* STEP 6:  system should enters the interrupt function */
    if (0 == cb_wdt_flag) {
        ret = -1;
    }

    /* STEP 7: start work */
    csi_wdt_detach_callback(&g_wdt);

    /* STEP 8: delay 995ms */
    mdelay(WDT_TIMEOUT_MS + 1);

    /* !!!system should be restarted, and not be here! */
    ret = -1;
    
    return ret;
}
```

