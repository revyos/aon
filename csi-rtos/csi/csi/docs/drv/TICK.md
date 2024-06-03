# TICK

## 设备说明

TICK设备用于系统计时，计时从csi_tick_init被调用时开始，目前TICK设备支持使用coretim或者普通的CSI-Timer两种方式。

## 接口列表

| 函数               | 说明                  |
| :---------------- | :-------------------- |
| csi_tick_init     | TICK设备初始化，并开始计时|
| csi_tick_uninit   | TICK设备去初始化，并停止计时|
| csi_tick_get      | 获取tick数，tick单位由CONFIG_SYSTICK_HZ决定|
| csi_tick_get_ms   | 获取开始计时起，经过的时间，单位毫秒 |
| csi_tick_get_us   | 获取开始计时起，经过的时间，单位微秒 |
| csi_tick_increase | tick的自增接口 |

## 接口详细说明

### csi_tick_init

```c
csi_error_t csi_tick_init(void)
```

* 功能描述:

  * TICK设备初始化，并开始计时。

* 返回值:

  * 成功返回CSI_OK，失败CSI_ERROR。

#### CONFIG_SYSTICK_HZ
这个宏可以由用户自行定义，也可以使用默认的定义值100。TICK的单位取决于CONFIG_SYSTICK_HZ，如果CONFIG_SYSTICK_HZ是100，则每个TICK是10ms。

------

### csi_tick_uninit

```c
void csi_tick_uninit(void)
```

* 功能描述:

  * TICK设备去初始化，并停止计时。

### csi_tick_get

```c
uint32_t csi_tick_get(void)
```

* 功能描述:

  * 获取从开始计时起到当前的TICK数。

* 返回值:

  * TICK数。

------

### csi_tick_get_ms

```c
uint32_t csi_tick_get_ms(void)
```

* 功能描述:

  * 获取从开始计时起到当前的时间，单位是毫秒。

* 返回值:

  * 获取从开始计时起到当前的时间

------

### csi_tick_get_us

```c
uint64_t csi_tick_get_us(void)
```

* 功能描述:

  * 获取从开始计时起到当前的时间，单位是微秒。

* 返回值:

  * 获取从开始计时起到当前的时间

------

### csi_tick_get_us

```c
uint64_t csi_tick_get_us(void)
```

* 功能描述:

  * 获取从开始计时起到当前的时间，单位是微秒。

* 返回值:

  * 获取从开始计时起到当前的时间

------

### csi_tick_increase

```c
void csi_tick_increase(void)
```

* 功能描述:

  * 试SYSTICK计数增加1。用户可以通过此接口自行设计TICK设备的中断处理行为。

------

## 使用示例
```c
#define TICK_CHECK_RETURN(ret)                      \
        do {                                        \
            if (ret != CSI_OK) {                    \
                return -1;                          \
            }                                       \
        } while(0);
int example_tick(void)
{
    csi_error_t ret;
    uint32_t tick, time_ms;
    uint64_t time_us;

    /* init tick， CONFIG_SYSTICK_HZ is 100 */
    ret = csi_tick_init();
    TICK_CHECK_RETURN(ret);

    /* get tick */
    tick = csi_tick_get();
    printf("The tick is %d\n", tick);

    /* get time (ms) */
    time_ms = csi_tick_get_ms();
    printf("The time is %d ms\n", time_ms);

    /* get time (us) */
    time_us = csi_tick_get_ms();
    printf("The time is %lld us\n", time_us);

    /* uninit tick */
    csi_tick_uninit();
    return 0;
}
```
