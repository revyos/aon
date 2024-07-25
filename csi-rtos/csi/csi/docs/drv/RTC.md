# RTC

## 说明
RTC （Real-Time Clock）实时时钟可以提供精确的实时时间，它可以用于产生年、月、日、时、分、秒等信息。目前实时时钟芯片大多采用精度较高的晶体振荡器作为时钟源。有些时钟芯片为了在主电源掉电时还可以工作，会外加电池供电，使时间信息一直保持有效。

## 接口列表

RTC的CSI接口如下所示：

| 函数                             | 说明                       |
| -------------------------------- | -------------------------- |
| csi_rtc_init                     | 初始化                     |
| csi_rtc_uninit                   | 反初始化                   |
| csi_rtc_set_time                 | 设置时间（需要同步时间）   |
| csi_rtc_set_time_no_wait         | 设置时间（不需要同步时间） |
| csi_rtc_get_time                 | 获取当前时间               |
| csi_rtc_get_alarm_remaining_time | 获取距离闹钟剩余时间       |
| csi_rtc_set_alarm                | 设置闹钟                   |
| csi_rtc_cancel_alarm             | 取消闹钟                   |
| csi_rtc_is_running               | 获取工作状态               |

## 接口详细说明

### csi_rtc_init

```c
csi_error_t csi_rtc_init(csi_rtc_t *rtc, uint32_t idx)
```

* 功能描述:

  * 通过设备ID初始化对应的RTC实例。

* 参数:
  
  * `rtc`: 设备句柄（需要用户申请句柄空间）。

  * `idx`: 设备ID 。

* 返回值:

  * 错误码csi_error_t。

---

#### csi_rtc_t

| 成员              | 类型      | 说明    |
| :------------- | :--------- | :--------- |
| dev | csi_dev_t | csi设备统一句柄     |
| callback | void (*callback)(csi_rtc_t *rtc, void *arg) | 用户回调函数     |
| arg | void * | 回调函数对应传参      |
| priv | void * | 设备私有变量 |

---

### csi_rtc_uninit

```c
void csi_rtc_uninit(csi_rtc_t *rtc)
```

* 功能描述:

  * RTC实例反初始化，该接口会停止RTC实例正在进行的工作，并且释放相关的软硬件资源。

* 参数:

  * `rtc`: 设备句柄。

* 返回值：
  
  * 无。

---

### csi_rtc_set_time

```c
csi_error_t csi_rtc_set_time(csi_rtc_t *rtc, const csi_rtc_time_t *rtctime);
```

* 功能描述:

  * 通过rtctime设置时间（等待同步时间接口）。

* 参数:

  * `rtc`: 设备句柄。

  * `rtctime`:  rtc设置起始时间年、月、日、时、分、秒。

* 返回值:

  * 错误码csi_error_t。
  

---

#### csi_rtc_time_t

| 成员    | 类型 | 说明                            |
| :------ | :--- | :------------------------------ |
| tm_sec  | int  | 秒      取值范围[0-59]          |
| tm_min  | int  | 分      取值范围[0-59]          |
| tm_hour | int  | 小时   取值范围[0-23]           |
| tm_mday | int  | 天 	取值范围[1-31]           |
| tm_mon  | int  | 月      取值范围[0-11]          |
| tm_year | int  | 年     取值范围[70-199]         |
| tm_wday | int  | 周     取值范围[0-6]            |
| tm_yday | int  | 一年中的第几天  取值范围[0-365] |

例：设置2020年1月13日23点59分59秒则如下配置：

rtctime.tm_year = 120 , rtctime.tm_mon = 0 , rtctime.tm_mday = 13 
rtctime.tm_hour = 23 , rtctime.tm_min = 59 , rtctime.tm_sec = 59 

---

### csi_rtc_set_time_no_wait

```c
csi_error_t csi_rtc_set_time_no_wait(csi_rtc_t *rtc, const csi_rtc_time_t *rtctime)
```

* 功能描述:

  * 通过rtctime设置时间（无需等待同步时间接口）。

* 参数:

  * `rtc`: 设备句柄。

  * `rtctime`:  rtc设置起始时间年、月、日、时、分、秒。

* 返回值:

  * 错误码csi_error_t。
  

> 注意：因硬件不同设计的两种设置时间接口，有些硬件设置时间写入寄存器比较快，直接调用csi_rtc_set_time_no_wait即可快速完成设置时间；有些硬件设置时间写入寄存器响应比较慢，就需要调用csi_rtc_set_time进行设置时间；

### csi_rtc_get_time

```c
csi_error_t csi_rtc_get_time(csi_rtc_t *rtc, csi_rtc_time_t *rtctime)
```

* 功能描述:

  * 获取RTC当前时间。

* 参数:

  * `rtc`: 设备句柄。

  * `rtctime`:  rtc返回当前时间年、月、日、时、分、秒。

* 返回值:

  * 错误码csi_error_t。




### csi_rtc_get_alarm_remaining_time

```c
uin32_t csi_rtc_get_alarm_remaining_time(csi_rtc_t *rtc)
```

* 功能描述:

  * 获取距离下次闹钟间隔时间。

* 参数:

  * `rtc`: 设备句柄。

* 返回值:

  * 闹钟倒计时(单位：s)。

------

### csi_rtc_set_alarm

```c
csi_error_t csi_rtc_set_alarm(csi_rtc_t *rtc, const csi_rtc_time_t *rtctime, void *callback, void *arg)
```

* 功能描述:

  * 设置RTC闹钟时间。

