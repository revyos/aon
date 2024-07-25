

# I2S
-----

## 说明

I2S(Inter—IC Sound)总线, 又称集成电路内置音频总线，是飞利浦公司为数字音频设备之间的音频数据传输而制定的一种总线标准，该总线专门用于音频设备之间的数据传输。

## 接口列表

I2S的CSI接口如下所示：

| 函数                    | 说明                         |
| ----------------------- | ---------------------------- |
| csi_i2s_init            | I2S设备初始化                |
| csi_i2s_uninit          | I2S设备反初始化              |
| csi_i2s_enbale          | I2S设备使能/禁止接口         |
| csi_i2s_format          | I2S设备参数配置接口          |
| csi_i2s_tx_select_sound_channel  | I2S发送通道配置单/双声道     |
| csi_i2s_rx_select_sound_channel  | I2S接收通道配置单/双声道     |
| csi_i2s_tx_link_dma     | I2S发送通道配置DMA           |
| csi_i2s_rx_link_dma     | I2S接收通道配置DMA           |
| csi_i2s_tx_set_buffer   | I2S发送通道配置缓冲区地址    |
| csi_i2s_rx_set_buffer   | I2S接收通道配置缓冲区地址    |
| csi_i2s_tx_set_period   | 设置完成多少数据发送上报周期 |
| csi_i2s_rx_set_period   | 设置完成多少数据接收上报周期 |
| csi_i2s_tx_buffer_avail    | 返回I2S发送缓冲区内数据量    |
| csi_i2s_tx_buffer_reset    | 重置I2S发送缓冲区            |
| csi_i2s_rx_buffer_avail    | 返回I2S接收缓冲区内数据量    |
| csi_i2s_rx_buffer_reset    | 重置I2S接收缓冲区            |
| csi_i2s_send            | I2S同步模式发送数据          |
| csi_i2s_receive         | I2S同步模式接收数据          |
| csi_i2s_send_async      | I2S异步模式发送数据          |
| csi_i2s_receive_async   | I2S异步模式接收数据          |
| csi_i2s_send_start      | I2S开始发送音频流            |
| csi_i2s_send_pause      | I2S暂停发送音频流            |
| csi_i2s_send_resume     | I2S恢复发送音频流            |
| csi_i2s_send_stop       | I2S停止发送音频流            |
| csi_i2s_receive_start   | I2S开始接收音频流            |
| csi_i2s_receive_stop    | I2S停止接收音频流            |
| csi_i2s_attach_callback | I2S注册回调函数              |
| csi_i2s_detach_callback | I2S注销回调函数              |
| csi_i2s_get_state       | 获取I2S设备的当前读写状态      |




## 接口详细说明

### csi_i2s_init

```c
csi_error_t csi_i2s_init(csi_i2s_t *i2s, uint32_t idx)
```

* 功能描述:

  * 通过设备ID初始化对应的I2S实例。

* 参数:

  * `i2s`: 设备句柄（需要用户申请句柄空间）。
  * `idx`: 设备ID。
  
* 返回值:

  * 错误码csi_error_t。

#### csi_i2s_t

| 成员              | 类型      | 说明    |
| :------------- | :--------- | :--------- |
| dev | csi_dev_t | csi设备统一句柄     |
| callback | void (*callback)(csi_i2s_t *i2s, csi_i2s_event_t event, void *arg) | 用户回调函数 |
| arg | void * | 回调函数对应传参 |
| tx_buf | ringbuffer_t * | 发送音频流用的ringbuffer缓冲区 |
| rx_buf | ringbuffer_t * | 接收音频流用的ringbuffer缓冲区 |
| tx_dma | csi_dma_ch_t * | 用于发送的DMA通道句柄 |
| rx_dma | csi_dma_ch_t * | 用于接收的DMA通道句柄 |
| tx_period | uint32_t     | 设置完成多少数据发送上报周期 |
| rx_period | uint32_t     | 设置完成多少数据接收上报周期 |
| state | csi_state_t | I2S设备的当前读写状态 |
| priv | void * | 对接接口句柄地址 |

### csi_i2s_uninit

```c
void csi_i2s_uninit(csi_i2s_t *i2s)
```

* 功能描述:

  * i2s实例反初始化。
  * 该接口会清理并释放相关的软硬件资源。
* 参数:

  * `i2s`: 实例句柄。
* 返回值：
  * 无。

### csi_i2s_enable

```c
void csi_i2s_enable(csi_i2s_t *i2s, bool en)
```

* 功能描述:

  * 设置I2S设备的使能或禁止。

* 参数:

  * `i2s`: 实例句柄。
  * `en`: True代表I2S设备打开，Flase代表I2S设备关闭。

* 返回值：

  * 无。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_i2s_t g_i2s;
  csi_error_t ret;
  ret = csi_i2s_enable(&g_i2s, true);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_i2s_format

```c
csi_error_t csi_i2s_format(csi_i2s_t *i2s, csi_i2s_format_t *format)
```

* 功能描述:

  * 配置I2S的参数。
  
