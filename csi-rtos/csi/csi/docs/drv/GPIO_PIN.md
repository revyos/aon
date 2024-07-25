# GPIO_PIN设备
-----
## 设备说明

GPIO(General-purpose input/output) 通用型之输入输出的简称。在嵌入式系统中，经常需要控制许多结构简单的外部设备或者电路，这些设备有的需要通过CPU控制，有的需要CPU提供输入信号。并且，许多设备或电路只要求有开/关两种状态就够了，比如LED的亮与灭。对这些设备的控制，使用传统的串口或者并口就显得比较复杂，所以，在嵌入式微处理器上通常提供了一种“通用可编程I/O端口”，也就是GPIO。

## 接口列表
GPIO_PIN的CSI接口说明如下所示：

| 函数                         | 说明                  |
| ---------------------------- | --------------------- |
| csi_gpio_pin_init            | PIN设备初始化         |
| csi_gpio_pin_uninit          | PIN设备反初始化       |
| csi_gpio_pin_dir             | 配置引脚输入/输出配置 |
| csi_gpio_pin_mode            | 配置引脚模式          |
| csi_gpio_pin_irq_mode        | 配置引脚中断模式      |
| csi_gpio_pin_irq_enable      | 使能引脚中断          |
| csi_gpio_pin_debonce         | 设置引脚debonce模式   |
| csi_gpio_pin_write           | 设置引脚的电平状态    |
| csi_gpio_pin_read            | 读取引脚的电平状态    |
| csi_gpio_pin_toggle          | 翻转引脚电平状态      |
| csi_gpio_pin_attach_callback | 注册回调函数          |

## 接口详细说明

### csi_gpio_pin_init

```c
csi_error_t csi_gpio_pin_init(csi_gpio_pin_t *pin, pin_name_t pin_name)
```

- 功能描述:

  - 根据指定引脚号，初始化对应的PIN实例。

- 参数:

  - `pin`: 设备句柄（需要用户申请句柄空间）。
  - `pin_name`:  引脚号。

- 返回值:


  - 错误码csi_error_t

* * *
#### csi_gpio_pin_t

| 成员 | 类型      | 说明                   |
| ---- | --------- | ---------------------- |
| gpio | csi_gpio_t | 设备统一句柄 |
| pin_idx  | uint32_t                                         | 引脚号                 |
| callback | void (*callback)(csi_gpio_pin_t *pin, void *arg) | 用户回调函数           |
| arg | void*     | 用户回调函数对应的传参 |

### csi_gpio_pin_uninit

```c
csi_error_t csi_gpio_pin_uninit(csi_gpio_pin_t *pin)
```

- 功能描述:

  - PIN实例反初始化。
  
- 参数:

  - `pin`: 实例句柄。

- 返回值: 无

### csi_gpio_pin_dir

```c
csi_error_t csi_gpio_pin_dir(csi_gpio_pin_t *pin, csi_gpio_dir_t dir)
```

- 功能描述:

  - PIN输入/输出模式配置

- 参数

  - `pin` : 实例句柄。
  - `dir`: 输入/输出模式。
  
- 返回值:


  - 错误码csi_error_t

* * *
#### csi_gpio_dir_t

| 类型                  | 说明     |
| --------------------- | -------- |
| GPIO_DIRECTION_INPUT  | 输入模式 |
| GPIO_DIRECTION_OUTPUT | 输出模式 |


### csi_gpio_pin_mode

```c
csi_error_t csi_gpio_pin_mode(csi_gpio_pin_t *pin, csi_gpio_mode_t mode)
```

- 功能描述:

  - 配置指定引脚的的引脚模式。
- 参数:

  - `gpio`: 实例句柄。
  - `mode`: 引脚模式。
  
- 返回值:

  - 错误码csi_error_t


* * *
#### csi_gpio_mode_t

| 类型                 | 说明     |
| -------------------- | -------- |
| GPIO_MODE_PULLNONE   | 悬空输入 |
| GPIO_MODE_PULLUP     | 上拉输入 |
| GPIO_MODE_PULLDOWN   | 下拉输入 |
| GPIO_MODE_OPEN_DRAIN | 开漏输出 |
| GPIO_MODE_PUSH_PULL  | 推挽输出 |

### csi_gpio_pin_irq_mode

```c
csi_error_t csi_gpio_pin_irq_mode(csi_gpio_pin_t *pin, csi_gpio_irq_mode_t mode)
```

- 功能描述:

  - 配置引脚的中断模式。

- 参数:

  - `pin`: 实例句柄。
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

### csi_gpio_pin_irq_enable

```c
csi_error_t csi_gpio_pin_irq_enable(csi_gpio_pin_t *pin, bool enable)
```

- 功能描述:

  - 使能/禁止引脚中断。

