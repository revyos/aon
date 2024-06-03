# CODEC
-----

## 说明

CODEC在这里指的是同时具有D/A（数字讯号转换成模拟讯号）和A/D（模拟讯号转换成数字讯号）转换功能的编解码器，播放音乐的时候用到的是D/A转换功能。在录音的时候用到的是A/D转换功能。

在接口中，D/A指的是输出通道，A/D指的是输入通道。

## 接口列表

CODEC的CSI接口如下所示：

| 函数                             | 说明                          |
| -------------------------------- | ----------------------------- |
| csi_codec_init                   | CODEC设备初始化               |
| csi_codec_uninit                 | CODEC设备去初始化             |
| csi_codec_output_open            | CODEC输出通道打开             |
| csi_codec_output_config          | CODEC输出通道配置             |
| csi_codec_output_attach_callback | CODEC输出通道注册回调函数     |
| csi_codec_output_detach_callback | CODEC输出通道注销回调函数     |
| csi_codec_output_close           | CODEC输出通道关闭             |
| csi_codec_output_link_dma        | CODEC输出通道配置DMA          |
| csi_codec_output_write           | CODEC输出通道同步模式发送数据 |
| csi_codec_output_write_async     | CODEC输出通道异步模式发送数据 |
| csi_codec_output_start           | CODEC开始输出音频流           |
| csi_codec_output_stop            | CODEC结束输出音频流           |
| csi_codec_output_pause           | CODEC暂停输出音频流           |
| csi_codec_output_resume          | CODEC恢复输出音频流           |
| csi_codec_output_buffer_avail    | 返回CODEC输出缓冲区内可用数据量|
| csi_codec_output_buffer_remain   | 返回CODEC输出缓冲区内已用数据量|
| csi_codec_output_buffer_reset    | 重置CODEC输出缓冲区           |
| csi_codec_output_mute            | CODEC输出通道静音             |
| csi_codec_output_digital_gain    | CODEC输出通道设置数字增益     |
| csi_codec_output_analog_gain     | CODEC输出通道设置模拟增益     |
| csi_codec_output_mix_gain        | CODEC输出通道设置混频增益     |
| csi_codec_output_get_state       | CODEC输出通道获取当前读写状态 |
| csi_codec_input_open             | CODEC输入通道打开             |
| csi_codec_input_config           | CODEC输入通道配置             |
| csi_codec_input_attach_callback  | CODEC输入通道注册回调函数     |
| csi_codec_input_detach_callback  | CODEC输入通道注销回调函数     |
| csi_codec_input_close            | CODEC输入通道关闭             |
| csi_codec_input_link_dma         | CODEC输入通道配置DMA          |
| csi_codec_input_read             | CODEC输入通道同步模式读取数据 |
| csi_codec_input_read_async       | CODEC输入通道异步模式读取数据 |
| csi_codec_input_start            | CODEC接收输入音频流           |
| csi_codec_input_stop             | CODEC结束接收输入音频流       |
| csi_codec_input_buffer_avail     | 返回CODEC输入缓冲区内可用数据量|
| csi_codec_input_buffer_remain    | 返回CODEC输入缓冲区内已用数据量|
| csi_codec_input_buffer_reset     | 重置CODEC输入缓冲区           |
| csi_codec_input_mute             | CODEC输入通道静音             |
| csi_codec_input_digital_gain     | CODEC输入通道设置数字增益     |
| csi_codec_input_analog_gain      | CODEC输入通道设置模拟增益     |
| csi_codec_input_mix_gain         | CODEC输入通道设置混频增益     |
| csi_codec_input_get_state        | CODEC输入通道获取当前读写状态 |




## 接口描述

### csi_codec_init

```c
csi_error_t csi_codec_init(csi_codec_t *codec, uint32_t idx)
```

* 功能描述:

  * 通过设备ID初始化对应的CODEC实例。

* 参数:

  * `codec`: 设备句柄（需要用户申请句柄空间）。
  * `idx`: 设备ID。
  
* 返回值:

  * CSI_OK: 初始化成功。
  * CSI_ERROR: 初始化失败。

#### csi_codec_t

| 成员              | 类型      | 说明    |
| :------------- | :--------- | :--------- |
| dev | csi_dev_t | csi设备统一句柄     |
| output_chs | csi_codec_output_t | 输出通道句柄 |
| input_chs | csi_codec_input_t | 输入通道句柄 |
| *priv | void | 设备私有变量 |

#### ringbuffer_t

| 成员     | 类型      | 描述                       |
| -------- | --------- | -------------------------- |
| buffer   | uint8_t * | 环形缓冲区地址             |
| size     | uint32_t  | 环形缓冲区大小             |
| write    | uint32_t  | 环形缓冲区当前写指针位置   |
| read     | uint32_t  | 环形缓冲区当前读指针位置   |
| data_len | uint32_t  | 环形缓冲区当前可读数据长度 |

#### csi_codec_output_t

| 成员              | 类型                                                         | 描述                         |
| ----------------- | ------------------------------------------------------------ | ---------------------------- |
| codec             | csi_codec_t *                                                | CODEC设备句柄                |
| ch_idx            | uint32_t                                                     | 当前通道的序号               |
| callback          | void (*callback)(csi_codec_output_t *output, csi_codec_event_t event, void *arg) | 当前通道的回调               |
| arg               | void *                                                       | 当前通道的用户参数           |
| ring_buf          | ringbuffer_t *                                               | 当前通道的缓冲器句柄         |
| period            | uint32_t                                                     | 设置完成多少数据发送上报周期 |
| sound_channel_num | uint32_t                                                     | 声道数                       |
| state             | csi_state_t                                                  | 当前通道的状态               |
| dma               | csi_dma_ch_t *                                               | 当前通道的DMA句柄            |
| next              | struct csi_codec_output *                                    | 下一个输出通道的地址指针     |
| priv              | void *                                                       | 设备私有变量                 |