* 参数:

  * `rtc`: 设备句柄。
  
  * `rtctime`:  设置rtc闹钟时间年、月、日、时、分、秒。
  
  * `callback`：rtc实例的事件回调函数（一般在中断上下文执行）。
  
  * `arg`: 回调函数参数（可选，由用户定义）。

* 返回值:

  * 错误码csi_error_t。


####  callback

```c
void (*callback)(csi_rtc_t *rtc, void *arg);
```

其中rtc为设备句柄，arg为用户自定义的回调函数对应的参数。 

---

### csi_rtc_cancel_alarm

```c
csi_error_t csi_rtc_cancel_alarm(csi_rtc_t *rtc)
```

* 功能描述:

  * 取消闹钟控制。

* 参数:

  * `rtc`: 设备句柄。

* 返回值:

  * 错误码csi_error_t。


------

###  csi_rtc_is_running

```c
bool csi_rtc_is_running(csi_rtc_t *rtc)
```

* 功能描述:

  * 判断RTC是否还在工作。

* 参数:

  * `rtc`: 设备句柄。

* 返回值:

  * ture代表RTC正在工作。

  * false代表RTC工作停止。

-----

## 初始化示例

```c
#include <stdio.h>
#include <soc.h>
#include <drv/rtc.h>

static csi_rtc_t g_rtc;

int main(void) 
{
    csi_error_t ret = 0;

    ret = csi_rtc_init(&g_rtc, 0);

    return ret;
}
```

## 设置时间/读取时间示例

```c
#include <stdio.h>
#include <soc.h>
#include <drv/rtc.h>

static csi_rtc_t g_rtc;

#define CHECK_RETURN(ret)                           \
        do {                                        \
            if (ret != 0) {                         \
                return -1;                          \
            }                                       \
        } while(0);

int main(void) 
{
    csi_error_t ret;
    csi_rtc_time_t last_time, base_time;

    /* STEP 1: init rtc */
    ret = csi_rtc_init(&g_rtc, 0);
    CHECK_RETURN(ret);

    base_time.tm_year    = 120;
    base_time.tm_mon     = 0;
    base_time.tm_mday    = 5;
    base_time.tm_hour    = 23;
    base_time.tm_min     = 59;
    base_time.tm_sec     = 55;

    /* STEP 2: set base time */
    ret = csi_rtc_set_time(&g_rtc, &base_time);
    CHECK_RETURN(ret);

    /* STEP 3: delay 10s */
    mdelay(10000);

    /* STEP 4: read time */
    ret = csi_rtc_get_time(&g_rtc, &last_time);
    CHECK_RETURN(ret);

    /* STEP 5: check time(add 10 sec)   There is a certain error */
    CHECK_RETURN((last_time.tm_year - 120));
    CHECK_RETURN((last_time.tm_mon - 0));
    CHECK_RETURN((last_time.tm_mday - 6));
    CHECK_RETURN((last_time.tm_hour - 0));
    CHECK_RETURN((last_time.tm_min - 0));
    CHECK_RETURN((last_time.tm_sec - 5));

    /* STEP 6: uinit rtc */
    csi_rtc_uninit(&g_rtc);

    return ret;
}
```

## 设置闹钟示例

```c
#include <stdio.h>
#include <soc.h>
#include <drv/rtc.h>

static csi_rtc_t g_rtc;
static uint8_t cb_rtc_flag;

#define CHECK_RETURN(ret)                           \
        do {                                        \
            if (ret != 0) {                         \
                return -1;                          \
            }                                       \
        } while(0);

static void rtc_event_cb_fun(csi_rtc_t *rtc, void *arg)
{
    cb_rtc_flag = 1;
}

int main(void) 
{
    csi_error_t ret;
    csi_rtc_time_t last_time, base_time, alarm_time;

    /* STEP 1: init rtc */
    ret = csi_rtc_init(&g_rtc, 0);
    CHECK_RETURN(ret);

    base_time.tm_year    = 120;
    base_time.tm_mon     = 0;
    base_time.tm_mday    = 5;
    base_time.tm_hour    = 23;
    base_time.tm_min     = 59;
    base_time.tm_sec     = 55;

    /* STEP 2: set base time */
    ret = csi_rtc_set_time(&g_rtc, &base_time); 
    CHECK_RETURN(ret);

    alarm_time.tm_year    = 120;
    alarm_time.tm_mon     = 0;
    alarm_time.tm_mday    = 6;
    alarm_time.tm_hour    = 0;
    alarm_time.tm_min     = 0;
    alarm_time.tm_sec     = 5;

    cb_rtc_flag = 0;

    /* STEP 3: set alarm time */
    ret = csi_rtc_set_alarm(&g_rtc, &alarm_time, rtc_event_cb_fun, NULL);
    CHECK_RETURN(ret);

    /* STEP 4: delay 15s, Trigger alarm interrupt in delay time */
    mdelay(15000);

    if (cb_rtc_flag == 1) {

        /* STEP 5: get the current time */
        ret = csi_rtc_get_time(&g_rtc, &last_time);
        CHECK_RETURN(ret);

        /* STEP 6: check time(add 15 sec)   There is a certain error */
        CHECK_RETURN((last_time.tm_year - 120));
        CHECK_RETURN((last_time.tm_mon - 0));
        CHECK_RETURN((last_time.tm_mday - 6));
        CHECK_RETURN((last_time.tm_hour - 0));
        CHECK_RETURN((last_time.tm_min - 0));
        CHECK_RETURN((last_time.tm_sec - 10));

        ret = csi_rtc_cancel_alarm(&g_rtc);
        CHECK_RETURN(ret);

    } else {
        ret = -1;
    }
    /* STEP 7: uinit rtc */
    csi_rtc_uninit(&g_rtc);

    return ret;
}

```
