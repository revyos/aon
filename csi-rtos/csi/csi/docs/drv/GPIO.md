# GPIO设备
-----
## 设备说明

GPIO(General-purpose input/output) 通用型之输入输出的简称。在嵌入式系统中，经常需要控制许多结构简单的外部设备或者电路，这些设备有的需要通过CPU控制，有的需要CPU提供输入信号。并且，许多设备或电路只要求有开/关两种状态就够了，比如LED的亮与灭。对这些设备的控制，使用传统的串口或者并口就显得比较复杂，所以，在嵌入式微处理器上通常提供了一种“通用可编程I/O端口”，也就是GPIO。

## 接口列表
GPIO的CSI接口说明如下所示：

|   函数                  |     说明                 |
|-------------------------|--------------------------|
|csi_gpio_init            |GPIO设备初始化            |
|csi_gpio_uninit          |GPIO设备反初始化          |
|csi_gpio_dir             |GPIO配置输入/输出配置     |
|csi_gpio_mode            |GPIO配置引脚模式          |
|csi_gpio_irq_mode        |配置GPIO中断模式          |
|csi_gpio_irq_enable      |使能引脚中断              |
|csi_gpio_debonce         |设置debonce模式           |
|csi_gpio_write           |设置引脚的电平状态        |
|csi_gpio_read            |读取引脚的电平状态        |
|csi_gpio_toggle          |翻转引脚电平状态          |
|csi_gpio_attach_callback |注册回调函数              |
|csi_gpio_detach_callback |注销回调函数              |

## 接口详细说明

### csi_gpio_init

```c
csi_error_t csi_gpio_init(csi_gpio_t *gpio, uint32_t port_idx)
```

- 功能描述:

  - 通过设备ID初始化对应的GPIO实例。

- 参数:

  - `gpio`: 设备句柄（需要用户申请句柄空间）。
  - `port_idx`:  设备ID。

- 返回值:


  - 错误码csi_error_t

* * *
#### csi_gpio_t

| 成员 | 类型      | 说明                   |
| ---- | --------- | ---------------------- |
| dev  | csi_dev_t | 设备统一句柄           |
| callback | void (*callback)(csi_gpio_t *gpio, uint32_t pins, void *arg) | 用户回调函数           |
| arg | void*     | 用户回调函数对应的传参 |
| priv| void*     | 设备私有变量       |

### csi_gpio_uninit

```c
csi_error_t csi_gpio_uninit(csi_gpio_t *gpio)
```

- 功能描述:

  - GPIO实例反初始化。
  - 该接口会释放所有IO口,恢复为默认状态停止GPIO，并且释放相关的软硬件资源。

- 参数:

  - `gpio`: 实例句柄。

- 返回值:


  - 错误码csi_error_t

### csi_gpio_dir

```c
csi_error_t csi_gpio_dir(csi_gpio_t *gpio, uint32_t pin_mask, csi_gpio_dir_t dir)
```

- 功能描述:

  - GPIO输入/输出模式配置

- 参数

  - `gpio` : 实例句柄。
  - `pin_mask`: bit位掩码，指定需要设置的bit位，如:0x00ff，代表设置pin0~pin7。
  - `dir`: 输入/输出模式。

- 返回值:


  - 错误码csi_error_t

* * *
#### csi_gpio_dir_t

| 类型                  | 说明     |
| --------------------- | -------- |
| GPIO_DIRECTION_INPUT  | 输入模式 |
| GPIO_DIRECTION_OUTPUT | 输出模式 |


### csi_gpio_mode

```c
csi_error_t csi_gpio_mode(csi_gpio_t *gpio, uint32_t pin_mask, csi_gpio_mode_t mode)
```

- 功能描述:

  - 配置GPIO的引脚模式。
- 参数:

  - `gpio`: 实例句柄。
  - `pin_mask`: bit位掩码，指定需要设置的bit位，如:0x00ff，代表设置pin0~pin7。
  - `mode`: 引脚模式。

- 返回值:

  - 错误码csi_error_t

- 返回值策略说明：
  - 1.传入的引脚中如果有一个引脚配置失败，函数直接退出并返回CSI_ERROR，剩下引脚不配置。
  - 2.传入的引脚中有一个或者多个UNSUPPORT，函数返回CSI_UNSUPPORT，但其它引脚依然会配置。