* 参数：

  * `i2s`: 实例句柄。
  * `format`:配置I2S的结构体指针。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_i2s_t g_i2s;
  csi_error_t ret;
  csi_i2s_format_t i2s_format;
  i2s_format.mode = I2S_MODE_MASTER;              ///< 设置I2S 为主机
  i2s_format.protocol = I2S_PROTOCOL_I2S;			///< 设置I2S协议为I2S
  i2s_format.width = I2S_SAMPLE_WIDTH_16BIT;		///< 设置采样宽度为16bit
  i2s_format.rate = I2S_SAMPLE_RATE_48000;		///< 设置采样率为48K
  i2s_format.polarity = I2S_LEFT_POLARITY_LOW;    ///< WSCLK的高低极性电平对应的声道
  i2s_format.sclk_nfs = I2S_SCLK_32FS;			///< 设置SCLK为FS的32倍
  i2s_format.mclk_nfs = I2S_MCLK_256FS;           ///< 设置MCLK为FS的256倍
  ret = csi_i2s_format(&g_i2s, &format);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

#### csi_i2s_format_t

| 类型                       | 成员     | 说明                      |
| :------------------------- | -------- | :------------------------ |
| csi_i2s_mode_t             | mode     | I2S主从模式设置           |
| csi_i2s_protocol_t         | protocol | I2S传输协议设置           |
| csi_i2s_ws_left_polarity_t | polarity | WSCLK电平极性对应声道设置 |
| csi_i2s_sample_rate_t      | rate     | I2S采样比率设置           |
| csi_i2s_sample_width_t     | width    | I2S采样宽度设置           |
| csi_i2s_sclk_freq_t        | sclk_nfs | SCLK的频率设置            |
| csi_i2s_mclk_freq_t        | mclk_nfs | MCLK的频率设置            |

##### csi_i2s_mode_t

| 类型            | 说明          |
| --------------- | ------------- |
| I2S_MODE_MASTER | I2S设置为主机 |
| I2S_MODE_SLAVE  | I2S设置为从机 |

##### csi_i2s_protocol_t

| 类型                       | 说明                       |
| -------------------------- | -------------------------- |
| I2S_PROTOCOL_I2S           | I2S传输协议为I2S           |
| I2S_PROTOCOL_MSB_JUSTIFIED | I2S传输协议为MSB_JUSTIFIED |
| I2S_PROTOCOL_LSB_JUSTIFIED | I2S传输协议为LSB_JUSTIFIED |
| I2S_PROTOCOL_PCM           | I2S传输协议为PCM           |

##### csi_i2s_ws_left_polarity_t

| 类型                   | 说明             |
| ---------------------- | ---------------- |
| I2S_LEFT_POLARITY_LOW  | 低电平对应左声道 |
| I2S_LEFT_POLARITY_HIGH | 高电平对应左声道 |

##### csi_i2s_sample_rate_t

| 类型                   | 说明            |
| ---------------------- | --------------- |
| I2S_SAMPLE_RATE_8000   | 采样比率8K      |
| I2S_SAMPLE_RATE_11025  | 采样比率11.025K |
| I2S_SAMPLE_RATE_12000  | 采样比率12K     |
| I2S_SAMPLE_RATE_16000  | 采样比率16K     |
| I2S_SAMPLE_RATE_22050  | 采样比率22.05K  |
| I2S_SAMPLE_RATE_24000  | 采样比率24K     |
| I2S_SAMPLE_RATE_32000  | 采样比率32K     |
| I2S_SAMPLE_RATE_44100  | 采样比率44.1K   |
| I2S_SAMPLE_RATE_48000  | 采样比率48K     |
| I2S_SAMPLE_RATE_96000  | 采样比率96K     |
| I2S_SAMPLE_RATE_192000 | 采样比率192K    |
| I2S_SAMPLE_RATE_256000 | 采样比率256K    |

##### csi_i2s_sample_width_t

| 类型                   | 说明           |
| ---------------------- | -------------- |
| I2S_SAMPLE_WIDTH_16BIT | 采样宽度16bits |
| I2S_SAMPLE_WIDTH_24BIT | 采样宽度24bits |
| I2S_SAMPLE_WIDTH_32BIT | 采样宽度32bits |

##### csi_i2s_sclk_freq_t

| 类型          | 说明               |
| ------------- | ------------------ |
| I2S_SCLK_16FS | SCLK频率为FS的16倍 |
| I2S_SCLK_32FS | SCLK频率为FS的32倍 |
| I2S_SCLK_48FS | SCLK频率为FS的48倍 |
| I2S_SCLK_64FS | SCLK频率为FS的64倍 |

##### csi_i2s_mclk_freq_t

| 类型           | 说明            |
| -------------- | --------------- |
| I2S_MCLK_256FS | MCLK频率为256倍 |
| I2S_MCLK_384FS | MCLK频率为384倍 |

### csi_i2s_tx_select_sound_channel

```c
csi_error_t csi_i2s_tx_select_sound_channel(csi_i2s_t *i2s, csi_i2s_sound_channel_t ch)
```

* 功能描述:

  * 设置I2S发送通道配置单/双声道。
  
* 参数：
  * `i2s`: 实例句柄。
  * `ch`:设置单双声道。
  
* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_i2s_t g_i2s;
  csi_error_t ret;
  ret = csi_i2s_tx_select_sound_channel(&g_i2s, I2S_LEFT_CHANNEL);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

#### csi_i2s_sound_channel_t

| 类型                   | 说明             |
| ---------------------- | ---------------- |
| I2S_LEFT_CHANNEL       | 单声道选择左声道 |
| I2S_RIGHT_CHANNEL      | 单声道选择右声道 |
| I2S_LEFT_RIGHT_CHANNEL | 选择左右声道     |

### csi_i2s_rx_select_sound_channel

```c
csi_error_t csi_i2s_rx_select_sound_channel(csi_i2s_t *i2s, csi_i2s_sound_channel_t ch)
```

* 功能描述:

  * 设置I2S接收通道配置单/双声道。

