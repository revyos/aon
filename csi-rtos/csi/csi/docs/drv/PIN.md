# PIN
-----

## 说明

　　在 SOC 设计中，IOCTL（IO controller）作为 APB 的外设，提供了 IO cell 和外设交换数据的通道， 除此之外，还可以配置输入输出的方向，IO cell 复用功能（ALT）选择，驱动能力大小，上拉/下拉和其他 IO cell 的属性
功能
	通过配置管脚复用控制寄存器，来选择管脚作为其复用的一种功能，还可以调整其他属性如上拉（或 下拉），驱动能力增强等，IOCTL 模块具有以下特点 
 连接 IO cell 和外设 
 支持功能模式或者测试模式 
 支持 IO CELL 的 GPIO 功能或者其他复用（ALT）功能 
 可配置 IO cell 其他属性，如驱动能力，上下拉使能 
 支持 test mode 的特殊使用

## 接口列表
PIN的CSI接口说明如下所示：

|   函数                  |     说明                 |
|-------------------------|-------------------------|
|csi_pin_set_mux            |设置PIN的复用功能            |
|csi_pin_get_mux          |获取PIN的复用功能          |
|csi_pin_mode             |设置PIN的模式         |
|csi_pin_speed            |设置PIN的速度              |
|csi_pin_wakeup           |设置PIN的唤醒功能           |
|csi_pin_drive |设置PIN的驱动能力 |
|csi_pin_get_gpio_devidx        |通过pin name找GPIO端口号          |
|csi_pin_get_uart_devidx      |通过pin name查找UART设备号          |
|csi_pin_get_iic_devidx         |通过pin name查找IIC设备号           |
|csi_pin_get_spi_devidx           |通过pin name查找SPI设备号     |
|csi_pin_get_i2s_devidx          |通过pin name查找I2S设备号          |
| csi_pin_get_gpio_channel    | 通过pin name获取通道号 |
| csi_pin_get_pwm_channel     | 通过pin name获取PWM通道 |
| csi_pin_get_adc_channel     | 通过pin name获取ADC通道 |
| csi_pin_get_pinname_by_gpio | 通过PIN端口号与通道号获取pin name |

## 接口详细说明

### csi_pin_set_mux

```c
csi_error_t csi_pin_set_mux(pin_name_t pin_name, pin_func_t pin_func)
```

- 功能描述:

  - 设置PIN为复用功能。

- 参数:

  - pin_name: pin name，每一款芯片都有自己的pin name，与芯片手册pin name一一对应。枚举定义详见pin_name。
  - pin_func:  复用功能编号,每一款芯片都有自己的复用编号。枚举定义详见pin_func。

- 返回值:
  - 错误码csi_error_t。
  