* * *
#### csi_gpio_mode_t

| 类型                 | 说明     |
| -------------------- | -------- |
| GPIO_MODE_PULLNONE   | 悬空输入 |
| GPIO_MODE_PULLUP     | 上拉输入 |
| GPIO_MODE_PULLDOWN   | 下拉输入 |
| GPIO_MODE_OPEN_DRAIN | 开漏输出 |
| GPIO_MODE_PUSH_PULL  | 推挽输出 |

### csi_gpio_irq_mode

```c
csi_error_t csi_gpio_irq_mode(csi_gpio_t *gpio, uint32_t pin_mask, csi_gpio_irq_mode_t mode)
```

- 功能描述:

  - 配置GPIO的中断模式。

- 参数:

  - `gpio`: 实例句柄。
  - `pin_mask`: bit位掩码，指定需要设置的bit位，如:0x00ff，代表设置pin0~pin7。
  - `mode`: 中断模式。

- 返回值:


  - 错误码csi_error_t

* * *
#### csi_gpio_irq_mode_t

| 类型                       | 说明           |
| -------------------------- | -------------- |
| GPIO_IRQ_MODE_RISING_EDGE  | 上升沿中断模式 |
| GPIO_IRQ_MODE_FALLING_EDGE | 下降沿中断模式 |
| GPIO_IRQ_MODE_BOTH_EDGE    | 双边沿模式     |
| GPIO_IRQ_MODE_LOW_LEVEL    | 低电平模式     |
| GPIO_IRQ_MODE_HIGH_LEVEL   | 高电平模式     |

### csi_gpio_irq_enable

```c
csi_error_t csi_gpio_irq_enable(csi_gpio_t *gpio, uint32_t pin_mask, bool enable)
```

- 功能描述:

  - 使能/禁止引脚中断。

- 参数:

  - `gpio`: 实例句柄。
  - `pin_mask`: bit位掩码，指定需要设置的bit位，如:0x00ff代表，设置pin0~pin7。
  - `enable`: 使能标志位。true: 使能， false: 禁止。

- 返回值:


  - 错误码csi_error_t

### csi_gpio_debonce

```c
csi_error_t csi_gpio_debonce(csi_gpio_t *gpio, uint32_t pin_mask, bool enable)
```

- 功能描述:

  - 设置debonce模式。

- 参数:

  - `gpio`: 实例句柄。
  - `pin_mask`: bit位掩码，指定需要设置的bit位，如:0x00ff，代表设置pin0~pin7。
  - `enable`: 使能标志位。true: 开启去抖功能，false：关闭去抖功能。

- 返回值:


  - 错误码csi_error_t

### csi_gpio_write

```c
void  csi_gpio_write(csi_gpio_t *gpio, uint32_t pin_mask, csi_gpio_pin_state_t value)
```

- 功能描述:

  - 设置引脚的电平状态。

- 参数:

  - `gpio`: 实例句柄。
  - `pin_mask`: bit位掩码，指定需要设置的bit位，如:0x00ff，代表设置pin0~pin7。
  - `value`: 引脚状态，定义见csi_gpio_pin_state_t。

- 返回值: 无

* * *
#### csi_gpio_pin_state_t

| 类型          | 说明   |
| ------------- | ------ |
| GPIO_PIN_LOW  | 低电平 |
| GPIO_PIN_HIGH | 高电平 |


### csi_gpio_toggle

```c
void csi_gpio_toggle(csi_gpio_t *gpio, uint32_t pin_mask)
```

- 功能描述:

  - 翻转引脚的电平状态。

- 参数:

  - `gpio`: 实例句柄。
  - `pin_mask`: bit位掩码，指定需要设置的bit位，如:0x00ff，代表设置pin0~pin7。

- 返回值: 无

### csi_gpio_read

```c
uin32_t csi_gpio_read(csi_gpio_t *gpio, uint32_t pin_mask)
```

- 功能描述:

  - 读取指定引脚掩码的电平状态。

- 参数:

  - `gpio`: 实例句柄。
  - `pin_mask`: bit位掩码，指定需要设置的bit位，如:0x00ff，代表设置pin0~pin7。

- 返回值:

  - 根据位掩码，得到对应的引脚状态。

### csi_gpio_attach_callback

```c
csi_error_t csi_gpio_attach_callback(csi_gpio_t *gpio, void *callback, void *arg)
```