* 参数：

  * `i2s`: 实例句柄。
  * `ch`:选择单声道。
  
* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_i2s_t g_i2s;
  csi_error_t ret;
  ret = csi_i2s_rx_select_sound_channel(&g_i2s, I2S_LEFT_CHANNEL);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_i2s_tx_link_dma

```c
csi_error_t csi_i2s_tx_link_dma(csi_i2s_t *i2s, csi_dma_ch_t *tx_dma)
```

* 功能描述：

  * 设置发送通道连接DMA。

* 参数：

  * `i2s`:实例句柄
  * `tx_dma`:用于发送的DMA通道句柄，传NULL时会关闭DMA。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_i2s_t g_i2s;
  csi_error_t ret;
  csi_dma_ch_t g_dma_ch_tx;
  /* 为发送设置DMA通道 */
  ret = csi_i2s_tx_link_dma(&g_i2s, &g_dma_ch_tx);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_i2s_rx_link_dma

```c
csi_error_t csi_i2s_rx_link_dma(csi_i2s_t *i2s, csi_dma_ch_t *rx_dma)
```

* 功能描述：

  * 设置接收通道连接DMA。

* 参数：

  * `i2s`:实例句柄
  * `rx_dma`:用于接收的DMA通道句柄，传NULL时会关闭DMA。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_i2s_t g_i2s;
  csi_error_t ret;
  csi_dma_ch_t g_dma_ch_rx;
  /* 为接收设置DMA通道 */
  ret = csi_i2s_rx_link_dma(&g_i2s, &g_dma_ch_rx);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_i2s_tx_set_buffer

```c
void csi_i2s_tx_set_buffer(csi_i2s_t *i2s, ringbuffer_t *buffer)
```

* 功能描述：

  * 对i2s发送通道实例注册ringbuffer缓冲区。

* 参数：

  * `i2s`:实例句柄。
  * `buffer`:ringbuffer地址。

* 返回值：

  * 无。

* 使用示例

  ```c
  /* 句柄使用前请先初始化 */
  static csi_i2s_t g_i2s;
  csi_error_t ret;
  ringbuffer_t tx_ring_buffer; 
  /* 将 ring_buffer传递到i2s实例 */
  ret = csi_i2s_tx_set_buffer(&g_i2s, &tx_ring_buffer);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

#### ringbuffer_t

| 成员     | 类型      | 描述                       |
| -------- | --------- | -------------------------- |
| buffer   | uint8_t * | 环形缓冲区地址             |
| size     | uint32_t  | 环形缓冲区大小             |
| write    | uint32_t  | 环形缓冲区当前写指针位置   |
| read     | uint32_t  | 环形缓冲区当前读指针位置   |
| data_len | uint32_t  | 环形缓冲区当前可读数据长度 |

### csi_i2s_rx_set_buffer

```c
void csi_i2s_rx_set_buffer(csi_i2s_t *i2s, ringbuffer_t *buffer)
```

* 功能描述：

  * 对i2s接收通道实例注册ringbuffer缓冲区

* 参数：

  * `i2s`:实例句柄
  * `buffer`:ringbuffer地址

* 返回值：

  * 无。

* 使用示例

  ```c
  /* 句柄使用前请先初始化 */
  static csi_i2s_t g_i2s;
  csi_error_t ret;
  ringbuffer_t rx_ring_buffer; 
  /* 将 ring_buffer传递到i2s实例 */
  ret = csi_i2s_rx_set_buffer(&g_i2s, &rx_ring_buffer);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_i2s_tx_set_period

```c
csi_error_t csi_i2s_tx_set_period(csi_i2s_t *i2s, uint32_t period)
```

* 功能描述：

  * 设置完成多少数据发送上报周期，注意：，这里要设置period要小于缓冲区大小，且可以整除缓冲区大小。

* 参数：

  * `i2s`:实例句柄。
  * `period`：产生回调数据的大小。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_i2s_t g_i2s;
  csi_error_t ret;
  #define I2S_MASTER_TX_BUF_SIZE 2048                       ///< 缓冲区长度
  uint8_t i2s_master_tx_buf[I2S_MASTER_TX_BUF_SIZE]; ///< 缓冲区
  /* 将 ring_buffer传递到i2s实例 */
  ret = csi_i2s_tx_set_period(&g_i2s, (I2S_MASTER_TX_BUF_SIZE >> 1));
  if (ret != CSI_OK) {
      return -1;
  }
  ```

  > 注意：
  >
  > 设置period，必须设置为环形缓冲区的一半长度，否则驱动不会正常工作

### csi_i2s_rx_set_period

```c
csi_error_t csi_i2s_rx_set_period(csi_i2s_t *i2s, uint32_t period)
```

* 功能描述：

  * 设置完成多少数据接收上报周期，注意：，这里要设置period要小于缓冲区大小，且可以整除缓冲区大小。

* 参数：

  * `i2s`:实例句柄。
  * `period`：产生回调数据的大小。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_i2s_t g_i2s;
  csi_error_t ret;
  #define I2S_MASTER_RX_BUF_SIZE 2048                       ///< 缓冲区长度
  uint8_t i2s_master_rx_buf[I2S_MASTER_RX_BUF_SIZE];       ///< 缓冲区
  /* 将 ring_buffer传递到i2s实例 */
  ret = csi_i2s_rx_set_period(&g_i2s, (I2S_MASTER_RX_BUF_SIZE >> 1));
  if (ret != CSI_OK) {
      return -1;
  }
  ```

  > 注意：
  >
  > 设置period，必须设置为环形缓冲区的一半长度，否则驱动不会正常工作