#### csi_codec_input_t

| 成员              | 类型                                                         | 描述                         |
| ----------------- | ------------------------------------------------------------ | ---------------------------- |
| codec             | csi_codec_t *                                                | CODEC设备句柄                |
| ch_idx            | uint32_t                                                     | 当前通道的序号               |
| callback          | void (*callback)(csi_codec_input_t *input, csi_codec_event_t event, void *arg) | 当前通道的回调               |
| arg               | void *                                                       | 当前通道的用户参数           |
| ring_buf          | ringbuffer_t *                                               | 当前通道的缓冲器句柄         |
| period            | uint32_t                                                     | 设置完成多少数据接收上报周期 |
| sound_channel_num | uint32_t                                                     | 声道数                       |
| state             | csi_state_t                                                  | 当前通道的状态               |
| dma               | csi_dma_ch_t *                                               | 当前通道的DMA句柄            |
| next              | struct csi_codec_input *                                     | 下一个输入通道的地址指针     |
| priv              | void *                                                       | 设备私有变量                 |


### csi_codec_uninit

```c
void csi_codec_uninit(csi_codec_t *codec)
```

* 功能描述:

  * codec实例反初始化。
  * 该接口会清理并释放相关的软硬件资源。
* 参数:

  * `codec`: 实例句柄。
* 返回值：
  * 无。

### csi_codec_output_open

```c
csi_error_t csi_codec_output_open(csi_codec_t *codec, csi_codec_output_t *ch, uint32_t ch_idx)
```

* 功能描述：

  * 将输出通道的ch句柄注册到codec句柄中。
  * 初始化输出通道有关的硬件资源。

* 参数：

  * `codec`: codec实例句柄。
  * `ch`: 输出通道的实例句柄。
  * `ch_idx`:通道的ID。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_t g_codec;
  csi_error_t ret;
  csi_codec_output_t output_ch;
  /* ch_idx 需要根据实际硬件是否支持填入*/
  ret = csi_codec_output_open(&g_codec, &output_ch, 0);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_codec_output_config

```c
csi_error_t csi_codec_output_config(csi_codec_output_t *ch, csi_codec_output_config_t *config)
```

* 功能描述：

  * 根据传入的配置配置输出通道。
  * 配置输出通道采样宽度、采样比率、设置缓冲区地址、设置输出通道的输出模式（差分输出还是单端输出）。

* 参数

  * `ch`:通道实例句柄。
  * `config`：配置参数结构体指针。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_t g_codec;
  csi_error_t ret;
  csi_codec_output_t output_ch;
  csi_codec_output_config_t output_config;     ///< 输出通道的配置参数
  output_config.bit_width = 16;                ///< 设置采样宽度为16bit	
  output_config.sample_rate = 48000;           ///< 设置采样比率为48K
  output_config.buffer = output_buf;           ///< 设置缓冲区地址
  output_config.buffer_size = OUTPUT_BUF_SIZE; ///< 设置缓冲区大小
  output_config.period = OUTPUT_BUF_SIZE/2;    ///< 设置发送多少数据上报值
  output_config.mode = CODEC_OUTPUT_DIFFERENCE;///< 设置差分输出
  ret = csi_codec_output_config(&output_ch,&output_config);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

#### csi_codec_output_config_t

| 成员              | 类型                    | 说明               |
| ----------------- | ----------------------- | ------------------ |
| sample_rate       | uint32_t                | 输出通道的采样比率 |
| bit_width         | uint32_t                | 输出通道的宽度     |
| mode              | csi_codec_output_mode_t | 输出通道的模式     |
| buffer            | uint8_t *               | 输出通道缓冲区地址 |
| buffer_size       | uint32_t                | 缓冲区地址长度     |
| period            | uint32_t                | 发送多少数据上报值 |
| sound_channel_num | uint32_t                | 声道数             |

#### csi_codec_output_mode_t

| 类型                      | 说明     |
| ------------------------- | -------- |
| CODEC_OUTPUT_SINGLE_ENDED | 差分输出 |
| CODEC_OUTPUT_DIFFERENCE   | 单端输出 |

### csi_codec_output_attach_callback

```c
csi_error_t csi_codec_output_attach_callback(csi_codec_output_t *ch, void *callback, void *arg)
```

* 功能描述：
  * 设置输出通道回调函数
* 参数：
  * `csi_codec_output_t`：输出通道实例句柄
  * `callback：codec`输出通道的事件回调函数（一般在上下文执行）函数传参见下文
  * `arg`:用户参数
* 返回值：
  * 错误码csi_error_t。

#### callback

```c
void (*callback)(csi_codec_output_t *output, csi_codec_event_t event, void *arg)
```

其中 output为输出通道句柄，event 为传给回调函数的事件类型，arg 为用户自定义的回调函数对应的参数。 

codec 回调事件枚举类型csi_codec_event_t定义如下：