- 功能描述:

  - 注册回调函数。

- 参数:

  - `gpio`: 实例句柄。
  - `callback`: GPIO实例的事件回调函数（一般在中断上下文执行）。
  - `arg`: 回调函数的参数。

- 参数: 


  - 错误码csi_error_t

### csi_gpio_detach_callback

```c
void csi_gpio_detach_callback(csi_gpio_t *gpio)
```

- 功能描述:

  - 注销回调函数。

- 参数:

  - `gpio`: 实例句柄。

## 电平翻转使用示例

```c
#include <stdio.h>

#include <soc.h>
#include <drv/gpio.h>
#include <drv/tick.h>
#include <board_config.h>
#include <board_init.h>

// Select pin29 and pin30
#define EXAMPLE_GPIO_PIN_MASK      ( ( 1 << 29 ) | ( 1 << 30  ) )

#define GPIO_CHECK_RETURN(ret)           \
    do {                                 \
        if (ret != CSI_OK) {             \
            return -1;                   \
        }                                \
    } while(0);

static csi_gpio_t gpio;

int main(void)
{
    uint32_t tmp = 0;
    int ret;

    board_init();

    /* Initialize GPIO periphrial */
    ret = csi_gpio_init(&gpio,  EXAMPLE_TOGGLE_GPIO_IDX);
    GPIO_CHECK_RETURN(ret);

    /* Set input mode */
    ret = csi_gpio_dir(&gpio,   EXAMPLE_GPIO_PIN_MASK, GPIO_DIRECTION_OUTPUT);
    GPIO_CHECK_RETURN(ret);

    ret = csi_gpio_write(&gpio, EXAMPLE_GPIO_PIN_MASK,  GPIO_PIN_LOW);
    GPIO_CHECK_RETURN(ret);
    while (1) {
        csi_gpio_toggle(&gpio, EXAMPLE_GPIO_PIN_MASK);
        tmp ^= 1;
        printf("gpio set to: %s \r\n", (tmp == 1) ? "high level" : "low level");
        mdelay(1000);
    }
}
```

## 中断模式使用示例

```c
#include <stdio.h>

#include <soc.h>
#include <drv/gpio.h>
#include <drv/tick.h>
#include <board_config.h>
#include <board_init.h>

// Select pin5 and pin6
#define EXAMPLE_GPIO_PIN_MASK      ( ( 1 << 5 ) | ( 1 << 6  ) )

#define GPIO_CHECK_RETURN(ret)           \
    do {                                 \
        if (ret != CSI_OK) {             \
            return -1;                   \
        }                                \
    } while(0);

volatile static bool intr_flag = false;
static csi_gpio_t gpio;

static void gpio_interrupt_handler(csi_gpio_t *gpio, uint32_t pin_mask, void *arg)
{
    intr_flag = true;
}

int main(void)
{
    int ret;

    board_init();

    /* Initialize GPIO periphrial */
    ret = csi_gpio_init(&gpio, EXAMPLE_INTR_GPIO_IDX);
    GPIO_CHECK_RETURN(ret);

    /* Attach callback */
    ret = csi_gpio_attach_callback(&gpio, gpio_interrupt_handler, NULL);
    GPIO_CHECK_RETURN(ret);

    /* Set pull-up mode */
    ret = csi_gpio_mode(&gpio,EXAMPLE_GPIO_PIN_MASK, GPIO_MODE_PULLUP);
    GPIO_CHECK_RETURN(ret);

    /* Set input mode */
    ret = csi_gpio_dir(&gpio, EXAMPLE_GPIO_PIN_MASK, GPIO_DIRECTION_INPUT);
    GPIO_CHECK_RETURN(ret);

    /* Set falling-edge triger mode */
    ret = csi_gpio_irq_mode(&gpio, EXAMPLE_GPIO_PIN_MASK, GPIO_IRQ_MODE_FALLING_EDGE);
    GPIO_CHECK_RETURN(ret);

    /* Enable irq */
    ret = csi_gpio_irq_enable(&gpio, EXAMPLE_GPIO_PIN_MASK, true);
    GPIO_CHECK_RETURN(ret);

    printf("please change the gpio pin from high to low\n");

    while (1) {
        if (intr_flag) {
            printf("gpio pin passed!!!\n");
            intr_flag = false;
        }
    }
}

```