### csi_i2s_tx_buffer_avail

```c
uint32_t csi_i2s_tx_buffer_avail(csi_i2s_t *i2s)
```

* 功能描述：

  * 读取缓冲区剩余数据。

* 参数：

  * `i2s`:实例句柄。

* 返回值：

  * uint32_t 当前缓冲区剩余数据数。

* 使用实例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_i2s_t g_i2s;
  uint32_t num;
  num = csi_i2s_tx_buffer_avail(&g_i2s);
  ```

### csi_i2s_rx_buffer_avail

```c
uint32_t csi_i2s_rx_buffer_avail(csi_i2s_t *i2s)
```

* 功能描述：

  * 读取缓冲区剩余数据。

* 参数：

  * `i2s`:实例句柄。

* 返回值：

  * uint32_t 当前缓冲区剩余数据数。

* 使用实例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_i2s_t g_i2s;
  uint32_t num;
  num = csi_i2s_rx_buffer_avail(&g_i2s);
  ```


### csi_i2s_tx_buffer_reset

```c
csi_error_t csi_i2s_tx_buffer_reset(csi_i2s_t *i2s)
```

* 功能描述：

  * 对I2S发送缓冲区（ringbuffer）进行重置。

* 参数：

  * `i2s`:实例句柄。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_i2s_t g_i2s;
  csi_error_t ret;
  ret = csi_i2s_tx_buffer_reset(&g_i2s);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_i2s_rx_buffer_reset

```c
csi_error_t csi_i2s_rx_buffer_reset(csi_i2s_t *i2s)
```

* 功能描述：

  * 对I2S接收的缓冲区进行重置。

* 参数：

  * `i2s`:实例句柄。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_i2s_t g_i2s;
  csi_error_t ret;
  ret = csi_i2s_rx_buffer_reset(&g_i2s);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_i2s_send

```c
int32_t csi_i2s_send(csi_i2s_t *i2s, const void *data, uint32_t size)
```

* 功能描述：

  *  i2s 同步模式发送数据。

* 参数：

  * `i2s`:实例句柄。
  * `data`：用户缓冲区数据指针。
  * `size`：要发送的数据长度。

* 返回值：

  * int32_t 返回发送的数据长度或错误码csi_error_t。

* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_i2s_t g_i2s;
  uint8_t send_data[1024];
  int32_t num;
  num = csi_i2s_send(&g_i2s, send_data, sizeof(send_data));
  if (num != sizeof(send_data)) {
      return -1;
  }
  ```

### csi_i2s_send_async

```c
uint32_t csi_i2s_send_async(csi_i2s_t *i2s, const void *data, uint32_t size)
```

* 功能描述：

  * I2S 异步模式发送数据。

* 参数：

  * `i2s`:实例句柄。
  * `data`:用户缓冲区数据指针。
  * `size`:要发送的数据长度。

* 返回值：

  * uint32_t 返回成功写入缓冲区的数据长度。

* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_i2s_t g_i2s;
  uint8_t send_data[1024];
  uint32_t num;
  num = csi_i2s_send_async(&g_i2s, send_data, sizeof(send_data));
  ```

  > 注意：
  >
  > 由于I2S的主要功能是播放音频，提供的接口都是写数据到缓冲区的。用户发送一段音频需要根据I2S的回调函数，不断的写数据到缓冲区，数据进入缓冲区I2S会自动发送数据。

### csi_i2s_receive

```c
int32_t csi_i2s_receive(csi_i2s_t *i2s, const void *data, uint32_t size)
```

* 功能描述：

  *  i2s 同步模式接收数据。

* 参数：

  * `i2s`:实例句柄。
  * `data`：用户缓冲区数据指针。
  * `size`：要接收的数据长度。

* 返回值：

  * uint32_t 返回接收的数据长度。

* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_i2s_t g_i2s;
  uint8_t read_data[1024];
  int32_t num;
  num = csi_i2s_receive(&g_i2s, read_data, sizeof(read_data));
  if (num != sizeof(read_data)) {
      return -1;
  }
  ```

### csi_i2s_receive_async

```c
uint32_t csi_i2s_receive_async(csi_i2s_t *i2s, const void *data, uint32_t size)
```

* 功能描述：

  * I2S 异步模式接收数据。

* 参数：

  * `i2s`:实例句柄。
  * `data`:用户缓冲区数据指针。
  * `size`:要读取的数据长度。

* 返回值：

  * uint32_t 返回成功读取缓冲区的数据长度。

* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_i2s_t g_i2s;
  uint8_t read_data[1024];
  uint32_t num;
  num = csi_i2s_receive_async(&g_i2s, read_data, sizeof(read_data));
  ```

  > 注意：
  >
  > 由于I2S的主要功能是播放音频，提供的接口都是写数据到缓冲区的。用户发送一段音频需要根据I2S的回调函数，不断的写数据到缓冲区，数据进入缓冲区I2S会自动发送数据。

### csi_i2s_send_start

```c
csi_error_t csi_i2s_send_start(csi_i2s_t *i2s)
```

* 功能描述：

  * I2S 发送数据流开始运行，调用该接口后，DMA会开始数据搬运。

* 参数：

  * `i2s`:实例句柄。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_i2s_t g_i2s;
  csi_error_t ret;
  ret = csi_i2s_send_start(&g_i2s);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_i2s_send_resume

```c
csi_error_t csi_i2s_send_resume(csi_i2s_t *i2s)
```