| 类型                              | 说明                       |
| --------------------------------- | -------------------------- |
| CODEC_EVENT_PERIOD_READ_COMPLETE  | CODEC接收period数据数完成   |
| CODEC_EVENT_PERIOD_WRITE_COMPLETE | CODEC发送period数据数完成   |
| CODEC_EVENT_WRITE_BUFFER_EMPTY    | CODEC发送缓冲区已经空       |
| CODEC_EVENT_READ_BUFFER_FULL      | CODEC接收缓冲区已经满       |
| CODEC_EVENT_ERROR_OVERFLOW        | CODEC的FIFO产生溢出错误     |
| CODEC_EVENT_ERROR_UNDERFLOW       | CODEC的FIFO产生下溢错误     |
| CODEC_EVENT_ERROR                 | CODEC传输错误事件           |

### csi_codec_output_detach_callback

```c
void csi_codec_output_detach_callback(csi_codec_output_t *ch)
```

* 功能描述：
  * 注销CODEC 输出通道的回调函数。
* 参数：
  * `ch`：通道实例句柄。
* 返回值：
  * 无。

### csi_codec_output_close

```c
void csi_codec_output_close(csi_codec_output_t *ch)
```

* 功能描述：
  * 关闭输出通道。
  * 调用该接口会马上停止输出数据。
* 参数：
  * `ch`：通道实例句柄。
* 返回值：
  * 无。

### csi_codec_output_link_dma

```c
csi_error_t csi_codec_output_link_dma(csi_codec_output_t *ch, csi_dma_ch_t *dma)
```

*  功能描述：

  * 输出通道连接DMA。

* 参数：

  * `ch`：输出通道的实例句柄。
  * `dma`：dma实例句柄。

* 返回：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_output_t codec_output;
  csi_error_t ret;
  csi_dma_ch_t g_dma_ch;
  /* 为输出通道设置DMA通道 */
  ret = csi_codec_output_link_dma(&codec_output, &g_dma_ch);
  if (ret != CSI_OK) {
      return -1;
  }
  ```



### csi_codec_output_send

```c
uint32_t csi_codec_output_send(csi_codec_output_t *ch, const void *data, uint32_t size)
```

*  功能描述：

  * 输出通道同步模式发送数据。

* 参数：

  * `ch`：输出通道的实例句柄。
  * `data`：发送数据指针。
  * `size`: 发送数据长度。

* 返回：

  * uint32_t 发送成功数据的长度。

* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_output_t codec_output;
  uint8_t write_data[1024];
  uint32_t num;
  num = csi_codec_output_send(&codec_output, write_data, sizeof(write_data));
  if (num != sizeof(write_data)) {
      return -1;
  }
  ```

  

### csi_codec_output_send_async

```c
uint32_t csi_codec_output_send_async(csi_codec_output_t *ch, const void *data, uint32_t size)
```

*  功能描述：

  * 输出通道异步模式发送数据。

* 参数：

  * `ch`：输出通道的实例句柄。
  * `data`：发送数据指针。
  * `size`: 发送数据长度。

* 返回：

  * uint32_t 发送成功数据的长度。

* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_output_t codec_output;
  uint8_t write_data[1024];
  uint32_t num;
  /*调用该接口后，数据会写到缓冲区，用户需要根据设定的period值来判断数据是否发送完毕*/
  num = csi_codec_output_send_async(&codec_output, write_data, sizeof(write_data));
  if (num != sizeof(write_data)) {
      return -1;
  }
  ```

### csi_codec_output_start

```c
csi_error_t csi_codec_output_start(csi_codec_output_t *ch)
```

* 功能描述：

  * 输出通道开始数据流。

* 参数：

  * `ch`：输出通道的实例句柄。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_output_t codec_output;
  csi_error_t ret;
  num = csi_codec_output_start(&codec_output);
  if (num != CSI_OK) {
      return -1;
  }
  ```

### csi_codec_output_stop

```c
csi_error_t csi_codec_output_stop(csi_codec_output_t *ch)
```

* 功能描述：

  * 输出通道结束数据流。

* 参数：

  * `ch`：输出通道的实例句柄。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_output_t codec_output;
  csi_error_t ret;
  ret = csi_codec_output_stop(&codec_output);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_codec_output_pause

```c
csi_error_t csi_codec_output_pause(csi_codec_output_t *ch)
```

* 功能描述：

  * 输出通道暂停数据流。

* 参数：

  * `ch`：输出通道的实例句柄。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_output_t codec_output;
  csi_error_t ret;
  ret = csi_codec_output_pause(&codec_output);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_codec_output_resume

```c
csi_error_t csi_codec_output_resume(csi_codec_output_t *ch)
```

* 功能描述：

  * 输出通道恢复发送数据流。

* 参数：

  * `ch`：输出通道的实例句柄。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_output_t codec_output;
  csi_error_t ret;
  ret = csi_codec_output_resume(&codec_output);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_codec_output_buffer_avail

```c
uint32_t csi_codec_output_buffer_avail(csi_codec_output_t *ch)
```

* 功能描述：

  * 返回当前缓冲区剩余可用数据数。

* 参数：

  * `ch`：输出通道的实例句柄。

* 返回值：

  * uint32_t 剩余数据数。

* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_output_t codec_output;
  uint32_t num;
  num = csi_codec_output_buffer_avail(&codec_output);
  ```

### csi_codec_output_buffer_remain

```c
uint32_t csi_codec_output_buffer_remain(csi_codec_output_t *ch)
```

* 功能描述：

  * 返回当前缓冲区已用数据数。

* 参数：

  * `ch`：输出通道的实例句柄。

* 返回值：

  * uint32_t 已用数据数。

* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_output_t codec_output;
  uint32_t num;
  num = csi_codec_output_buffer_remain(&codec_output);
  ```

