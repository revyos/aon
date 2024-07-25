# Porting

## 说明

为了适配不同的硬件平台，增加porting接口。

---

## 接口列表

porting的接口如下所示：

| 函数                          | 说明                  |
| ----------------------------- | --------------------- |
| soc_get_apb_freq              | 获取APB总线的频率     |
| soc_get_ahb_freq              | 获取AHB总线的频率     |
| soc_get_cpu_freq              | 获取CPU的频率         |
| soc_get_uart_freq             | 获取UART的频率        |
| soc_get_spi_freq              | 获取SPI的频率         |
| soc_get_iic_freq              | 获取IIC的频率         |
| soc_get_i2s_freq              | 获取I2S的频率         |
| soc_get_pwm_freq              | 获取PWM的频率         |
| soc_get_adc_freq              | 获取ADC的频率         |
| soc_get_qspi_freq             | 获取QSPI的频率        |
| soc_get_usi_freq              | 获取USI的频率         |
| soc_get_timer_freq            | 获取TIMER的频率       |
| soc_get_rtc_freq              | 获取RTC的频率         |
| soc_get_wdt_freq              | 获取WDT的频率         |
| soc_get_sdio_freq             | 获取SDIO的频率        |
| soc_get_emmc_freq             | 获取EMMC的频率        |
| soc_get_usb_freq              | 获取USB的频率         |
| soc_get_coretime_freq         | 获取CORETIME的频率    |
| soc_get_cur_cpu_freq          | 获取当前CPU的频率     |
| soc_get_ref_clk_freq          | 获取REF_CLK的频率     |
| soc_set_sys_freq_freq         | 设置系统频率          |
| soc_clk_enable_freq           | 使能IP的时钟          |
| soc_clk_disable_freq          | 失效IP的时钟          |
| soc_get_cpu_id                | 获取CPU的ID号         |
| soc_dcache_clean_invalid_range| 按地址清楚并无效DCACHE|
| soc_dcache_clean_invalid_all  | 清除并无效所以DCACHE  |
| soc_dcache_invalid_range      | 按地址无效DCACHE      |
| soc_dcache_clean              | 清除DCACHE            |
| soc_icache_invalid            | 无效ICACHE            |
| soc_dma_address_remap         | DMA地址重映射         |
| soc_pm_enter_sleep            | 进入睡眠模式          |
| soc_pm_config_wakeup_source   | 配置唤醒源            |

---

## 接口详细说明

### soc_get_apb_freq

```c
uint32_t soc_get_apb_freq(uint32_t idx)
```

* 功能描述:

  * 通过设备ID获取APB总线频率。

* 参数:
  
  * `idx`: 设备ID。

* 返回值:

  * 频率。
  
------

### soc_get_ahb_freq

```c
uint32_t soc_get_ahb_freq(uint32_t idx)
```

* 功能描述:

  * 通过设备ID获取AHB总线频率。

* 参数:
  
  * `idx`: 设备ID。

* 返回值:

  * 频率。
  
------

### soc_get_cpu_freq

```c
uint32_t soc_get_cpu_freq(uint32_t idx)
```

* 功能描述:

  * 通过设备ID获取CPU频率。

* 参数:
  
  * `idx`: 设备ID。

* 返回值:

  * 频率。
  
------

### soc_get_uart_freq

```c
uint32_t soc_get_uart_freq(uint32_t idx)
```

* 功能描述:

  * 通过设备ID获取UART频率。

* 参数:
  
  * `idx`: 设备ID。

* 返回值:

  * 频率。
  
------

### soc_get_spi_freq

```c
uint32_t soc_get_spi_freq(uint32_t idx)
```

* 功能描述:

  * 通过设备ID获取SPI频率。

* 参数:
  
  * `idx`: 设备ID。

* 返回值:

  * 频率。
  
------

### soc_get_iic_freq

```c
uint32_t soc_get_iic_freq(uint32_t idx)
```

* 功能描述:

  * 通过设备ID获取IIC频率。

* 参数:
  
  * `idx`: 设备ID。

* 返回值:

  * 频率。
  
------

### soc_get_i2s_freq

```c
uint32_t soc_get_i2s_freq(uint32_t idx)
```

* 功能描述:

  * 通过设备ID获取I2S频率。

* 参数:
  
  * `idx`: 设备ID。

* 返回值:

  * 频率。
  
------

### soc_get_pwm_freq

```c
uint32_t soc_get_pwm_freq(uint32_t idx)
```

* 功能描述:

  * 通过设备ID获取PWM频率。

* 参数:
  
  * `idx`: 设备ID。

* 返回值:

  * 频率。
  
------

### soc_get_adc_freq

```c
uint32_t soc_get_adc_freq(uint32_t idx)
```

* 功能描述:

  * 通过设备ID获取ADC频率。

* 参数:
  
  * `idx`: 设备ID。

* 返回值:

  * 频率。
  
------

### soc_get_qspi_freq

```c
uint32_t soc_get_qspi_freq(uint32_t idx)
```

* 功能描述:

  * 通过设备ID获取QSPI频率。

* 参数:
  
  * `idx`: 设备ID。

* 返回值:

  * 频率。
  
------

### soc_get_usi_freq

```c
uint32_t soc_get_usi_freq(uint32_t idx)
```

* 功能描述:

  * 通过设备ID获取USI频率。

* 参数:
  
  * `idx`: 设备ID。

* 返回值:

  * 频率。
  
------

### soc_get_timer_freq

```c
uint32_t soc_get_timer_freq(uint32_t idx)
```

* 功能描述:

  * 通过设备ID获取TIMER频率。

* 参数:
  
  * `idx`: 设备ID。

* 返回值:

  * 频率。
  