* 功能描述：

  * I2S发送数据暂停后调用该接口，可以在暂停的缓冲区位置重新启动发送数据。

* 参数：

  * `i2s`:实例句柄。

* 返回值：
  
    * 错误码csi_error_t。
    
* 使用示例：

  ```c
    /* 句柄使用前请先初始化 */
    static csi_i2s_t g_i2s;
    csi_error_t ret;
    ret = csi_i2s_send_resume(&g_i2s);
    if (ret != CSI_OK) {
        return -1;
    }
  ```

### csi_i2s_send_pause

```c
csi_error_t csi_i2s_send_pause(csi_i2s_t *i2s)
```

* 功能描述：

  * 调用该接口，DMA停止搬运，数据流停止，但是会保留缓冲区，保留发送的指针，以便恢复发送。

* 参数：

  * `i2s`:实例句柄。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_i2s_t g_i2s;
  csi_error_t ret;
  ret = csi_i2s_send_pause(&g_i2s);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_i2s_send_stop

```c
csi_error_t csi_i2s_send_stop(csi_i2s_t *i2s)
```

* 功能描述：

  * I2S 发送数据流停止，调用该接口后，缓冲区会重置，DMA会停止。

* 参数：

  * `i2s`:实例句柄。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_i2s_t g_i2s;
  csi_error_t ret;
  ret = csi_i2s_send_stop(&g_i2s);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_i2s_receive_start

```c
csi_error_t csi_i2s_receive_start(csi_i2s_t *i2s)
```

* 功能描述：

  * I2S 接收数据流开始运行，调用该接口后，DMA会不断的从I2S拿取数据到缓冲区。

* 参数：

  * `i2s`:实例句柄。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_i2s_t g_i2s;
  csi_error_t ret;
  ret = csi_i2s_receive_start(&g_i2s);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_i2s_receive_stop

```c
csi_error_t csi_i2s_receive_stop(csi_i2s_t *i2s)
```

* 功能描述：

  * I2S 接收数据流停止，调用该接口后，缓冲区会重置，DMA会停止。

* 参数：

  * `i2s`:实例句柄。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_i2s_t g_i2s;
  csi_error_t ret;
  ret = csi_i2s_receive_stop(&g_i2s);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_i2s_attach_callback

```c
csi_error_t csi_i2s_attach_callback(csi_i2s_t *i2s, void *callback, void *arg)
```

* 功能描述：
  * 注册回调函数。
* 参数：
  * `i2s`:实例句柄。
  * `callback`：i2s实例的事件回调函数（一般在中断上下文执行）。
  * `arg`: 回调函数参数（可选，由用户定义）。
* 返回值：
  * 错误码csi_error_t。

#### callback

```c
void (*callback)(csi_i2s_t *i2s, csi_i2s_event_t event, void *arg)
```

其中 i2s 为设备句柄，event 为传给回调函数的事件类型，arg 为用户自定义的回调函数对应的参数。 

i2s  回调事件枚举类型csi_i2s_event_t定义如下：

| 事件类型                     | 事件说明                  |
| :-------------------------- | ------------------------- |
| I2S_EVENT_TRANSMIT_COMPLETE | 数据发送完成事件           |
| I2S_EVENT_RECEIVE_COMPLETE  | 数据接收完成错误事件       |
| I2S_EVENT_TX_BUFFER_EMPYT   | I2S发送FIFO数据为空       |
| I2S_EVENT_RX_BUFFER_FULL    | I2S接收FIFO数据满         |
| I2S_EVENT_ERROR_OVERFLOW    | I2S总线产生FIFO溢出错误事件|
| I2S_EVENT_ERROR_UNDERFLOW   | I2S总线产生FIFO下溢错误事件|
| I2S_EVENT_ERROR             | I2S总线数据错误           |

>注意：
>在使用异步工作模式前，必须调用本函数来注册回调函数，否则将无法使用异步接口。

### csi_i2s_detach_callback

```c
void csi_i2s_detach_callback(csi_i2s_t *i2s)
```

* 功能描述：
  
  * 注销I2S设备的回调函数。
  
* 参数：
  
  * `i2s`：实例句柄。
  
* 返回值：
  * 无。
  

### csi_i2s_get_state

```c
csi_error_t csi_i2s_get_state(csi_i2s_t *i2s, csi_state_t *state)
```

* 功能描述;
  * 获取`i2s`的状态。通过此函数来判断I2S设备在获取状态的时刻是否可以进行send和receive操作。
* 参数：
  * i2s：实例句柄。
  * state: 用于返回状态信息的参数地址。
* 返回值：
  * 错误码csi_error_t。

## 初始化及配置示例