### csi_codec_output_buffer_reset

```c
csi_error_t csi_codec_output_buffer_reset(csi_codec_output_t *ch)
```

* 功能描述：

  * 对缓冲区数据全部置0，重置ringbuffer。

* 参数：

  * `ch`：输出通道的实例句柄。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_output_t codec_output;
  csi_error_t ret;
  ret = csi_codec_output_buffer_reset(&codec_output);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_codec_output_mute

```c
csi_error_t csi_codec_output_mute(csi_codec_output_t *ch, bool en)
```

* 功能描述：

  * 设置输出通道静音。

* 参数：

  * `ch`：输出通道的实例句柄。
  * `en`: true 代表输出静音,  Flase 代表输出不静音。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_output_t codec_output;
  csi_error_t ret;
  ret = csi_codec_output_mute(&codec_output, true);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_codec_output_digital_gain

```c
csi_error_t csi_codec_output_digital_gain(csi_codec_output_t *ch, uint32_t val)
```

* 功能描述：

  * 设置输出通道数码增益。

* 参数：

  * `ch`：输出通道的实例句柄。
  * `val`: 增益的DB值。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_output_t codec_output;
  csi_error_t ret;
  /* 设置的数码增益 需要根据实际硬件支持的值进行设置*/
  ret = csi_codec_output_digital_gain(&codec_output, 0x20);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_codec_output_analog_gain

```c
csi_error_t csi_codec_output_analog_gain(csi_codec_output_t *ch, uint32_t val)
```

* 功能描述：

  * 设置输出通道模拟增益。

* 参数：

  * `ch`：输出通道的实例句柄。
  * `val`: 增益的DB值。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_output_t codec_output;
  csi_error_t ret;
  /* 设置的数码增益 需要根据实际硬件支持的值进行设置*/
  ret = csi_codec_output_analog_gain(&codec_output, 0x20);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_codec_output_mix_gain

```c
csi_error_t csi_codec_output_mix_gain(csi_codec_output_t *ch, uint32_t val)
```

* 功能描述：

  * 设置输出通道混频增益。

* 参数：

  * `ch`：输出通道的实例句柄。
  * `val`: 增益的DB值。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_output_t codec_output;
  csi_error_t ret;
  /* 设置的混频增益 需要根据实际硬件支持的值进行设置*/
  ret = csi_codec_output_mix_gain(&codec_output, 0x20);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_codec_output_get_state

```c
csi_error_t csi_codec_output_get_state(csi_codec_output_t *ch, csi_state_t *state)
```

* 功能描述：

  * 获取输出通道的状态。通过此函数来判断code通道在获取状态的时刻是否可以进行读写操作。

* 参数

  * `ch`：实例句柄。
  * `state`: 用于返回状态信息的参数地址。

* 返回值：

  * 错误码csi_error_t。

### csi_codec_input_open

```c
csi_error_t csi_codec_input_open(csi_codec_t *codec, csi_codec_input_t *ch, uint32_t ch_idx)
```

* 功能描述：

  * 将输入通道的ch句柄注册到codec句柄中。
  * 初始化输入通道有关的硬件资源。

* 参数：

  * `codec`: codec实例句柄。
  * `ch`: 输入通道的实例句柄。
  * `ch_idx`:通道的ID。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_t g_codec;
  csi_error_t ret;
  csi_codec_input_t input_ch;
  /* ch_idx 需要根据实际硬件是否支持填入*/
  ret = csi_codec_input_open(&g_codec, &input_ch, 0);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_codec_input_config

```c
csi_error_t csi_codec_input_config(csi_codec_input_t *ch, csi_codec_input_config_t *config)
```

* 功能描述：

  * 根据传入的配置配置输入通道。
  * 配置输入通道采样宽度、采样比率、设置缓冲区地址、设置输入通道的输出模式（差分输入还是单端输入）。

* 参数

  * `ch`:通道实例句柄。
  * `config`：配置参数。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_t g_codec;
  #define CODEC_BUF_SIZE 2048
  uint8_t input_buf[CODEC_BUF_SIZE];  ///< 定义接收的缓冲区
  csi_error_t ret;
  csi_codec_input_t input_ch;
  csi_codec_input_config_t input_config;        ///< 输入通道的配置参数
  input_config.bit_width = 16;			      ///< 设置采样宽度为16bit	
  input_config.sample_rate = 48000;			  ///< 设置采样比率为48K
  input_config.buffer = input_buf;			  ///< 设置缓冲区地址
  input_config.buffer_size = CODEC_BUF_SIZE;    ///< 设置缓冲区大小
  input_config.period = 1024;	                  ///< 设置接收多少数据上报值
  input_config.mode = CODEC_INPUT_DIFFERENCE;  ///< 设置差分输入
  ret = csi_codec_input_config(&input_ch,&input_config);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

#### csi_codec_input_config_t

| 成员              | 类型                   | 说明               |
| ----------------- | ---------------------- | ------------------ |
| sample_rate       | uint32_t               | 输入通道的采样比率 |
| bit_width         | uint32_t               | 输入通道的宽度     |
| mode              | csi_codec_input_mode_t | 输入通道的模式     |
| buffer            | uint8_t *              | 输入通道缓冲区地址 |
| buffer_size       | uint32_t               | 缓冲区地址长度     |
| period            | uint32_t               | 接收多少数据上报值 |
| sound_channel_num | uint32_t               | 声道数             |

#### csi_codec_input_mode_t

