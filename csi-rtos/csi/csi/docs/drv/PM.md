# PM
-----

## 设备说明

　　PM（power manager）负责统一管理整个SOC芯片的低功耗行为。控制各个驱动设备在进入/退出低功耗模式时执行对应的操作。

## 接口列表

PIN的CSI接口说明如下所示：

|   函数                  |     说明                 |
|-------------------------|-------------------------|
|csi_pm_init              |power manager初始化       |
|csi_pm_uninit            |power manager去初始化     |
|csi_pm_set_reten_mem     |设置retention内存空间      |
|csi_pm_config_wakeup_source |配置低功耗模式的唤醒源    |
|csi_pm_enter_sleep       |进入低功耗模式  |
|csi_pm_dev_register      |注册设备到低功耗管理模块(驱动设备调用)   |
|csi_pm_dev_unregister    |从低功耗管理模块取消初始化(驱动设备调用) |
|csi_pm_dev_save_regs     |保存寄存器到内存中(驱动设备调用)   |
|csi_pm_dev_restore_regs  |从内存中恢复到寄存器中(驱动设备调用)  |
|csi_pm_dev_notify        |执行驱动设备回调函数          |
|soc_pm_enter_sleep       |进入低功耗模式(实现在soc的pmu驱动中)        |
|soc_pm_config_wakeup_source | 配置低功耗模式的唤醒源(实现在soc的pmu驱动中) |

## 接口详细说明

### csi_pm_init

csi_error_t csi_pm_init(void)

- 功能描述:

  - 功耗管理模块初始化

- 参数:

 - 无

- 返回值:

  - CSI_OK: 设置成功。 
  - CSI_ERROR: 设置失败。
  - CSI_UNSUPPORTED:参数错误。
  
### csi_pm_uninit

void csi_pm_uninit(void)

- 功能描述:

  - 功耗管理模块去初始化

- 参数:

  - 无

- 返回值:

  - 无
  

### csi_pm_set_reten_mem

csi_error_t csi_pm_set_reten_mem(uint32_t *mem, uint32_t num)

- 功能描述:
  - 设置保留内存的区域，用于保存在掉电模式下相关IP寄存器的内容。
- 参数:
  - mem：以字为单位
  - num：指定内存大小，以字为单位
- 返回值:
  - CSI_OK: 设置成功。 
  - CSI_ERROR: 设置失败。
  - CSI_UNSUPPORTED:参数错误。

### csi_pm_config_wakeup_source

csi_error_t csi_pm_config_wakeup_source(uint32_t wakeup_num, bool enable)

- 功能描述:

  -  配置唤醒源

- 参数:

  - wakeup_num: 唤醒号
  - enable: 是否使能唤醒功能

- 返回值:

  - CSI_OK: 配置成功。
  - CSI_ERROR: 配置失败。
  - CSI_UNSUPPORTED: 模式不支持。

### csi_pm_enter_sleep

csi_error_t csi_pm_enter_sleep(csi_pm_mode_t mode)

- 功能描述:

  -  进入低功耗模式

- 参数:

  - mode: 低功耗模式

- 返回值:

  - CSI_OK: 配置成功。
  - CSI_ERROR: 配置失败。
  - CSI_UNSUPPORTED: 模式不支持。

#### csi_pm_mode_t

| 类型          | 说明        |
| :------------- | ----------- |
| PM_MODE_RUN     | 运行模式   |
| PM_MODE_SLEEP_1 | 一级浅睡眠模式 |
| PM_MODE_SLEEP_2 | 二级浅睡眠模式 |
| PM_MODE_DEEP_SLEEP_1 | 一级深度睡眠模式 |
| PM_MODE_DEEP_SLEEP_2 | 二级深度睡眠模式 |

### csi_pm_dev_register

 csi_error_t csi_pm_dev_register(csi_dev_t *dev, void *pm_action, uint32_t mem_size, uint8_t priority)