- 使用示例：

  ```c
  /* 将PA2引脚设置为UART0发送复用功能 */
  csi_error_t  ret;
  ret = csi_pin_set_mux(PA2, PA2_UART0_TX);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

  

* * *
#### pin_name（例）

| 定义 | 值   |
| ---- | ---- |
| PA0  | 0    |
| PA1  | 1    |
| PA2  | 2    |
| PA3  | 3    |
| PA4  | 4    |
| PA5  | 5    |
| PB0  | 6    |
| PB1  | 7    |
| PB2  | 8    |
| PB3  | 9    |
| PA6  | 10   |
| PA7  | 11   |
| PA8  | 12   |
| PA9  | 13   |
| PA10 | 14   |
| PA11 | 15   |
| PA12 | 16   |
| PA13 | 17   |
| PA14 | 18   |
| PA15 | 19   |
| PA16 | 20   |
| PA17 | 21   |
| PA18 | 22   |
| PA19 | 23   |
| PA20 | 24   |
| PA21 | 25   |
| PA22 | 26   |
| PA23 | 27   |
| PA24 | 28   |
| PA25 | 29   |
| PA26 | 30   |
| PA27 | 31   |
| PC0  | 32   |
| PC1  | 33   |

#### pin_func（例）

| 定义             | 值     |
| :--------------- | ------ |
| PA0_ETB_TRIG0    | 0      |
| PA0_JTAG_TCK     | 2      |
| PA1_ETB_TRIG1    | 0      |
| PA1_JTAG_TMS     | 2      |
| PA2_SPI0_MISO    | 2      |
| PA2_UART0_SIROUT | 3      |
| ......           | ...... |
| PC1_PWM_CH11     | 2      |
| PC1_ADC_A15      | 3      |
| PIN_FUNC_GPIO    | 4      |

### csi_pin_get_mux

```c
pin_func_t csi_pin_get_mux(pin_name_t pin_name)
```

- 功能描述:

  - 获取PIN的复用功能。

- 参数:

  - pin_name: pin name，每一款芯片都有自己的pin name，与芯片手册pin name一一对应。

- 返回值:

  - 复用功能编号。

* 使用示例：

  ```c
  /* 获取PA2引脚复用功能*/
  pin_func_t  pin_func;
  pin_func = csi_pin_get_mux(PA2);
  return pin_func;
  ```

  

### csi_pin_mode

```c
csi_error_t csi_pin_mode(pin_name_t pin_name, csi_pin_mode_t mode)
```

- 功能描述:
  - 设置PIN的模式。
- 参数:
  - pin_name: pin name，每一款芯片都有自己的pin name，与芯片手册pin name一一对应。
  - mode：工作模式，枚举定义详见csi_pin_mode_t。
- 返回值:
  - 错误码csi_error_t。

#### csi_pin_mode_t

| 类型                 | 说明     |
| :------------------- | -------- |
| GPIO_MODE_PULLNONE   | 悬空输入 |
| GPIO_MODE_PULLUP     | 上拉输入 |
| GPIO_MODE_PULLDOWN   | 下拉输入 |
| GPIO_MODE_OPEN_DRAIN | 开漏输出 |
| GPIO_MODE_PUSH_PULL  | 推挽输出 |

### csi_pin_speed

```c
csi_error_t csi_pin_speed(pin_name_t pin_name, csi_pin_speed_t speed)
```

- 功能描述:

  -  设置PIN的速度。

- 参数:

  - pin_name: pin name，每一款芯片都有自己的pin name，与芯片手册pin name一一对应。
  - speed：响应速度，枚举定义详见csi_pin_speed_t。

- 返回值:

  - 错误码csi_error_t。

#### csi_pin_speed_t（0级为最低速，依次类推）

| 类型          | 说明    |
| :------------ | ------- |
| PIN_SPEED_LV0 | 0级速度 |
| PIN_SPEED_LV1 | 1级速度 |
| PIN_SPEED_LV2 | 2级速度 |
| PIN_SPEED_LV3 | 3级速度 |

### csi_pin_wakeup

```c
csi_error_t csi_pin_wakeup(pin_name_t pin_name, bool enable)
```

- 功能描述:

  -  设置PIN的唤醒功能。

- 参数:

  - pin_name: pin name，每一款芯片都有自己的pin name，与芯片手册pin name一一对应。
  - enable: 使能标记位。

- 返回值:

  - 错误码csi_error_t。

### csi_pin_drive

```c
csi_error_t csi_pin_drive(pin_name_t pin_name, csi_pin_drive_t drive)
```

- 功能描述:

  -  设置PIN的驱动能力。

- 参数:

  - pin_name: pin name，每一款芯片都有自己的pin name，与芯片手册pin name一一对应。
  - drive：驱动能力，枚举定义详见csi_pin_drive_t。

- 返回值:

  - 错误码csi_error_t。

#### csi_pin_drive_t（0级驱动能力最弱，依次类推）

| 类型          | 说明        |
| :------------ | ----------- |
| PIN_DRIVE_LV0 | 0级驱动能力 |
| PIN_DRIVE_LV1 | 1级驱动能力 |
| PIN_DRIVE_LV2 | 2级驱动能力 |
| PIN_DRIVE_LV3 | 3级驱动能力 |

### csi_pin_get_gpio_devidx  

```c
 uint32_t csi_pin_get_gpio_devidx(pin_name_t pin_name)
```

- 功能描述:
  -  通过pin name查找GPIO端口号。
- 参数:
  - pin_name: pin name，每一款芯片都有自己的pin name，与芯片手册pin name一一对应。
- 返回值:
  - GPIO端口号。（PORTA为0，PORTB为1，依次类推，失败返回0xFFFFFFFFU）。

### csi_pin_get_uart_devidx

```c
 uint32_t csi_pin_get_uart_devidx(pin_name_t pin_name)
```

- 功能描述:
  -   通过pin name查找UART设备号 。
- 参数:
  - pin_name: pin name，每一款芯片都有自己的pin name，与芯片手册pin name一一对应。
- 返回值:
  - UART设备号。(UART0为0，UART1为1，依次类推，失败返回0xFFFFFFFFU）。

### csi_pin_get_iic_devidx

```c
 uint32_t csi_pin_get_iic_devidx(pin_name_t pin_name)
```

- 功能描述:
  -   通过pin name查找IIC设备号。
- 参数:
  - pin_name: pin name，每一款芯片都有自己的pin name，与芯片手册pin name一一对应。
- 返回值:
  - IIC设备号。(IIC0为0，IIC1为1，依次类推，失败返回0xFFFFFFFFU）。

### csi_pin_get_spi_devidx

```c
 uint32_t csi_pin_get_spi_devidx(pin_name_t pin_name)
