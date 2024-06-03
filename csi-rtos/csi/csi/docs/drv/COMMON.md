# COMMON

## 设备说明

CSI的通用接口、结构体定义。

## 宏与枚举列表

| 函数               | 说明                  |
| :---------------- | :-------------------- |
| CSI_ASSERT     | 参数判空，如果为空，则程序停止|
| CSI_PARAM_CHK  | 参数判空，如果为空， 返回用户传入的第二个参数 |
| CSI_PARAM_CHK_NORETVAL | 参数判空，如果为空， 直接返回 |
| HANDLE_REG_BASE   | 返回句柄中的设备基址 |
| HANDLE_IRQ_NUM   | 返回句柄中的设备中断号 |
| HANDLE_DEV_IDX   | 返回句柄中的设备ID |
| HANDLE_IRQ_HANDLER   | 返回句柄中的设备中断服务函数 |
| csi_error_t     | CSI错误码 |
| csi_pm_dev_state_t   | 设备的电源控制状态 |
| csi_pm_mode_t   | 电源控制的模式 |
| csi_dev_tag_t   | 设备标签，区分设备类型和厂商 |

## 结构体列表

| 结构体             | 说明                  |
| :---------------- | :-------------------- |
| csi_dev_t         | CSI设备描述 |
| csi_pm_dev_t      | CSI电源控制描述   |
| csi_state_t       | CSI设备的读写状态 |
| csi_perip_info_t  | 设备信息存储结构，用于芯片对接  |

## 接口列表

| 函数               | 说明                  |
| :---------------- | :-------------------- |
| target_get     | 获取设备基本信息描述（中断号，基址等）|
| target_get_optimal_dma_channel   | 根据父设备，获取DMA控制器中的最优通道|
| mdelay      | 毫秒分辨率的延时函数|
| udelay   | 微秒分辨率的延时函数 |

------

## 宏与枚举详细说明

### CSI_ASSERT
```c
#ifdef CONFIG_DEBUG_MODE
#define CSI_ASSERT(expr)                            \
        do {                                        \
            if ((int32_t)expr == (int32_t)NULL) {   \
                printf("PROGRAM ASSERT\n");         \
                while(1);                           \
            }                                       \
        } while(0);
#else
#define CSI_ASSERT(expr)        ((void)0U)
#endif
};
```
* 功能描述:

  * 用于判空，如果为空，则程序停止，此功能由宏CONFIG_DEBUG_MODE控制。

* 使用场景：

  * 一般用于函数中的参数判断。

* 使用举例 
```c
static void func_a(uint8_t *buffer)
{
    /* 如果此时打开了宏CONFIG_DEBUG_MODE，当判断参数buffer为空时，则程序会暂停以报警 */
    CSI_ASSERT(buffer);
}
```

### CSI_PARAM_CHK
```c
#ifdef CONFIG_PARAM_NOT_CHECK
#define CSI_PARAM_CHK(para, err)                    \
    do {                                            \
        if ((int32_t)para == (int32_t)NULL) {       \
            return (err);                           \
        }                                           \
    } while (0)
#else
#define CSI_PARAM_CHK(para, err)
#endif
```
* 功能描述:

  * 用于判空，如果传入的第一个参数`para`为空，则返回用户传入的第二个参数`err`。

* 使用场景：

  * 一般用于函数中的参数判断，服务于带返回值的接口。

* 使用示例
```c
csi_error_t func_a(uint8_t *buffer)
{
    /* 如果buffer为空，会返回第二个参数（这里是CSI_ERROR），以告知用户 */
    CSI_PARAM_CHK(buffer, CSI_ERROR);
}
```

### CSI_PARAM_CHK_NORETVAL
```c
#ifdef CONFIG_PARAM_NOT_CHECK
#define CSI_PARAM_CHK_NORETVAL(para)                \
    do {                                            \
        if ((int32_t)para == (int32_t)NULL) {       \
            return;                                 \
        }                                           \
    } while (0)
#else
#define CSI_PARAM_CHK_NORETVAL(para)
#endif
```
* 功能描述:

  * 用于判空，如果传入的`para`为空，则直接退出。

* 使用场景：

  * 一般用于函数中的参数判断，服务于无返回值的接口。

* 使用示例
```c
void func_a(uint8_t *buffer)
{
    /* 如果buffer为空，函数会直接退出 */
    CSI_PARAM_CHK_NORETVAL(buffer);
}
```

### HANDLE_REG_BASE
```c
#define HANDLE_REG_BASE(handle)     (handle->dev.reg_base)
```
* 功能描述:

  * 返回传入句柄的设备基址。

### HANDLE_IRQ_NUM
```c
#define HANDLE_IRQ_NUM(handle)      (handle->dev.irq_num)
```
* 功能描述:

  * 返回传入句柄的设备中断号。

### HANDLE_DEV_IDX
```c
#define HANDLE_DEV_IDX(handle)      (handle->dev.idx)
```
* 功能描述:

  * 返回传入句柄的设备ID。

### HANDLE_IRQ_HANDLER
```c
#define HANDLE_IRQ_HANDLER(handle)  (handle->dev.irq_handler)
```
* 功能描述:

  * 返回传入句柄的设备中断服务函数。

### csi_error_t

| 类型 | 说明     |
| :------------- | :--------- |
| CSI_OK    | 成功 |
| CSI_ERROR | 错误 |
| CSI_BUSY  | 忙碌 |
| CSI_TIMEOUT | 超时 |
| CSI_UNSUPPORTED | 不支持 |
> 用于返回CSI接口的调用情况