------

### soc_get_rtc_freq

```c
uint32_t soc_get_rtc_freq(uint32_t idx)
```

* 功能描述:

  * 通过设备ID获取RTC频率。

* 参数:
  
  * `idx`: 设备ID。

* 返回值:

  * 频率。
  
------

### soc_get_wdt_freq

```c
uint32_t soc_get_wdt_freq(uint32_t idx)
```

* 功能描述:

  * 通过设备ID获取WDT频率。

* 参数:
  
  * `idx`: 设备ID。

* 返回值:

  * 频率。
  
------

### soc_get_sdio_freq

```c
uint32_t soc_get_sdio_freq(uint32_t idx)
```

* 功能描述:

  * 通过设备ID获取SDIO频率。

* 参数:
  
  * `idx`: 设备ID。

* 返回值:

  * 频率。
  
------

### soc_get_emmc_freq

```c
uint32_t soc_get_emmc_freq(uint32_t idx)
```

* 功能描述:

  * 通过设备ID获取EMMC频率。

* 参数:
  
  * `idx`: 设备ID。

* 返回值:

  * 频率。
  
------

### soc_get_usb_freq

```c
uint32_t soc_get_usb_freq(uint32_t idx)
```

* 功能描述:

  * 通过设备ID获取USB频率。

* 参数:
  
  * `idx`: 设备ID。

* 返回值:

  * 频率。
  
------

### soc_get_coretime_freq

```c
uint32_t soc_get_coretime_freq(void)
```

* 功能描述:

  * 通过设备ID获取coretime频率。

* 参数:

* 返回值:

  * 频率。
  
------

### soc_get_cur_cpu_freq

```c
uint32_t soc_get_cur_cpu_freq(void)
```

* 功能描述:

  * 通过设备ID获取当前CPU频率。

* 参数:
  
* 返回值:

  * 频率。
  
------

### soc_get_ref_clk_freq

```c
uint32_t soc_get_ref_clk_freq(uint32_t idx)
```

* 功能描述:

  * 通过设备ID获取REF_CLK总线频率。

* 参数:
  
  * `idx`: 设备ID。

* 返回值:

  * 频率。
  
------

### soc_set_sys_freq

```c
void soc_set_sys_freq(uint32_t freq)
```

* 功能描述:

  * 设置系统频率。

* 参数:

  * `freq`: 目标频率,参见sys_freq_t

* 返回值:

  * 无。

------

### soc_clk_enable

```c
void soc_clk_enable(int32_t module)
```

* 功能描述:

  * 使能时钟。

* 参数:

  * `module`: 模块名,参见clk_manager_type_t

* 返回值:

  * 无。

------

### soc_clk_disable

```c
void soc_clk_disable(int32_t module)
```

* 功能描述:

  * 使能时钟。

* 参数:

  * `module`: 模块名,参见clk_manager_type_t

* 返回值:

  * 无。

------

### soc_get_cpu_id

```c
uint32_t soc_get_cpu_id(void)
```

* 功能描述:

  * 获取CPU号。

* 参数:

* 返回值:

  * CPU idx号。
  
------

### soc_dcache_clean_invalid_range

```c
void soc_dcache_clean_invalid_range(unsigned long addr, uint32_t size)
```

* 功能描述:

  * 按地址清除并无效DCACHE

* 参数:

  * `addr`: 首地址。

  * `size`: 大小。

* 返回值:

  * 无。

------

### soc_dcache_clean_invalid_all

```c
void soc_dcache_clean_invalid_all(void)
```

* 功能描述:

  * 清除并无效所有DCACHE

* 参数:

  * 无。

* 返回值:

  * 无。

------

### soc_dcache_invalid_range

```c
void soc_dcache_invalid_range(unsigned long addr, uint32_t size)
```

* 功能描述:

  * 按地址无效DCACHE

* 参数:

  * `addr`: 首地址。

  * `size`: 大小。

* 返回值:

  * 无。

------

### soc_dcache_clean

```c
void soc_dcache_clean(void)
```

* 功能描述:

  * 清除DCACHE

* 参数:

  * 无。

* 返回值:

  * 无。

------

### soc_icache_invalid

```c
void soc_icache_invalid(void)
```

* 功能描述:

  * 无效ICACHE

* 参数:

  * 无。

* 返回值:

  * 无。

------

### soc_dma_address_remap

```c
void soc_dma_address_remap(unsigned long addr)
```

* 功能描述:

  * DMA地址重映射

* 参数:

  * `addr`: 地址。

* 返回值:

  * 无。

------

### soc_pm_enter_sleep

```c
csi_error_t soc_pm_enter_sleep(csi_pm_mode_t mode)
```

* 功能描述:

  * 进入低功耗模式。

* 参数:

  * `mode`: 低功耗模式。

* 返回值:

  * 错误码csi_error_t。

---

#### csi_pm_mode_t

| 类型                 | 说明              |
| ---------------      | ----------------- |
| PM_MODE_RUN          | 运行模式          |
| PM_MODE_SLEEP_1      | 睡眠等级1模式     |
| PM_MODE_SLEEP_2      | 睡眠等级2模式     |
| PM_MODE_DEEP_SLEEP_1 | 深度睡眠等级1模式 |
| PM_MODE_DEEP_SLEEP_2 | 深度睡眠等级2模式 |

------

### soc_pm_config_wakeup_source

```c
csi_error_t soc_pm_config_wakeup_source(uint32_t wakeup_num, bool enable)
```

* 功能描述:

  * 配置唤醒源。

* 参数:

  * `wakeup_num`: 唤醒号。

  * `enable`: 唤醒源使能标记。

* 返回值:

  * 错误码csi_error_t。

------