```c
/* 句柄空间一般使用静态空间 */
static csi_i2s_t g_i2s;
csi_dma_ch_t dma_ch_tx_handle;
#define I2S_TX_BUF_SIZE 2048
uint8_t i2s_tx_buf[I2S_TX_BUF_SIZE];  ///< 定义接收的缓冲区
static ringbuffer_t tx_ring_buffer;		     ///< 定义环形缓冲区

volatile uint8_t cb_i2s_transfer_flag = 0;
static void i2s_event_cb_fun(csi_i2s_t *i2s, csi_i2s_event_t event, void *arg)
{
    if (event == I2S_EVENT_TRANSMIT_COMPLETE) {
        cb_i2s_transfer_flag = 1;
    }
}

/* 本示例 示例了I2S的master发送流启动 */
int main(void) {
    csi_error_t ret;
    csi_i2s_format_t i2s_format;
    /* init函数的idx参数，请根据soc的实际情况进行选择 */
    ret = csi_i2s_init(&g_i2s, 0);
    if (ret != CSI_OK) {
        return -1;
    }
    
    csi_i2s_attach_callback(&g_i2s, i2s_event_cb_fun, NULL);  ///< 注册回调函数

    i2s_format.mode = I2S_MODE_MASTER;			    	 ///< 设置I2S为主机模式
    i2s_format.protocol = I2S_PROTOCOL_I2S;		    	 ///< 设置为I2S协议
    i2s_format.width = I2S_SAMPLE_WIDTH_16BIT;	    	 ///< 设置采样宽度为16bits
    i2s_format.rate = I2S_SAMPLE_RATE_48000;	    	 ///< 设置采样比率为48K
    i2s_format.polarity = I2S_LEFT_POLARITY_LOW;    	 ///< 设置高电平代表左声道
    csi_i2s_format(&g_i2s, &i2s_format);	             ///< 调用该接口进行设置I2S

    csi_i2s_tx_link_dma(&g_i2s, &dma_ch_tx_handle);      ///<设置I2S的dma句柄，连接DMA

    csi_i2s_tx_set_period(&g_i2s, I2S_TX_BUF_SIZE / 2);  ///< 设置发送period值
    tx_ring_buffer.buffer = i2s_tx_buf;					 ///< 将缓冲区地址传递给ringbuffer
    tx_ring_buffer.size = I2S_TX_BUF_SIZE;			     ///< 将缓冲区长度传递给ringbuffer
    csi_i2s_tx_set_buffer(&g_i2s, &tx_ring_buffer);
    csi_i2s_tx_buffer_reset(&g_i2s);					 ///< 将缓冲区重置

    csi_i2s_enable(&g_i2s, true);						 ///< 开启I2S外设
    csi_i2s_send_start(&g_i2s);							 ///< 开启I2S发送流
}
```

> 注意：
>
> 开启I2S的enable
>
> 开启发送流以后，假如缓冲区是刚重置的状态 将对外发数据都是0



## 同步模式传输

> 注意：
>
> 无论是同步模式，异步模式都要求注册回调函数，DMA进行连接，启动发送流

### 同步模式发送数据

```c
/* 句柄空间一般使用静态空间 */
static csi_i2s_t g_i2s;
csi_dma_ch_t dma_ch_tx_handle;
#define I2S_TX_BUF_SIZE 2048
uint8_t i2s_tx_buf[I2S_TX_BUF_SIZE];         ///< 定义缓冲区
static ringbuffer_t tx_ring_buffer;          ///< 定义环形缓冲区
uint8_t write_data[2048];                    ///< 用户发送数据
volatile uint8_t cb_i2s_transfer_flag = 0;
static void i2s_event_cb_fun(csi_i2s_t *i2s, csi_i2s_event_t event, void *arg)
{
    if (event == I2S_EVENT_TRANSMIT_COMPLETE) {
        cb_i2s_transfer_flag = 1;
    }
}

/* 本示例 示例了I2S的master发送流启动 */
int main(void) {
    csi_error_t ret;
    csi_i2s_format_t i2s_format;
    /* init函数的idx参数，请根据soc的实际情况进行选择 */
    ret = csi_i2s_init(&g_i2s, 0);
    if (ret != CSI_OK) {
        return -1;
    }
    

csi_i2s_attach_callback(&g_i2s, i2s_event_cb_fun, NULL);  ///< 注册回调函数

i2s_format.mode = I2S_MODE_MASTER;			    	 ///< 设置I2S为主机模式
i2s_format.protocol = I2S_PROTOCOL_I2S;		    	 ///< 设置为I2S协议
i2s_format.width = I2S_SAMPLE_WIDTH_16BIT;	    	 ///< 设置采样宽度为16bits
i2s_format.rate = I2S_SAMPLE_RATE_48000;	    	 ///< 设置采样比率为48K
i2s_format.polarity = I2S_LEFT_POLARITY_LOW;    	 ///< 设置高电平代表左声道
csi_i2s_format(&g_i2s, &i2s_format);	             ///< 调用该接口进行设置I2S

csi_i2s_tx_link_dma(&g_i2s, &dma_ch_tx_handle);      ///<设置I2S的dma句柄，连接DMA

csi_i2s_tx_set_period(&g_i2s, I2S_TX_BUF_SIZE / 2);  ///< 设置发送period值
tx_ring_buffer.buffer = i2s_tx_buf;					 ///< 将缓冲区地址传递给ringbuffer
tx_ring_buffer.size = I2S_TX_BUF_SIZE;			     ///< 将缓冲区长度传递给ringbuffer
csi_i2s_tx_set_buffer(&g_i2s, &tx_ring_buffer);
csi_i2s_tx_buffer_reset(&g_i2s);					 ///< 将缓冲区重置

csi_i2s_enable(&g_i2s, true);						 ///< 开启I2S外设
csi_i2s_send_start(&g_i2s);							 ///< 开启I2S发送流
                                  
for (uint32_t i = 0; i < sizeof(write_data); i++) {
	write_data[i] = 0x5a;					
}

int32_t write_size = 0;
write_size = csi_i2s_send(&g_i2s, write_data, sizeof(write_data)); ///<等待数据发送

if (write_size == sizeof(write_data)) {
	printf("test_i2s successfully\n");
} else {
	printf("test_i2s failed\n");
} 
csi_i2s_enable(&g_i2s, 0);
csi_i2s_tx_link_dma(&g_i2s, NULL);
csi_i2s_detach_callback(&g_i2s);
csi_i2s_uninit(&g_i2s);
}
```