| 类型                     | 说明     |
| ------------------------ | -------- |
| CODEC_INPUT_SINGLE_ENDED | 差分输入 |
| CODEC_INPUT_DIFFERENCE   | 单端输入 |

### csi_codec_input_attach_callback

```c
csi_error_t csi_codec_input_attach_callback(csi_codec_input_t *ch, void *callback, void *arg)
```

* 功能描述：
  * 设置输入通道回调函数。
* 参数：
  * `csi_codec_input_t`：输入通道实例句柄。
  * `callback：codec`:输入通道的事件回调函数（一般在上下文执行）。
  * `arg`:回调函数参数（可选，由用户定义）。
* 返回值：
  * 错误码csi_error_t。

#### callback

```c
void (*callback)(csi_codec_input_t *input, csi_codec_event_t event, void *arg)
```

其中 input为输入通道句柄，event 为传给回调函数的事件类型，arg 为用户自定义的回调函数对应的参数。 

codec 回调事件枚举类型csi_codec_event_t定义如下：

| 类型                              | 说明             |
| --------------------------------- | ---------------- |
| CODEC_EVENT_PERIOD_READ_COMPLETE  | 接收period完成   |
| CODEC_EVENT_PERIOD_WRITE_COMPLETE | 发送period完成   |
| CODEC_EVENT_WRITE_BUFFER_EMPTY    | 发送缓冲区已经空 |
| CODEC_EVENT_READ_BUFFER_FULL      | 接收缓冲区已经满 |
| CODEC_EVENT_TRANSFER_ERROR        | 传输错误         |

### csi_codec_input_detach_callback

```c
void csi_codec_input_detach_callback(csi_codec_input_t *ch)
```

* 功能描述：
  * 注销CODEC 输入通道的回调函数。
* 参数：
  * `ch`：通道实例句柄。
* 返回值：
  * 无。

### csi_codec_input_close

```c
void csi_codec_input_close(csi_codec_input_t *ch)
```

* 功能描述：
  * 关闭输入通道。
  * 调用该接口会马上停止接收数据。
* 参数：
  * `ch`：通道实例句柄。

### csi_codec_input_link_dma

```c
csi_error_t csi_codec_input_link_dma(csi_codec_input_t *ch, csi_dma_ch_t *dma)
```

*  功能描述：

  * 输入通道连接DMA。

* 参数：

  * `ch`：输入通道的实例句柄。
  * `dma`：dma实例句柄。

* 返回：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_input_t codec_input;
  csi_error_t ret;
  csi_dma_ch_t g_dma_ch;
  /* 为输入通道设置DMA通道 */
  ret = csi_codec_input_link_dma(&codec_input, &g_dma_ch);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_codec_input_send

```c
uint32_t csi_codec_input_send(csi_codec_input_t *ch, const void *data, uint32_t size)
```

*  功能描述：

  * 输入通道同步模式发送数据。

* 参数：

  * `ch`：输入通道的实例句柄。
  * `data`：发送数据指针。
  * `size`: 发送数据长度。

* 返回：

  * uint32_t 发送成功数据的长度。

* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_input_t codec_input;
  uint8_t write_data[1024];
  uint32_t num;
  num = csi_codec_input_send(&codec_input, write_data, sizeof(write_data));
  if (num != sizeof(write_data)) {
      return -1;
  }
  ```

  

### csi_codec_input_send_async

```c
uint32_t csi_codec_input_send_async(csi_codec_input_t *ch, const void *data, uint32_t size)
```

*  功能描述：

  * 输入通道异步模式发送数据。

* 参数：

  * `ch`：输入通道的实例句柄。
  * `data`：发送数据指针。
  * `size`: 发送数据长度。

* 返回：

  * uint32_t 发送成功数据的长度。

* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_input_t codec_input;
  uint8_t write_data[1024];
  uint32_t num;
  /*调用该接口后，数据会写到缓冲区，用户需要根据设定的period值来判断数据是否发送完毕*/
  num = csi_codec_input_send_async(&codec_input, write_data, sizeof(write_data));
  if (num != sizeof(write_data)) {
      return -1;
  }
  ```

### csi_codec_input_start

```c
csi_error_t csi_codec_input_start(csi_codec_input_t *ch)
```

* 功能描述：

  * 输入通道开始数据流。

* 参数：

  * `ch`：输入通道的实例句柄。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_input_t codec_input;
  csi_error_t ret;
  num = csi_codec_input_start(&codec_input);
  if (num != CSI_OK) {
      return -1;
  }
  ```

### csi_codec_input_stop

```c
csi_error_t csi_codec_input_stop(csi_codec_input_t *ch)
```

* 功能描述：

  * 输入通道结束数据流。

* 参数：

  * `ch`：输入通道的实例句柄。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_input_t codec_input;
  csi_error_t ret;
  ret = csi_codec_input_stop(&codec_input);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_codec_input_buffer_avail

```c
uint32_t csi_codec_input_buffer_avail(csi_codec_input_t *ch)
```

* 功能描述：

  * 返回当前缓冲区剩余可用数据数。

* 参数：

  * `ch`：输出通道的实例句柄。

* 返回值：

  * uint32_t 剩余数据数。

* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_input_t codec_input;
  uint32_t num;
  num = csi_codec_input_buffer_avail(&codec_input);
  ```

### csi_codec_input_buffer_remain

```c
uint32_t csi_codec_input_buffer_remain(csi_codec_input_t *ch)
```

* 功能描述：

  * 返回当前缓冲区已用数据数。

* 参数：

  * `ch`：输出通道的实例句柄。

* 返回值：

  * uint32_t 已用数据数。

* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_input_t codec_input;
  uint32_t num;
  num = csi_codec_input_buffer_remain(&codec_input);
  ```

### csi_codec_input_buffer_reset

```c
csi_error_t csi_codec_input_buffer_reset(csi_codec_input_t *ch)
```

* 功能描述：

  * 对缓冲区数据全部置0，重置ringbuffer。

* 参数：

  * `ch`：输出通道的实例句柄。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_input_t codec_input;
  csi_error_t ret;
  ret = csi_codec_input_buffer_reset(&codec_input);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_codec_input_mute

```c
csi_error_t csi_codec_input_mute(csi_codec_input_t *ch, bool en)
```

* 功能描述：

  * 设置输入通道静音。

* 参数：

  * `ch`：输入通道的实例句柄。
  * `en`: true 代表输入静音,  Flase 代表输入不静音。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_input_t codec_input;
  csi_error_t ret;
  ret = csi_codec_input_mute(&codec_input, true);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_codec_input_digital_gain

```c
csi_error_t csi_codec_input_digital_gain(csi_codec_input_t *ch, uint32_t val)
```

* 功能描述：

  * 设置输入通道数码增益。

* 参数：

  * `ch`：输入通道的实例句柄。
  * `val`: 增益的DB值。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_input_t codec_input;
  csi_error_t ret;
  /* 设置的数码增益 需要根据实际硬件支持的值进行设置*/
  ret = csi_codec_input_digital_gain(&codec_input, 0x20);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_codec_input_analog_gain

```c
csi_error_t csi_codec_input_analog_gain(csi_codec_input_t *ch, uint32_t val)
```

* 功能描述：

  * 设置输入通道模拟增益。

* 参数：

  * `ch`：输入通道的实例句柄。
  * `val`: 增益的DB值。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_input_t codec_input;
  csi_error_t ret;
  /* 设置的数码增益 需要根据实际硬件支持的值进行设置*/
  ret = csi_codec_input_analog_gain(&codec_input, 0x20);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_codec_input_mix_gain

```c
csi_error_t csi_codec_input_mix_gain(csi_codec_input_t *ch, uint32_t val)
```

* 功能描述：

  * 设置输入通道混频增益。

* 参数：

  * `ch`：输入通道的实例句柄。
  * `val`: 增益的DB值。

* 返回值：

  * 错误码csi_error_t。
  
* 使用示例：

  ```c
  /* 句柄使用前请先初始化 */
  static csi_codec_input_t codec_input;
  csi_error_t ret;
  /* 设置的混频增益 需要根据实际硬件支持的值进行设置*/
  ret = csi_codec_input_mix_gain(&codec_input, 0x20);
  if (ret != CSI_OK) {
      return -1;
  }
  ```

### csi_codec_input_get_state

```c
csi_error_t csi_codec_input_get_state(csi_codec_input_t *ch, csi_state_t *state)
```

* 功能描述：

  * 获取输出通道的状态。通过此函数来判断code通道在获取状态的时刻是否可以进行读写操作。

* 参数

  * `ch`：实例句柄。
  * `state`: 用于返回状态信息的参数地址。

* 返回值：

  * 错误码csi_error_t。
  

## 初始化及配置示例

```c
/* 句柄空间一般使用静态空间 */
static csi_codec_t codec;

static csi_codec_output_t codec_output_ch;
static csi_codec_input_t  codec_input_ch;

static csi_dma_ch_t dma_ch_output_handle;
static csi_dma_ch_t dma_ch_input_handle;	    

#define OUTPUT_BUF_SIZE 2048
uint8_t output_buf[OUTPUT_BUF_SIZE];

#define INPUT_BUF_SIZE  2048
uint8_t input_buf[INPUT_BUF_SIZE];

static ringbuffer_t output_ring_buffer; 
static ringbuffer_t input_ring_buffer;

volatile uint8_t cb_output_transfer_flag = 0;
volatile uint8_t cb_input_transfer_flag = 0;

static void codec_output_event_cb_fun(csi_codec_output_t *output, csi_codec_event_t event, void *arg)
{
    if (event == CODEC_EVENT_PERIOD_WRITE_COMPLETE) {
        cb_output_transfer_flag = 1;
    }
}

static void codec_input_event_cb_fun(csi_codec_input_t *i2s, csi_codec_event_t event, void *arg)
{
    if (event == CODEC_EVENT_PERIOD_READ_COMPLETE) {
        cb_input_transfer_flag = 1;
    }
}

