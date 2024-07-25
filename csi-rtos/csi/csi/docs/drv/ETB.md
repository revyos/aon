# ETB

## 简要说明

ETB的功能是通过发送信息的方式，是一个IP触发另一个IP，并且释放CPU。
ETB会管理所有触发事件。

## 接口描述

### csi_etb_init

```c
csi_error_t csi_etb_init(void)
```

* 功能描述:

  * 初始化ETB设备，并使能ETB 。


* 返回值:

  * 错误码。

------

### csi_etb_uninit

```c
void csi_etb_init(void)
```

* 功能描述:

  * 去初始化ETB设备，并关闭ETB 。

------

#### csi_etb_ch_type_t
| 类型                    | 定义              |
| :---------------------- | :---------------- |
| ETB_CH_ONE_TRIGGER_ONE | 单IP触发单IP的通道类型 |
| ETB_CH_ONE_TRIGGER_MORE | 单IP触发多IP的通道类型 |
| ETB_CH_MORE_TRIGGER_ONE | 多IP触发单IP的通道类型 |

### csi_etb_ch_alloc

```c
int32_t csi_etb_ch_alloc(csi_etb_ch_type_t ch_type)
```

* 功能描述:

  * 根据传入的通道类型，申请一个空闲通道，并返回通道号 。

* 参数:

  * `ch_type`: 通道工作类型。


* 返回值:

  * 成功返回通道号，失败-1。

------

### csi_etb_ch_free

```c
void csi_etb_ch_free(int32_t ch_id)
```

* 功能描述:

  * 通过传入的通道号，释放对应的通道。

* 参数:

  * `ch_id`: 通道号。

------
#### csi_etb_trig_mode_t
| 类型                    | 说明              |
| :---------------------- | :---------------- |
| ETB_HARDWARE_TRIG | 硬件触发 |
| ETB_SOFTWARE_TRIG | 软件触发 |

#### csi_etb_config_t
| 成员           | 类型         |  说明  |
| :------------- | :---------- |:----- |
| src_ip | uint8_t | 源IP设备号 |
| dst_ip | uint8_t | 目的IP设备号|
| trig_mode | csi_etb_trig_mode_t |触发模式 |
| ch_type | csi_etb_ch_type_t |通道类型 |

### csi_etb_ch_config

```c
int32_t csi_etb_ch_config(int32_t ch_id, csi_etb_config_t *config)
```

* 功能描述:

  * 配置通道工作模式。

* 参数:

  * `ch_id`: 通道号。

  * `config`: 通道配置。

* 返回值:

  * 错误码。

------

### csi_etb_ch_start

```c
void csi_etb_ch_start(int32_t ch_id)
```

* 功能描述:

  * 使能通道功能。

* 参数:

  * `ch_id`: 通道号。

------

### csi_etb_ch_stop

```c
void csi_etb_ch_stop(int32_t ch_id)
```

* 功能描述:

  * 关闭通道使能。

* 参数:

  * `ch_id`: 通道号。

------