### 同步模式接收数据

```c
/* 句柄空间一般使用静态空间 */
static csi_i2s_t g_i2s;
csi_dma_ch_t dma_ch_rx_handle;
#define I2S_RX_BUF_SIZE 2048
uint8_t i2s_rx_buf[I2S_RX_BUF_SIZE];         ///< 定义缓冲区
static ringbuffer_t rx_ring_buffer;          ///< 定义环形缓冲区
uint8_t read_data[2048];                     ///< 用户接收数据
volatile uint8_t cb_i2s_transfer_flag = 0;
static void i2s_event_cb_fun(csi_i2s_t *i2s, csi_i2s_event_t event, void *arg)
{
    if (event == I2S_EVENT_TRANSMIT_COMPLETE) {
        cb_i2s_transfer_flag = 1;
    }
}

/* 本示例 示例了I2S的master接收流启动 */
int main(void) {
    csi_error_t ret;
    csi_i2s_format_t i2s_format;
    /* init函数的idx参数，请根据soc的实际情况进行选择 */
    ret = csi_i2s_init(&g_i2s, 0);
    if (ret != CSI_OK) {
        return -1;
    }
    

csi_i2s_attach_callback(&g_i2s, i2s_event_cb_fun, NULL);  ///< 注册回调函数

i2s_format.mode = I2S_MODE_MASTER;			    	 ///< 设置I2S为主机模式
i2s_format.protocol = I2S_PROTOCOL_I2S;		    	 ///< 设置为I2S协议
i2s_format.width = I2S_SAMPLE_WIDTH_16BIT;	    	 ///< 设置采样宽度为16bits
i2s_format.rate = I2S_SAMPLE_RATE_48000;	    	 ///< 设置采样比率为48K
i2s_format.polarity = I2S_LEFT_POLARITY_LOW;    	 ///< 设置高电平代表左声道
csi_i2s_format(&g_i2s, &i2s_format);	             ///< 调用该接口进行设置I2S

csi_i2s_rx_link_dma(&g_i2s, &dma_ch_rx_handle);      ///<设置I2S的dma句柄，连接DMA

csi_i2s_rx_set_period(&g_i2s, I2S_RX_BUF_SIZE / 2);  ///< 设置发送period值
tx_ring_buffer.buffer = i2s_rx_buf;					 ///< 将缓冲区地址传递给ringbuffer
tx_ring_buffer.size = I2S_RX_BUF_SIZE;			     ///< 将缓冲区长度传递给ringbuffer
csi_i2s_rx_set_buffer(&g_i2s, &rx_ring_buffer);
csi_i2s_rx_buffer_reset(&g_i2s);					 ///< 将缓冲区重置

csi_i2s_enable(&g_i2s, true);						 ///< 开启I2S外设
csi_i2s_receive_start(&g_i2s);					     ///< 开启I2S接收流

int32_t read_size = 0;
read_size = csi_i2s_receive(&g_i2s, read_data, sizeof(read_data)); ///<等待数据发送

if (read_size == sizeof(read_data)) {
	printf("test_i2s successfully\n");
} else {
	printf("test_i2s failed\n");
} 
csi_i2s_enable(&g_i2s, 0);
csi_i2s_rx_link_dma(&g_i2s, NULL);
csi_i2s_detach_callback(&g_i2s);
csi_i2s_uninit(&g_i2s);
}
```

## 异步模式传输

> 注意：
>
> 无论是同步模式，异步模式都要求注册回调函数，DMA进行连接，启动发送流

### 异步模式发送数据

```c
/* 句柄空间一般使用静态空间 */
static csi_i2s_t g_i2s;
csi_dma_ch_t dma_ch_tx_handle;
#define I2S_TX_BUF_SIZE 2048
uint8_t i2s_tx_buf[I2S_TX_BUF_SIZE];         ///< 定义缓冲区
static ringbuffer_t tx_ring_buffer;          ///< 定义环形缓冲区
uint8_t write_data[2048];                    ///< 用户发送数据
volatile uint8_t cb_i2s_transfer_flag = 0;
static void i2s_event_cb_fun(csi_i2s_t *i2s, csi_i2s_event_t event, void *arg)
{
    if (event == I2S_EVENT_TRANSMIT_COMPLETE) {
        cb_i2s_transfer_flag--;
    }
}

/* 本示例 示例了I2S的master发送流启动 */
int main(void) {
    csi_error_t ret;
    csi_i2s_format_t i2s_format;
    /* init函数的idx参数，请根据soc的实际情况进行选择 */
    ret = csi_i2s_init(&g_i2s, 0);
    if (ret != CSI_OK) {
        return -1;
    }
    

csi_i2s_attach_callback(&g_i2s, i2s_event_cb_fun, NULL);  ///< 注册回调函数

i2s_format.mode = I2S_MODE_MASTER;			    	 ///< 设置I2S为主机模式
i2s_format.protocol = I2S_PROTOCOL_I2S;		    	 ///< 设置为I2S协议
i2s_format.width = I2S_SAMPLE_WIDTH_16BIT;	    	 ///< 设置采样宽度为16bits
i2s_format.rate = I2S_SAMPLE_RATE_48000;	    	 ///< 设置采样比率为48K
i2s_format.polarity = I2S_LEFT_POLARITY_LOW;    	 ///< 设置高电平代表左声道
csi_i2s_format(&g_i2s, &i2s_format);	             ///< 调用该接口进行设置I2S

csi_i2s_tx_link_dma(&g_i2s, &dma_ch_tx_handle);      ///<设置I2S的dma句柄，连接DMA

csi_i2s_tx_set_period(&g_i2s, I2S_TX_BUF_SIZE / 2);  ///< 设置发送period值
tx_ring_buffer.buffer = i2s_tx_buf;					 ///< 将缓冲区地址传递给ringbuffer
tx_ring_buffer.size = I2S_TX_BUF_SIZE;			     ///< 将缓冲区长度传递给ringbuffer
csi_i2s_tx_set_buffer(&g_i2s, &tx_ring_buffer);
csi_i2s_tx_buffer_reset(&g_i2s);				     ///< 将缓冲区重置

csi_i2s_enable(&g_i2s, true);						 ///< 开启I2S外设
csi_i2s_send_start(&g_i2s);							 ///< 开启I2S发送流
                                  
for (uint32_t i = 0; i < sizeof(write_data); i++) {
	write_data[i] = 0x5a;					
}

uint32_t write_size = 0;
cb_i2s_transfer_flag = sizeof(write_data) / g_i2s->tx_period;///< 请确保发送数据是period的整数倍
write_size = csi_i2s_send_async(&g_i2s, write_data, sizeof(write_data)); ///<等待数据发送
while(cb_i2s_transfer_flag != 0); ///<等待同步
if (write_size == sizeof(write_data)) {
	printf("test_i2s successfully\n");
} else {
	printf("test_i2s failed\n");
} 
csi_i2s_enable(&g_i2s, 0);
csi_i2s_tx_link_dma(&g_i2s, NULL);
csi_i2s_detach_callback(&g_i2s);
csi_i2s_uninit(&g_i2s);
}
```