/* 本示例 示例了codec 的输出输入流启动 */
int main(void) {
    csi_error_t ret;
    csi_codec_output_config_t output_config;
    csi_codec_input_config_t input_config;
    /* init函数的idx参数，请根据soc的实际情况进行选择 */
    ret = csi_codec_init(&codec, 0);;
    if (ret != CSI_OK) {
        return -1;
    }
    

	/* output ch config */
    csi_codec_output_attach_callback(&codec_output_ch, codec_output_event_cb_fun, NULL);

    codec_output_ch.period = OUTPUT_BUF_SIZE/2;
    codec_output_ch.ring_buf = &output_ring_buffer;
    csi_codec_output_open(&codec, &codec_output_ch, 0);

    output_config.bit_width = 16;
    output_config.sample_rate = 48000;
    output_config.buffer = output_buf;
    output_config.buffer_size = OUTPUT_BUF_SIZE;
    output_config.period = OUTPUT_BUF_SIZE/2;
    output_config.mode = CODEC_OUTPUT_DIFFERENCE;
    csi_codec_output_config(&codec_output_ch,&output_config);
    csi_codec_output_buffer_reset(&codec_output_ch);

    csi_codec_output_link_dma(&codec_output_ch,&dma_ch_output_handle);

    /* input ch config */
    csi_codec_input_attach_callback(&codec_input_ch, codec_input_event_cb_fun, NULL);

    codec_input_ch.period = INPUT_BUF_SIZE/2;
    codec_input_ch.ring_buf = &input_ring_buffer;
    csi_codec_input_open(&codec, &codec_input_ch, 0);

    input_config.bit_width = 16;
    input_config.sample_rate = 48000;
    input_config.buffer = input_buf;
    input_config.buffer_size = INPUT_BUF_SIZE;
    input_config.period = INPUT_BUF_SIZE/2;
    input_config.mode = CODEC_INPUT_DIFFERENCE;
    csi_codec_input_config(&codec_input_ch,&input_config);

    csi_codec_input_link_dma(&codec_input_ch,&dma_ch_input_handle);

    printf("start code input and output\n");
    csi_codec_input_start(&codec_input_ch);
    csi_codec_output_start(&codec_output_ch);
}
```

## 同步模式传输

> 注意：
>
> 无论是同步模式，异步模式都要求注册回调函数，DMA进行连接，启动发送流

```c
/* 句柄空间一般使用静态空间 */
static csi_codec_t codec;

static csi_codec_output_t codec_output_ch;
static csi_codec_input_t  codec_input_ch;

static csi_dma_ch_t dma_ch_output_handle;
static csi_dma_ch_t dma_ch_input_handle;	    

#define OUTPUT_BUF_SIZE 2048
uint8_t output_buf[OUTPUT_BUF_SIZE];

#define INPUT_BUF_SIZE  2048
uint8_t input_buf[INPUT_BUF_SIZE];

static ringbuffer_t output_ring_buffer; 
static ringbuffer_t input_ring_buffer;

volatile uint8_t cb_output_transfer_flag = 0;
volatile uint8_t cb_input_transfer_flag = 0;

static void codec_output_event_cb_fun(csi_codec_output_t *output, csi_codec_event_t event, void *arg)
{
    if (event == CODEC_EVENT_PERIOD_WRITE_COMPLETE) {
        cb_output_transfer_flag = 1;
    }
}

static void codec_input_event_cb_fun(csi_codec_input_t *i2s, csi_codec_event_t event, void *arg)
{
    if (event == CODEC_EVENT_PERIOD_READ_COMPLETE) {
        cb_input_transfer_flag = 1;
    }
}

/* 本示例 示例了codec 的同步发送和接收启动 */
int main(void) {
    uint8_t read_data[2048];
    uint8_t write_data[2048];
    csi_error_t ret;
    csi_codec_output_config_t output_config;
    csi_codec_input_config_t input_config;
    /* init函数的idx参数，请根据soc的实际情况进行选择 */
    ret = csi_codec_init(&codec, 0);;
    if (ret != CSI_OK) {
        return -1;
    }
	/* output ch config */
	csi_codec_output_attach_callback(&codec_output_ch, codec_output_event_cb_fun, NULL);

	codec_output_ch.period = OUTPUT_BUF_SIZE/2;
	codec_output_ch.ring_buf = &output_ring_buffer;
	csi_codec_output_open(&codec, &codec_output_ch, 0);

	output_config.bit_width = 16;
	output_config.sample_rate = 48000;
	output_config.buffer = output_buf;
	output_config.buffer_size = OUTPUT_BUF_SIZE;
	output_config.period = OUTPUT_BUF_SIZE/2;
	output_config.mode = CODEC_OUTPUT_DIFFERENCE;
	csi_codec_output_config(&codec_output_ch,&output_config);
	csi_codec_output_buffer_reset(&codec_output_ch);

	csi_codec_output_link_dma(&codec_output_ch,&dma_ch_output_handle);

	/* input ch config */
	csi_codec_input_attach_callback(&codec_input_ch, codec_input_event_cb_fun, NULL);

	codec_input_ch.period = INPUT_BUF_SIZE/2;
	codec_input_ch.ring_buf = &input_ring_buffer;
  csi_codec_input_open(&codec, &codec_input_ch, 0);

	input_config.bit_width = 16;
	input_config.sample_rate = 48000;
	input_config.buffer = input_buf;
	input_config.buffer_size = INPUT_BUF_SIZE;
	input_config.period = INPUT_BUF_SIZE/2;
	input_config.mode = CODEC_INPUT_DIFFERENCE;
	csi_codec_input_config(&codec_input_ch,&input_config);

	csi_codec_input_link_dma(&codec_input_ch,&dma_ch_input_handle);

	printf("start code input and output\n");
	csi_codec_input_start(&codec_input_ch);
	csi_codec_output_start(&codec_output_ch);
	uint32_t read_size = 0;
	read_size = csi_codec_input_receive(&codec_input_ch, read_data, 	sizeof(read_data)); 

	if (read_size == sizeof(read_data)) {
		printf("test_code_receive_sync successfully\n");
	} else {
		printf("test_code_receive_sync failed\n");
	} 
	uint32_t write_size = 0;
	write_size = csi_codec_output_send(&codec_output_ch, write_data, 	sizeof(write_data)); 

	if (write_size == sizeof(write_data)) {
		printf("test_code_send_sync successfully\n");
	} else {
		printf("test_code_send_sync failed\n");
	} 
	csi_codec_input_stop(&codec_input_ch);
	csi_codec_output_stop(&codec_output_ch);

	csi_codec_input_link_dma(&codec_input_ch,NULL);
	csi_codec_output_link_dma(&codec_output_ch,NULL);

	csi_codec_output_detach_callback(&codec_output_ch);
	csi_codec_input_detach_callback(&codec_input_ch);

	csi_codec_uninit(&codec);
}
```



## 异步模式传输

> 注意：
>
> 无论是同步模式，异步模式都要求注册回调函数，DMA进行连接，启动发送流

```c
/* 句柄空间一般使用静态空间 */
static csi_codec_t codec;

