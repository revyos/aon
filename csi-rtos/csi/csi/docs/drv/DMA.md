# DMA

## 函数列表

## 简要说明

  * DMA(Direct Memory Access，直接内存存取)它允许不同速度的硬件装置之间沟通，而不需要依赖于CPU的大量中断负载。

## 接口描述
#### csi_dma_ch_t
| 成员              | 类型      | 说明    |
| :------------- | :--------- | :--------- |
| parent | void * | 存放使用DMA的外设句柄，如uart，iic等 |
| ctrl_id | int8_t | DMA控制器id |
| ch_id | int8_t | 通道句柄 |
| etb_ch_id | int16_t | ETB通道id |
| cb | void * | 通道回调函数 |
| arg | void * | 回调函数的参数指针 |
| next | struct csi_dma_ch * | 链表的下一个节点 |

* * * * *
#### csi_dma_ctrl_t
| 成员              | 类型      | 说明    |
| :------------- | :--------- | :--------- |
| dev | csi_dev_t | csi设备统一句柄 |
| ch_list | csi_dma_ch_t * | 通道链表首地址 |
| alloc_status | uint32_t | 通道占用状态 |
| ch_num | uint32_t | 控制器拥有的通道数量 |
| priv | void | 对接接口句柄地址 |

* * * * *
### csi_dma_cb_t
```c
typedef void (*csi_dma_cb_t)(csi_dma_ch_t *dma, csi_dma_event_t event, void *arg);
```

其中dma为通道句柄，event 为传给回调函数的事件类型，arg为用户自定义的回调函数对应的参数。 dma 回调事件枚举类型csi_dma_event_t定义如下：

| 事件类型                           | 事件说明                 |
| :--------------------------------- | --------------------- |
| DMA_EVENT_TRANSFER_DONE            | 数据传输完成事件         |
| DMA_EVENT_TRANSFER_HALF_DONE       | 数据传输完成一半事件      |
| DMA_EVENT_TRANSFER_ERROR           | 传输异常事件             |


### csi_dma_ctrl_init
```c
csi_error_t csi_dma_ctrl_init(csi_dma_ctrl_t *dma, int8_t ctrl_id)
```
* 功能描述:

  * 初始化DMA控制器

* 参数:

  * `dma`:  DMA控制器句柄
  * `ctrl_id`: 控制器id

* 返回值:

  * 错误码。

------
### csi_dma_ctrl_uninit
```c
void csi_dma_ctrl_uninit(csi_dma_ctrl_t *dma)
```
* 功能描述:

  * DMA控制器反初始化

* 参数:

  * `dma`: DMA控制器句柄

* 返回值:

  * 无。

------

### csi_dma_ch_alloc

```c
csi_error_t csi_dma_ch_alloc(csi_dma_ch_t *dma, int8_t ch_id, int8_t ctrl_id)
```

* 功能描述:

  * 申请dma通道号。

* 参数:

  * `dma`: DMA通道句柄
  * `ch_id`: DMA通道ID，当传入-1时，将由驱动自行选择
  * `ctrl_id`: DMA控制器ID，当传入-1时，将由驱动自行选择

* 返回值:

  * 错误码。

------

### csi_dma_ch_free

```c
void csi_dma_ch_free(csi_dma_ch_t *dma)
```

* 功能描述:

  * 释放DMA特定通道占用。

* 参数:

  * `dma`: DMA通道句柄。

------
#### csi_dma_addr_inc_t
| 类型               | 说明    |
| :---------------- | :------ |
| DMA_ADDR_INC      | 地址递增 |
| DMA_ADDR_DEC      | 地址递减 |
| DMA_ADDR_CONSTANT | 地址固定 |

------
#### csi_dma_data_width_t
| 类型                     | 说明      |
| :---------------------- | :-------- |
| DMA_DATA_WIDTH_8_BITS   | 8位数据宽度 |
| DMA_DATA_WIDTH_16_BITS  | 16位数据宽度 |
| DMA_DATA_WIDTH_32_BITS  | 32位数据宽度 |

------
#### csi_dma_endian_t（仅WJ使用）
| 类型                 | 说明      |
| :------------------ | :-------- |
| DMA_ENDIAN_LITTLE   | 小端数据传输 |
| DMA_ENDIAN_BIG      | 大端数据传输 |