### 异步模式接收数据

```c
/* 句柄空间一般使用静态空间 */
static csi_i2s_t g_i2s;
csi_dma_ch_t dma_ch_rx_handle;
#define I2S_RX_BUF_SIZE 2048
uint8_t i2s_rx_buf[I2S_RX_BUF_SIZE];         ///< 定义缓冲区
static ringbuffer_t rx_ring_buffer;          ///< 定义环形缓冲区
uint8_t read_data[2048];                     ///< 用户接收数据
volatile uint8_t cb_i2s_transfer_flag = 0;
static void i2s_event_cb_fun(csi_i2s_t *i2s, csi_i2s_event_t event, void *arg)
{
    if (event == I2S_EVENT_TRANSMIT_COMPLETE) {
        cb_i2s_transfer_flag = 1;
    }
}

/* 本示例 示例了I2S的master接收流启动 */
int main(void) {
    csi_error_t ret;
    csi_i2s_format_t i2s_format;
    /* init函数的idx参数，请根据soc的实际情况进行选择 */
    ret = csi_i2s_init(&g_i2s, 0);
    if (ret != CSI_OK) {
        return -1;
    }
    

csi_i2s_attach_callback(&g_i2s, i2s_event_cb_fun, NULL);  ///< 注册回调函数

i2s_format.mode = I2S_MODE_MASTER;			    	 ///< 设置I2S为主机模式
i2s_format.protocol = I2S_PROTOCOL_I2S;		    	 ///< 设置为I2S协议
i2s_format.width = I2S_SAMPLE_WIDTH_16BIT;	    	 ///< 设置采样宽度为16bits
i2s_format.rate = I2S_SAMPLE_RATE_48000;	    	 ///< 设置采样比率为48K
i2s_format.polarity = I2S_LEFT_POLARITY_LOW;    	 ///< 设置高电平代表左声道
csi_i2s_format(&g_i2s, &i2s_format);	             ///< 调用该接口进行设置I2S

csi_i2s_rx_link_dma(&g_i2s, &dma_ch_rx_handle);      ///<设置I2S的dma句柄，连接DMA

csi_i2s_rx_set_period(&g_i2s, I2S_RX_BUF_SIZE / 2);  ///< 设置发送period值
tx_ring_buffer.buffer = i2s_rx_buf;					 ///< 将缓冲区地址传递给ringbuffer
tx_ring_buffer.size = I2S_RX_BUF_SIZE;			     ///< 将缓冲区长度传递给ringbuffer
csi_i2s_rx_set_buffer(&g_i2s, &rx_ring_buffer);
csi_i2s_rx_buffer_reset(&g_i2s);					 ///< 将缓冲区重置

csi_i2s_enable(&g_i2s, true);						 ///< 开启I2S外设
csi_i2s_receive_start(&g_i2s);					     ///< 开启I2S接收流

printf("start i2s receive\n");
uint32_t read_size = 0;
uint32_t period_num = sizeof(read_data) / i2s_slave.rx_period;///< 请确保数据是period的整数倍
cb_slave_transfer_flag = 0;
while (1) {
	if (cb_slave_transfer_flag == 1) {
        cb_slave_transfer_flag = 0;
        read_size += csi_i2s_receive_async(&g_i2s, (read_data + read_size), (sizeof(read_data) - read_size));///接收完注意：偏移接收指针
        period_num --;

        if (period_num == 0) {
            break;
        }
    }
}

if (read_size == sizeof(read_data)) {
	printf("test_i2s successfully\n");
} else {
	printf("test_i2s failed\n");
} 
csi_i2s_enable(&g_i2s, 0);
csi_i2s_rx_link_dma(&g_i2s, NULL);
csi_i2s_detach_callback(&g_i2s);
csi_i2s_uninit(&g_i2s);
}
```