- 参数:

  - `pin`: 实例句柄。
  - `enable`: 使能标志位。true: 使能， false: 禁止。
  
- 返回值:


  - 错误码csi_error_t

### csi_gpio_pin_debonce

```c
csi_error_t csi_gpio_pin_debonce(csi_gpio_pin_t *pin, bool enable); 
```

- 功能描述:

  - 设置debonce模式。

- 参数:

  - `pin`: 实例句柄。
  - `enable`: 使能标志位。true: 开启去抖功能，false：关闭去抖功能。
  
- 返回值:


  - 错误码csi_error_t

### csi_gpio_pin_write

```c
void csi_gpio_pin_write(csi_gpio_pin_t *pin, csi_gpio_pin_state_t value)
```

- 功能描述:

  - 设置引脚的电平状态。

- 参数:

  - `pin`: 实例句柄。
  - `value`: 引脚状态，定义见csi_gpio_pin_state_t。
  
- 返回值: 无

* * *
#### csi_gpio_pin_state_t

| 类型          | 说明   |
| ------------- | ------ |
| GPIO_PIN_LOW  | 低电平 |
| GPIO_PIN_HIGH | 高电平 |


### csi_gpio_pin_toggle

```c
void csi_gpio_pin_toggle(csi_gpio_pin_t *pin)
```

- 功能描述:

  - 翻转引脚的电平状态。

- 参数:

  - `pin`: 实例句柄。
  
- 返回值: 无

### csi_gpio_pin_read

```c
csi_gpio_pin_state_t csi_gpio_pin_read(csi_gpio_pin_t *pin)
```

- 功能描述:

  - 读取引脚的电平状态。

- 参数:

  - `pin`: 实例句柄。
  
- 返回值:

  - 引脚状态。

### csi_gpio_pin_attach_callback

```c
csi_error_t csi_gpio_pin_attach_callback(csi_gpio_pin_t *pin, void *callback, void *arg)
```

- 功能描述:

  - 注册回调函数。

- 参数:

  - `pin`: 实例句柄。
  - `callback`: PIN实例的事件回调函数（一般在中断上下文执行）。
  - `arg`: 回调函数的参数。

- 参数: 


  - 错误码csi_error_t

## 电平翻转使用示例

```c
#include <stdio.h>

#include <soc.h>
#include <drv/gpio_pin.h>
#include <drv/tick.h>
#include <board_config.h>
#include <board_init.h>

#define GPIO_CHECK_RETURN(ret)           \
    do {                                 \
        if (ret != CSI_OK) {             \
            return -1;                   \
        }                                \
    } while(0);

static csi_gpio_pin_t pin;

int main(void)
{
    uint32_t tmp = 0;
    int ret;

    board_init();

    /* Initialize pin periphrial */
    ret = csi_gpio_pin_init(&pin,  PA0);
    GPIO_CHECK_RETURN(ret);

    /* Set output mode */
    ret = csi_gpio_pin_dir(&pin, GPIO_DIRECTION_OUTPUT);
    GPIO_CHECK_RETURN(ret);

    csi_gpio_pin_write(&pin, GPIO_PIN_LOW);
    while (1) {
        csi_gpio_pin_toggle(&pin);
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
#include <drv/gpio_pin.h>
#include <drv/tick.h>
#include <board_config.h>
#include <board_init.h>

#define GPIO_CHECK_RETURN(ret)           \
    do {                                 \
        if (ret != CSI_OK) {             \
            return -1;                   \
        }                                \
    } while(0);

volatile static bool intr_flag = false;
static csi_gpio_pin_t pin;

static void gpio_pin_interrupt_handler(csi_gpio_pin_t *pin, void *arg)
{
    intr_flag = true;
}

int main(void)
{
    int ret;

    board_init();

    /* Initialize pin periphrial */
    ret = csi_gpio_pin_init(&pin, PA0);
    GPIO_CHECK_RETURN(ret);

    /* Attach callback */
    ret = csi_gpio_pin_attach_callback(&pin, gpio_pin_interrupt_handler, NULL);
    GPIO_CHECK_RETURN(ret);

    /* Set pull-up mode */
    ret = csi_gpio_pin_mode(&pin, GPIO_MODE_PULLUP);
    GPIO_CHECK_RETURN(ret);

    /* Set input mode */
    ret = csi_gpio_pin_dir(&pin, GPIO_DIRECTION_INPUT);
    GPIO_CHECK_RETURN(ret);

    /* Set falling-edge triger mode */
    ret = csi_gpio_pin_irq_mode(&pin, GPIO_IRQ_MODE_FALLING_EDGE);
    GPIO_CHECK_RETURN(ret);

    /* Enable irq */
    ret = csi_gpio_pin_irq_enable(&pin, true);
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