- 功能描述:
  -  驱动设备通过调用该接口将驱动设备在掉电模式下所需空间、执行的回调函数及其优先级注册到低功耗管理模块中
- 参数:
  - dev: 目标设备的句柄
  - pm_action: 进入低功耗模式前/退出低功耗模式后，执行的设备回调函数
  - mem_size: 目标设备在掉电模式下所需的保存空间
  - prority: 执行设备回调函数的优先级（值越小，进入低功耗模式前越晚调用/退出低功耗模式后越先执行）
- 返回值:

  - CSI_OK: 配置成功。
  - CSI_ERROR: 配置失败。
  - CSI_UNSUPPORTED: 模式不支持。

#### csi_dev_t

| 成员          | 类型        | 说明    |
| :------------- | :----------- |:-------|
| reg_base     | uint32_t | 寄存器基址   |
| irq_num      | uint8_t  | 中断号|
| idx          | uint8_t  | 设备号 |
| dev_tag      | uint16_t | 设备的tag |
| irq_handler  | void (*)(void *)|中断回调函数 |
|pm_dev        | csi_pm_dev_t | 低功耗管理模块的设备 |

#### csi_pm_dev_t

| 成员          | 类型        | 说明    |
| :------------- | :----------- |:-------|
| next          | slist_t | 寄存器基址   |
| pm_action     | csi_error_t (*)(csi_dev_t *dev, csi_pm_dev_action_t action )  | 低功耗设备的回调函数 |
| reten_mem     | uint32_t * | 驱动设备保存的区域地址 |
| size      | uint32_t | 驱动设备需要保存的大小 |

### csi_pm_dev_unregister

 uint32_t csi_pm_dev_unregister(csi_dev_t *dev)

- 功能描述:
  -   从低功耗管理模块中取消注册目标设备
- 参数:
  - dev: 目标设备的句柄
- 返回值:
  - 无

### csi_pm_dev_save_regs

 void csi_pm_dev_save_regs(uint32_t *mem, uint32_t *addr, uint32_t num)

- 功能描述:
  -   保存指定个数的寄存器内容到内存
- 参数:
  - mem: 内存地址
  - addr: 寄存器地址
  - num: 寄存器个数
- 返回值:
  - 无

### csi_pm_dev_restore_regs

 void csi_pm_dev_restore_regs(uint32_t *mem, uint32_t *addr, uint32_t num)

- 功能描述:
  -   从内存恢复指定个数的寄存器
- 参数:
  - mem: 内存地址
  - addr: 寄存器地址
  - num: 寄存器个数
- 返回值:
  - 无

### csi_pm_dev_notify

 csi_error_t csi_pm_dev_notify(csi_pm_dev_action_t action)

- 功能描述:
  -   通知注册到低功耗管理模块的设备，执行进入/退出低功耗模式所对应的回调函数
- 参数:
  - action: 通知回调函数的命令
- 返回值:

  - CSI_OK: 配置成功。
  - CSI_ERROR: 配置失败。
  - CSI_UNSUPPORTED: 模式不支持。

### soc_pm_enter_sleep

 csi_error_t soc_pm_enter_sleep(csi_pm_mode_t mode)

- 功能描述:
  -  进入低功耗模式，对接csi_pm_enter_sleep接口，实现在各个soc的pmu中
- 参数:
  - mode: 低功耗模式
- 返回值:

  - CSI_OK: 配置成功。
  - CSI_ERROR: 配置失败。
  - CSI_UNSUPPORTED: 模式不支持。

### soc_pm_config_wakeup_source

 csi_error_t csi_pm_config_wakeup_source(uint32_t wakeup_num, bool enable)

- 功能描述:
  -  配置唤醒源
- 参数:
  - wakeup_num: 唤醒号
  - enable: 是否使能唤醒功能
- 返回值:

  - CSI_OK: 配置成功。
  - CSI_ERROR: 配置失败。
  - CSI_UNSUPPORTED: 模式不支持。
