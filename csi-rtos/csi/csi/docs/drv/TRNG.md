## 简要说明

RNG是一个硬件随机数生成器

## 接口描述

### csi_rng_get_multi_word
```c
csi_error_t csi_rng_get_multi_word(uint32_t *data, uint32_t num)
```

- 功能描述:

- 获取指定字节数的随机数。

- 参数:

- - `data`: 指向接收数据的缓存。
  - `num` : 需要接收的字节数。

- 返回值: 

  - CSI_OK: 获取成功。
  - CSI_ERROR: 获取失败。

### csi_rng_get_single_word
```c
csi_error_t csi_rng_get_single_word(uint32_t* data)
```

- 功能描述:

- 获取一个字的随机数。

- 参数:

- - `data`: 指向接收数据的缓存。

- 返回值: 

  - CSI_OK: 获取成功。
  - CSI_ERROR: 获取失败。