------
#### csi_dma_trig_t（仅WJ使用）
| 类型                 | 说明      |
| :------------------ | :------- |
| DMA_SINGLE_TRIGGER   | 单次触发 |
| DMA_GROUP_TRIGGER    | 组触发   |
| DMA_BLOCK_TRIGGER    | 块触发   |

------
#### csi_dma_req_mode_t（仅WJ使用）
| 类型                 | 说明      |
| :------------------ | :------- |
| DMA_MODE_HARDWARE   | 硬件模式   |
| DMA_MODE_SOFTWARE   | 软件模式   |

------
#### csi_dma_single_dir_t（仅WJ使用）
| 类型                 | 说明               |
| :------------------ | :----------------- |
| DMA_SINGLE_DIR_DST   | 单次模式中方向为目的 |
| DMA_SINGLE_DIR_SRC   | 软件模式中方向为源   |

------
#### wj_dma_ch_config（仅WJ使用）
| 成员              | 类型      | 说明    |
| :------------- | :--------- | :--------- |
| group_len | uint8_t | 传输组长度 |
| half_int_en | uint8_t | 半完成中断使能 |
| secure_en | uint8_t | 安全模式使能 |
| src_endian | csi_dma_endian_t | 源大小端配置|
| dst_endian | csi_dma_endian_t | 目的大小端配置 |
| trig_mode | csi_dma_trig_t | 触发模式配置 |
| req_mode | csi_dma_req_mode_t | 请求模式配置 |
| single_dir | csi_dma_single_dir_t | 单次方向配置 |
| etb | csi_etb_config_t | ETB配置（定义请参阅ETB接口文档） |

------
#### csi_dma_trans_dir_t（仅DW使用）
| 类型           | 说明       |
| :------------ | :--------- |
| DMA_MEM2MEM   | 内存到内存   |
| DMA_MEM2PERH  | 内存到外设   |
| DMA_PERH2MEM  | 外设到内存   |
| DMA_PERH2PERH | 外设到外设   |

------
#### dw_dma_ch_config（仅DW使用）
| 成员            | 类型       | 说明        |
| :------------- | :--------- | :--------- |
| hs_if | uint8_t | 硬件握手号 |
| src_reload_en | uint8_t | 源地址自动加载使能 |
| dest_reload_en | uint8_t | 目的地址自动加载使能 |
| trans_dir | csi_dma_trans_dir_t | 传输数据方向配置 |

------

#### csi_dma_ch_config_t
| 成员              | 类型      | 说明    |
| :------------- | :--------- | :--------- |
| src_inc | csi_dma_addr_inc_t | DMA源地址增长方式 |
| dst_inc | csi_dma_addr_inc_t | DMA目的地址增长方式 |
| src_tw | csi_dma_data_width_t | DMA源数据宽度 |
| dst_tw | csi_dma_data_width_t | DMA目的数据宽度 |
| wj | wj_dma_ch_config | wj DMA配置结构体 |
| dw | dw_dma_ch_config | dw DMA配置结构体 |

------

### csi_dma_ch_config

```c
csi_error_t csi_dma_ch_config(csi_dma_ch_t *dma, csi_dma_ch_config_t *config, csi_dma_cb_t cb, void *arg);
```

* 功能描述:

  * 配置DMA通道工作方式，并配置回调函数。

* 参数:

  * `dma`: DMA通道句柄。
  * `config`: DMA通道配置结构体
  * `cb`: DMA用户回调函数
  * `arg`: DMA回调函数参数指针

* 返回值:

  * 错误码。

------

### csi_dma_ch_start

```c
void csi_dma_ch_start(csi_dma_ch_t *dma, void *srcaddr, void *dstaddr, uint32_t length)
```

* 功能描述:

  * 使能DMA通道，开始工作。

* 参数:

  * `dma`: DMA通道句柄。
  * `srcaddr`: 传输的源地址
  * `dstaddr`: 传输的目的地址
  * `length`: 数据的长度（字节为单位）
* 返回值:

  * 无。

------

### csi_dma_ch_stop

```c
void csi_dma_ch_stop(csi_dma_ch_t *dma)
```

* 功能描述:

  * 关闭DMA通道，结束工作。

* 参数:

  * `dma`: DMA通道句柄