```

- 功能描述:
  -   通过pin name查找SPI设备号。
- 参数:
  - pin_name: pin name，每一款芯片都有自己的pin name，与芯片手册pin name一一对应。
- 返回值:
  - SPI设备号。(SPI0为0，SPI1为1，依次类推，失败返回0xFFFFFFFFU）。

### csi_pin_get_i2s_devidx

```c
 uint32_t csi_pin_get_i2s_devidx(pin_name_t pin_name)
```

- 功能描述:
  -   通过pin name查找I2S设备号。
- 参数:
  - pin_name: pin name，每一款芯片都有自己的pin name，与芯片手册pin name一一对应。
- 返回值:
  - I2S设备号。(I2S0为0，I2S1为1，依次类推，失败返回0xFFFFFFFFU）。

### csi_pin_get_gpio_channel  

```c
 uint32_t csi_pin_get_gpio_channel  (pin_name_t pin_name)
```

- 功能描述:
  -  通过pin name获取GPIO通道号。
- 参数:
  - pin_name: pin name，每一款芯片都有自己的pin name，与芯片手册pin name一一对应。
- 返回值:
  - GPIO通道号。(PA0为0，PA1为1，依次类推，失败返回0xFFFFFFFFU）。

### csi_pin_get_pwm_channel    

```c
 uint32_t csi_pin_get_pwm_channel  (pin_name_t pin_name)
```

- 功能描述:
  -  通过pin name获取PWM通道 。
- 参数:
  - pin_name: pin name，每一款芯片都有自己的pin name，与芯片手册pin name一一对应。
- 返回值:
  - PWM通道 。(PWM_CH0为0，PWM_CH1为1，依次类推，失败返回0xFFFFFFFFU）。

### csi_pin_get_adc_channel  

```c
 uint32_t csi_pin_get_adc_channel  (pin_name_t pin_name)
```

- 功能描述:
  -  通过pin name获取ADC通道 。
- 参数:
  - pin_name: pin name，每一款芯片都有自己的pin name，与芯片手册pin name一一对应。
- 返回值:
  - ADC通道 。(ADC_A0为0，ADC_A1为1，依次类推，失败返回0xFFFFFFFFU）。

### csi_pin_get_pinname_by_gpio

```c
pin_name_t csi_pin_get_pinname_by_gpio(uint8_t gpio_idx, uint8_t channel)
```

- 功能描述:
  -  通过PIN端口号与通道号获取pin name。
- 参数:
  - gpio_idx:GPIO端口号。
  - channel:GPIO通道号。
- 返回值: 
  - pin name，每一款芯片都有自己的pin name，与芯片手册pin name一一对应。

### csi_pin_uart_t枚举说明

- 功能描述:
  -  用于查找UART设备具体的某一个引脚。

| 类型         | 说明         |
| :----------- | ------------ |
| PIN_UART_TX  | UART发送引脚 |
| PIN_UART_RX  | UART接收引脚 |
| PIN_UART_CTS | UART CTS引脚 |
| PIN_UART_RTS | UART RTS引脚 |

### csi_pin_iic_t枚举说明

- 功能描述:
  -  用于查找iic设备具体的某一个引脚。

| 类型        | 说明        |
| :---------- | ----------- |
| PIN_IIC_SCL | IIC时钟引脚 |
| PIN_IIC_SDA | IIC数据引脚 |

### csi_pin_spi_t枚举说明

- 功能描述:
  -  用于查找spi设备具体的某一个引脚。

| 类型         | 说明         |
| :----------- | ------------ |
| PIN_SPI_MISO | SPI MISO引脚 |
| PIN_SPI_MOSI | SPI MOSI引脚 |
| PIN_SPI_SCK  | SPI时钟引脚  |
| PIN_SPI_CS   | SPI片选引脚  |

### csi_pin_i2s_t枚举说明

- 功能描述:
  -  用于查找I2S设备具体的某一个引脚。

| 类型          | 说明          |
| :------------ | ------------- |
| PIN_I2S_MCLK  | I2S MCLK引脚  |
| PIN_I2S_SCLK  | I2S SCLK引脚  |
| PIN_I2S_WSCLK | I2S WSCLK引脚 |
| PIN_I2S_SDA   | I2S数据引脚   |

## 示例

### 将PA1引脚设置为GPIO功能、上拉输入、低速响应、低驱动能力。

```c

int main(void){
	csi_error_t  ret;
	ret = csi_pin_set_mux(PA1, PIN_FUNC_GPIO);
	if (ret != CSI_OK) {
    	return -1;
	}
	ret = csi_pin_set_mode(PA1, GPIO_MODE_PULLUP);
	if (ret != CSI_OK) {
    	return -1;
	}
	ret = csi_pin_speed(PA1, PIN_SPEED_LV0);
	if (ret != CSI_OK) {
    	return -1;
	}
	ret = csi_pin_drive(PA1, PIN_DRIVE_LV0);
	if (ret != CSI_OK) {
    	return -1;
	}
    return 1;
}
```