static csi_codec_output_t codec_output_ch;
static csi_codec_input_t  codec_input_ch;

static csi_dma_ch_t dma_ch_output_handle;
static csi_dma_ch_t dma_ch_input_handle;

#define OUTPUT_BUF_SIZE 2048
uint8_t output_buf[OUTPUT_BUF_SIZE];

#define INPUT_BUF_SIZE  2048
uint8_t input_buf[INPUT_BUF_SIZE];

static ringbuffer_t output_ring_buffer;
static ringbuffer_t input_ring_buffer;

volatile uint8_t cb_output_transfer_flag = 0;
volatile uint8_t cb_input_transfer_flag = 0;

static void codec_output_event_cb_fun(csi_codec_output_t *output, csi_codec_event_t event, void *arg)
{
    if (event == CODEC_EVENT_PERIOD_WRITE_COMPLETE) {
        cb_output_transfer_flag --;
    }
}

static void codec_input_event_cb_fun(csi_codec_input_t *i2s, csi_codec_event_t event, void *arg)
{
    if (event == CODEC_EVENT_PERIOD_READ_COMPLETE) {
        cb_input_transfer_flag = 1;
    }
}

/* 本示例 示例了codec 的异步步发送和接收启动 */
int main(void) {
    csi_error_t ret;
    csi_codec_output_config_t output_config;
    csi_codec_input_config_t input_config;
    /* init函数的idx参数，请根据soc的实际情况进行选择 */
    ret = csi_codec_init(&codec, 0);;
    if (ret != CSI_OK) {
        return -1;
    }

	/* output ch config */
	csi_codec_output_attach_callback(&codec_output_ch, codec_output_event_cb_fun, NULL);

	codec_output_ch.period = OUTPUT_BUF_SIZE/2;
	codec_output_ch.ring_buf = &output_ring_buffer;
	csi_codec_output_open(&codec, &codec_output_ch, 0);

	output_config.bit_width = 16;
	output_config.sample_rate = 48000;
	output_config.buffer = output_buf;
	output_config.buffer_size = OUTPUT_BUF_SIZE;
	output_config.period = OUTPUT_BUF_SIZE/2;
	output_config.mode = CODEC_OUTPUT_DIFFERENCE;
	csi_codec_output_config(&codec_output_ch,&output_config);
	csi_codec_output_buffer_reset(&codec_output_ch);

	csi_codec_output_link_dma(&codec_output_ch,&dma_ch_output_handle);

	/* input ch config */
	csi_codec_input_attach_callback(&codec_input_ch, codec_input_event_cb_fun, NULL);

	codec_input_ch.period = INPUT_BUF_SIZE/2;
	codec_input_ch.ring_buf = &input_ring_buffer;
	csi_codec_input_open(&codec, &codec_input_ch, 0);

	input_config.bit_width = 16;
	input_config.sample_rate = 48000;
	input_config.buffer = input_buf;
	input_config.buffer_size = INPUT_BUF_SIZE;
	input_config.period = INPUT_BUF_SIZE/2;
	input_config.mode = CODEC_INPUT_DIFFERENCE;
	csi_codec_input_config(&codec_input_ch,&input_config);

	csi_codec_input_link_dma(&codec_input_ch,&dma_ch_input_handle);

	printf("start code input and output\n");
	csi_codec_input_start(&codec_input_ch);
	csi_codec_output_start(&codec_output_ch);
  
    for (uint32_t i = 0; i < sizeof(write_data); i++) {
		write_data[i] = 0x5a;					
	}

	uint32_t write_size = 0;
	cb_output_transfer_flag = sizeof(write_data) / g_i2s->tx_period;///< 请确保发送数据是period的整数倍
	write_size = csi_codec_output_write_async(&codec_output_ch, write_data, sizeof(write_data)); ///<等待数据发送
	while(cb_output_transfer_flag != 0); ///<等待同步
	if (write_size == sizeof(write_data)) {
		printf("test_codec_send_async successfully\n");
	} else {
		printf("test_codec_send_async failed\n");
	} 
    
    uint32_t read_size = 0;
	uint32_t period_num = sizeof(read_data) / i2s_slave.rx_period;///< 请确保数据是period的整数倍
	cb_slave_transfer_flag = 0;
	while (1) {
		if (cb_slave_transfer_flag == 1) {
        	cb_slave_transfer_flag = 0;
        	read_size += csi_codec_input_receive_async(&codec_input_ch, (read_data + read_size), (sizeof(read_data) - read_size));///接收完注意：偏移接收指针
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
    
	csi_codec_input_stop(&codec_input_ch);
	csi_codec_output_stop(&codec_output_ch);

	csi_codec_input_link_dma(&codec_input_ch,NULL);
	csi_codec_output_link_dma(&codec_output_ch,NULL);

	csi_codec_output_detach_callback(&codec_output_ch);
	csi_codec_input_detach_callback(&codec_input_ch);

	csi_codec_uninit(&codec);
}
```