### csi_pm_dev_action_t
| 类型 | 说明     |
| :------------- | :-------- |
| PM_DEV_SUSPEND | 设备挂起   |
| PM_DEV_RESUME  | 设备恢复   |
> 在低功耗应用场景中，用于设置设备的行为模式

### csi_pm_mode_t
| 类型 | 说明     |
| :------------- | :--------- |
| PM_MODE_RUN | 设备正常运行模式 |
| PM_MODE_SLEEP_1 | 设备1级睡眠 |
| PM_MODE_SLEEP_2 | 设备2级睡眠 |
| PM_MODE_DEEP_SLEEP_1 | 设备1级深度睡眠 |
| PM_MODE_DEEP_SLEEP_2 | 设备2级深度睡眠 |
> 在低功耗应用场景中，对设备进行模式设置


### csi_dev_tag_t
| 类型 | 说明     |
| :------------- | :--------- |
| DEV_DW_UART_TAG | DW厂商的UART设备标签 |
| DEV_DW_TIMER_TAG | DW厂商的TIMER设备标签 |
| ...... | 持续扩充中 |

> 用于区分不同的外设类型


## 结构体详细说明

### csi_dev_t

```c
typedef struct csi_dev csi_dev_t;
struct csi_dev {
    unsigned long reg_base;
    uint8_t       irq_num;
    uint8_t       idx;
    uint16_t      dev_tag;
    void          (*irq_handler)(void *);
#ifdef CONFIG_PM
    csi_pm_dev_t pm_dev;
#endif
};
```

| 成员              | 类型      | 说明    |
| :------------- | :--------- | :--------- |
| reg_base | unsigned long | 设备基址     |
| irq_num | uint8_t | 设备中断号   |
| idx | uint8_t | 设备ID    |
| dev_tag | uint16_t | 设备标签，区分设备类型和厂商      |
| irq_handler | void (*irq_handler)(void *) | 设备中断服务函数 |
| pm_dev | csi_pm_dev_t | CSI的电源控制描述 |

> 本结构体为CSI设备的基础描述，包含中断号、基址等信息，其中的`pm_dev`电源控制描述是可选项。

------

### csi_pm_dev_t

```c
typedef struct {
    slist_t     next;
    csi_error_t (*pm_action)(csi_dev_t *dev, csi_pm_dev_action_t action);
    uint32_t    *reten_mem;
    uint32_t    size;
} csi_pm_dev_t;
```
| 成员            | 类型       | 说明        |
| :------------- | :--------- | :--------- |
| next | slist_t | 设备描述链表指针 |
| pm_action | csi_error_t (*pm_action)(csi_dev_t *dev, csi_pm_dev_action_t action) | 电源控制事件回调函数     |
| reten_mem | uint32_t | 待保存空间的地址 |
| size | uint32_t | 待保存空间的容量  |

### csi_state_t
```c
typedef struct {
   uint8_t    readable;
   uint8_t    writeable;
   uint8_t    error;
} csi_state_t;
```

| 成员            | 类型       | 说明        |
| :------------- | :--------- | :--------- |
| readable | uint8_t | 可读状态，为1时代表可读，为0时代表不可读 |
| writeable | uint8_t | 可写状态，为1时代表可写，为0时代表不可写 |
| error | uint8_t | 错误状态，为1表示错误，为0表示没有错误  |

> 此结构体作用于CSI接口的异步读写操作，用户在进行异步读写时，可以调用csi_xx_get_state接口获取设备的当前状态来判断是否进行读写操作。

### csi_perip_info_t

```c
typedef struct {
    unsigned long reg_base;
    uint8_t  irq_num;
    uint8_t  idx;
    uint16_t dev_tag;
} csi_perip_info_t;
```

| 成员            | 类型       | 说明        |
| :------------- | :--------- | :--------- |
| reg_base | unsigned long | 设备寄存器基址 |
| irq_num | uint8_t | 设备中断号    |
| idx | uint8_t | 设备ID  |
| dev_tag | uint16_t | 设备标签  |

> 此结构体用于芯片对接时，存储外设的基本信息。
> 在工程中，一款芯片的devices.c中，存在外设信息列表const csi_perip_info_t g_soc_info[]，这个是芯片对接者需要填充的。

------

## 接口详细说明


### target_get

```c
csi_error_t target_get(csi_dev_tag_t dev_tag, uint32_t idx, csi_dev_t *dev)
```

* 功能描述:

  * 通过用户传入的设备标签，获取此设备的基本信息。

* 参数：

  * `dev_tag`:设备标签，由用户传入
  * `idx`: 设备ID，同种设备可能同时存在多个，因此需要ID作区分，如UART0的idx=0，UART1的idx=1
  * `dev`: 设备信息，将设备基址，中断号等信息返回给用户

* 返回值:

  * 错误码csi_error_t

------

### target_get_optimal_dma_channel

```c
csi_error_t target_get_optimal_dma_channel(void *dma_list, uint32_t ctrl_num, csi_dev_t *parent_dev, void *ch_info)
```

* 功能描述:

  * 通过用户传入的设备信息，获取当前最优的DMA通道。

* 传参：

  * `dma_list`: dma控制器句柄数组指针
  * `ctrl_num`: dma控制器数量
  * `parent_dev`: dma通道将要服务的对象设备信息
  * `ch_info`: 用于返回的通道信息

* 返回值:

  * 错误码csi_error_t

------

### mdelay

```c
void mdelay(uint32_t ms)
```

* 功能描述:

  * 延时一段时间，单位是毫秒。

* 参数：

  * `ms`: 需要延时的时间量，单位毫秒

------

### udelay

```c
void udelay(uint32_t us)
```

* 功能描述:

  * 延时一段时间，单位是微秒。

* 参数：

  * `ms`: 需要延时的时间量，单位微秒

------
