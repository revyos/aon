# RSA

## 设备说明

RSA公开密钥密码体制是一种使用不同的加密密钥与解密密钥，“由已知加密密钥推导出解密密钥在计算上是不可行的”密码体制。RSA公开密钥密码体制的原理是：根据数论，寻求两个大素数比较简单，而将它们的乘积进行因式分解却极其困难，因此可以将乘积公开作为加密密钥。

## 接口列表

| 函数                    | 说明         |
| ----------------------- | ------------ |
| csi_rsa_init            | 初始化       |
| csi_rsa_uninit          | 去初始化     |
| csi_rsa_attach_callback | 注册回调函数 |
| csi_rsa_detach_callback | 注销回调函数 |
| csi_rsa_gen_key         | 配置         |
| csi_rsa_encrypt         | 加密计算     |
| csi_rsa_decrypt         | 解密计算     |
| csi_rsa_sign            | 签名         |
| csi_rsa_verify          | 验签         |
| csi_rsa_encrypt_async   | 异步加密计算 |
| csi_rsa_decrypt_async   | 异步解密计算 |
| csi_rsa_sign_async      | 异步签名     |
| csi_rsa_verify_async    | 异步验签     |
| csi_rsa_get_state       | 获取状态     |
| csi_rsa_get_prime       | 获取大素数   |
| csi_rsa_enable_pm       | 使能电源管理 |
| csi_rsa_disable_pm      | 禁用电源管理 |

## 接口详细描述

### csi_rsa_init

```c
csi_error_t csi_rsa_init(csi_rsa_t *rsa, uint32_t idx)
```

* 功能描述:

  * 通过索引号初始化对应的rsa实例，返回结果值。

* 参数:

  * rsa: 设备句柄（需要用户申请句柄空间）。

  * `idx`: 控制器号。

* 返回值:

  * CSI_OK: 初始化成功。

  * CSI_ERROR: 初始化失败。

#### csi_rsa_t

| 成员              | 类型      | 说明    |
| :------------- | :--------- | :--------- |
| dev | csi_dev_t | csi设备统一句柄     |
| *cb   | void            | 回调函数         |
| *arg | void | 参数 |
| state | csi_rsa_state_t | 状态 |
| *priv | void | 对接接口句柄地址 |

#### csi_dev_t

| 成员              | 类型      | 说明    |
| :------------- | :--------- | :--------- |
| reg_base | uint32_t | 外设基址     |
| irq_num | uint8_t | 外设中断号     |
| idx | uint8_t | 外设ID      |
| irq_handler | 函数指针 | 外设中断函数地址 |

#### csi_error_t

| 类型 | 说明     |
| :------------- | :--------- |
| CSI_OK | 返回成功 |
| CSI_ERROR | 返回错误 |
| CSI_BUSY | 返回忙碌 |
| CSI_TIMEOUT | 返回超时 |
| CSI_UNSUPPORTED | 返回不支持 |

### csi_rsa_uninit

```c
void csi_rsa_uninit(csi_rsa_t *rsa)
```

* 功能描述:

  * rsa实例反初始化，该接口会停止rsa实例正在进行的工作（如果有），并且释放相关的软硬件资源。

* 参数:

  * rsa: 实例句柄。

* 返回值:

  * 无。

------

### csi_rsa_attach_callback

```c
csi_error_t csi_rsa_attach_callback(csi_rsa_t *rsa, csi_rsa_callback_t cb, void *arg)
```

* 功能描述:

  * 注册回调函数。

* 参数:

  * rsa: 实例句柄。

  * cb: 密钥指针。

  * arg: 密钥长度。

* 返回值:

  * CSI_OK: 配置成功。
* CSI_ERROR: 配置失败。

---

### csi_rsa_detach_callback

```c
void csi_rsa_detach_callback(csi_rsa_t *rsa)
```

* 功能描述:

  * 注销回调函数。

* 参数:

  * rsa: 实例句柄。

* 返回值:

  * 无


------

### csi_rsa_gen_key

```c
csi_error_t csi_rsa_gen_key(csi_rsa_t *rsa, csi_rsa_context_t *context)
```

* 功能描述:

  * 产生密码对。

* 参数:

  * `rsa`: 实例句柄。

  * context: 输入数据指针。

* 返回值:

  * CSI_OK: 加密成功。
* CSI_ERROR: 加密失败。

#### csi_rsa_context_t

| 成员         | 类型                   | 说明 |
| ------------ | ---------------------- | ---- |
| n            | void *                 |      |
| e            | void *                 |      |
| d            | void *                 |      |
| key_bits     | csi_rsa_key_bits_t     |      |
| padding_type | csi_rsa_padding_type_t |      |

#### csi_rsa_key_bits_t

| 类型              | 说明         |
| ----------------- | ------------ |
| RSA_KEY_BITS_192  | 加密密码位数 |
| RSA_KEY_BITS_256  | 加密密码位数 |
| RSA_KEY_BITS_512  | 加密密码位数 |
| RSA_KEY_BITS_1024 | 加密密码位数 |
| RSA_KEY_BITS_2048 | 加密密码位数 |
| RSA_KEY_BITS_3072 | 加密密码位数 |
| RSA_KEY_BITS_4096 | 加密密码位数 |

#### csi_rsa_padding_type_t

| 类型                         | 说明                |
| ---------------------------- | ------------------- |
| RSA_PADDING_MODE_NO          | 无填充模式          |
| RSA_PADDING_MODE_PKCS1       | PKCS1 填充模式      |
| RSA_PADDING_MODE__PKCS1_OAEP | PKCS1  OAEP填充模式 |
| RSA_PADDING_MODE_SSLV23      | SSLV23填充模式      |
| RSA_PADDING_MODE_X931        | X931填充模式        |
| RSA_PADDING_MODE_PSS         | PSS 填充模式        |



------

### csi_rsa_encrypt

```c
csi_error_t csi_rsa_encrypt(csi_rsa_t *rsa, csi_rsa_context_t *context, void *src, uint32_t src_size, void *out)
```

* 功能描述:

  * 加密接口。

* 参数:

  * `rsa`: 实例句柄。
* context:加密参数。
  * src: 输入数据指针。
* src_size: 输入数据长度（16字节对齐）。
  * out:输出数据指针。

* 返回值:

  * CSI_OK: 加密成功。

  * CSI_ERROR: 加密失败。

------

### csi_rsa_decrypt

```c
csi_error_t csi_rsa_decrypt(csi_rsa_t *rsa, csi_rsa_context_t *context, void *src, uint32_t src_size, void *out, uint32_t *out_size);
```

* 功能描述:

  * cbc模式加密接口。

* 参数:

  * `rsa`: 实例句柄。
* context：解密参数
  * src: 输入数据指针。
* `out`: 输出数据指针。
  * src_size: 输入数据长度。
* out_size：输出数据长度指针

* 返回值:

  * CSI_OK: 解密成功。

  * CSI_ERROR: 解密失败。

------

### csi_rsa_sign

```c
csi_error_t csi_rsa_sign(csi_rsa_t *rsa, csi_rsa_context_t *context, void *src, uint32_t src_size, void *signature, csi_rsa_hash_type_t hash_type);
```

* 功能描述:

  * cbc模式解密接口。

* 参数:

  * `rsa`: 实例句柄。
* context：解密参数
  * src: 输入数据指针。
* src_size: 输入数据长度。
  * signature：输出数据长度指针
* hash_type：哈希类型

* 返回值:

  * CSI_OK: 签名成功。
* CSI_ERROR: 签名失败。

#### csi_rsa_hash_type_t

| 类型                 | 说明           |
| -------------------- | -------------- |
| RSA_HASH_TYPE_MD5    | 哈希类型MD5    |
| RSA_HASH_TYPE_SHA1   | 哈希类型SHA1   |
| RSA_HASH_TYPE_SHA224 | 哈希类型SHA224 |
| RSA_HASH_TYPE_SHA256 | 哈希类型SHA256 |
| RSA_HASH_TYPE_SHA384 | 哈希类型SHA384 |
| RSA_HASH_TYPE_SHA512 | 哈希类型SHA512 |



------

### csi_rsa_verify

```c
bool csi_rsa_verify(csi_rsa_t *rsa, csi_rsa_context_t *context, void *src, uint32_t src_size, void *signature, uint32_t sig_size, csi_rsa_hash_type_t hash_type)
```

* 功能描述:

  * 验签功能。

* 参数:

  * `rsa`: 实例句柄。
* context：解密参数
  * src: 输入数据指针。
* src_size: 输入数据长度。
  * signature：输出数据长度指针
* sig_size:签名长度
  * hash_type：哈希类型

* 返回值:

  * TRUE: 验签成功。

  * FALSE: 加密失败。

------

### csi_rsa_encrypt_async

```c
csi_error_t csi_rsa_encrypt_async(csi_rsa_t *rsa, csi_rsa_context_t *context, void *src, uint32_t src_size, void *out)
```

* 功能描述:

  * 加密接口。
* 参数:

  * `rsa`: 实例句柄。
  * context:加密参数。
  * src: 输入数据指针。
  * src_size: 输入数据长度（16字节对齐）。
  * out:输出数据指针。

* 返回值:

  * CSI_OK: 执行成功。
  * CSI_ERROR:执行失败。。

---

### csi_rsa_decrypt_async

```c
csi_error_t csi_rsa_decrypt_async(csi_rsa_t *rsa, csi_rsa_context_t *context, void *src, uint32_t src_size, void *out, uint32_t *out_size)
```

* 功能描述:

  * cbc模式加密接口。
* 参数:

  * `rsa`: 实例句柄。
  * context：解密参数
  * src: 输入数据指针。
  * `out`: 输出数据指针。
  * src_size: 输入数据长度。
  * out_size：输出数据长度指针

* 返回值:

  * CSI_OK: 执行成功。
  * CSI_ERROR:执行失败。

------

### csi_rsa_sign_async

```c
csi_error_t csi_rsa_sign_async(csi_rsa_t *rsa, csi_rsa_context_t *context, void *src, uint32_t src_size, void *signature, csi_rsa_hash_type_t hash_type)
```

* 功能描述:

  * cbc模式解密接口。
* 参数:

  * `rsa`: 实例句柄。
  * context：解密参数
  * src: 输入数据指针。
  * src_size: 输入数据长度。
  * signature：输出数据长度指针
  * hash_type：哈希类型

* 返回值:

  * CSI_OK: 执行成功。
  * CSI_ERROR:执行失败。

---

### csi_rsa_verify_async

```c
csi_error_t csi_rsa_verify_async(csi_rsa_t *rsa, csi_rsa_context_t *context, void *src, uint32_t src_size, void *signature, uint32_t sig_size, csi_rsa_hash_type_t hash_type)
```

* 功能描述:

  * 验签功能。
* 参数:

  * `rsa`: 实例句柄。
  * context：解密参数
  * src: 输入数据指针。
  * src_size: 输入数据长度。
  * signature：输出数据长度指针
  * sig_size:签名长度
  * hash_type：哈希类型

* 返回值:

  * CSI_OK: 执行成功。
  * CSI_ERROR:执行失败。

------

### csi_rsa_get_state

```c
csi_error_t csi_rsa_get_state(csi_rsa_t *rsa, csi_rsa_state_t *state)
```

* 功能描述:

  * 获取rsa模块状态。

* 参数:

  * `rsa`: 实例句柄。

  * csi_rsa_state_t: rsa 状态指针。

* 返回值:

  * CSI_OK: 获取成功。
* CSI_ERROR:  获取失败。

#### csi_rsa_state_t

| 成员  | 类型    | 说明      |
| ----- | ------- | --------- |
| busy  | uint8_t | busy 状态 |
| error | uint8_t | 错误状态  |



---

### csi_rsa_get_prime

```c
csi_error_t csi_rsa_get_prime(csi_rsa_t *rsa, void *p, uint32_t bit_length)
```

* 功能描述:

  * 获取最大素数。

* 参数:

  * `rsa`: 实例句柄。

  * p: 素数指针。

  * bit_length: 素数长度。

* 返回值:

  * CSI_OK: 获取成功。

  * CSI_ERROR: 获取失败。



---

## 初始化示例

```C
#include <stdio.h>
#include <soc.h>
#include <drv/rsa.h>

static csi_rsa_t g_rsa;

int main(void)
{
    int ret = 0;

    /* init rsa 0 */
    ret = csi_rsa_init(&g_rsa, 0);

    return ret;
}
```

## 同步模式使用示例

### 加解密及验签

```c
#include <stdio.h>
#include <string.h>
#include "soc.h"
#include <drv/rsa.h>
#include <board_config.h>
#include <board_init.h>

static csi_rsa_t rsa = {0};

uint8_t modulus[] = {
    0xdf, 0x83, 0xe4, 0x76, 0x2d, 0x00, 0x61, 0xf6, 0xd0, 0x8d, 0x4a, 0x04, 0x66, 0xb1, 0xd5, 0x55,
    0xef, 0x71, 0xb5, 0xa5, 0x4e, 0x69, 0x44, 0xd3, 0x4f, 0xb8, 0x3d, 0xec, 0xb1, 0x1d, 0x5f, 0x82,
    0x6a, 0x48, 0x21, 0x00, 0x7f, 0xd7, 0xd5, 0xf6, 0x82, 0x35, 0xc2, 0xa6, 0x67, 0xa3, 0x53, 0x2d,
    0x3a, 0x83, 0x9a, 0xba, 0x60, 0xc2, 0x11, 0x22, 0xc2, 0x35, 0x83, 0xe9, 0x10, 0xa1, 0xb4, 0xa6,
    0x74, 0x57, 0x99, 0xd3, 0xa8, 0x6a, 0x21, 0x83, 0x76, 0xc1, 0x67, 0xde, 0xd8, 0xec, 0xdf, 0xf7,
    0xc0, 0x1b, 0xf6, 0xfa, 0x14, 0xa4, 0x0a, 0xec, 0xd1, 0xee, 0xc0, 0x76, 0x4c, 0xcd, 0x4a, 0x0a,
    0x5c, 0x96, 0xf2, 0xc9, 0xa4, 0x67, 0x03, 0x97, 0x2e, 0x17, 0xcd, 0xa9, 0x27, 0x9d, 0xa6, 0x35,
    0x5f, 0x7d, 0xb1, 0x6b, 0x68, 0x0e, 0x99, 0xc7, 0xdd, 0x5d, 0x6f, 0x15, 0xce, 0x8e, 0x85, 0x33
};
static const uint8_t publicExponent[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01
};
static const uint8_t privateExponent[] = {
    0xc6, 0x15, 0x3d, 0x02, 0xfe, 0x1e, 0xb8, 0xb2, 0xe3, 0x60, 0x53, 0x98, 0x52, 0xea, 0x87, 0x06,
    0x01, 0x8d, 0xe4, 0x4c, 0xfb, 0x90, 0x8f, 0x4e, 0x35, 0xf8, 0x31, 0xe8, 0xf1, 0x8d, 0xf6, 0x76,
    0xbd, 0x79, 0xee, 0xc5, 0x62, 0x87, 0x05, 0x37, 0xd1, 0x6d, 0x93, 0x73, 0xa5, 0xa5, 0x38, 0xb1,
    0x7c, 0x89, 0xe5, 0x36, 0x07, 0x49, 0xf5, 0xa5, 0xb8, 0x37, 0x75, 0x0f, 0xb7, 0x8d, 0x97, 0x69,
    0xc4, 0xd4, 0x8a, 0xb7, 0xfe, 0x74, 0x48, 0x45, 0x58, 0x47, 0x29, 0xa3, 0x0b, 0xa7, 0xdc, 0x55,
    0x98, 0x18, 0x8c, 0xd4, 0x52, 0xf5, 0xc9, 0xe8, 0x40, 0xce, 0x97, 0x46, 0x14, 0x1f, 0x62, 0x94,
    0xc3, 0x21, 0x1e, 0x5d, 0x49, 0x59, 0x31, 0xeb, 0xc4, 0x95, 0xf9, 0x33, 0x70, 0xa7, 0x90, 0xc3,
    0x9e, 0x98, 0x58, 0xa4, 0x00, 0xa4, 0x0f, 0xf3, 0x51, 0x80, 0xc6, 0x14, 0xfb, 0xd5, 0x5b, 0x01
};

uint8_t Digest_signature_pkcs1_padding_out[] = {
    0x07, 0x2d, 0x25, 0xde, 0xa5, 0xfd, 0x7c, 0xb0, 0x92, 0xb4, 0xee, 0x57, 0xe8, 0xd3, 0x79, 0x74,
    0x59, 0x25, 0x34, 0xef, 0xfd, 0x2b, 0xda, 0x8b, 0xa4, 0x40, 0x4e, 0xd8, 0x92, 0x6e, 0xee, 0x84,
    0x52, 0xb0, 0xe1, 0x0e, 0xa8, 0xa9, 0x68, 0x62, 0x1b, 0x51, 0xed, 0x50, 0x84, 0x98, 0x6a, 0x97,
    0x98, 0xe8, 0xcf, 0x3f, 0x85, 0xd3, 0x28, 0x26, 0xf3, 0x7a, 0x52, 0x4b, 0x04, 0x95, 0xe6, 0xfd,
    0xfa, 0x41, 0xf3, 0xac, 0x8a, 0x6d, 0x74, 0x91, 0x8c, 0x87, 0x52, 0x38, 0x08, 0x49, 0xf4, 0x60,
    0xcd, 0x4b, 0x1a, 0x9e, 0x52, 0x60, 0xf2, 0x73, 0x60, 0x31, 0x78, 0x37, 0xd9, 0x42, 0xc4, 0x61,
    0x43, 0xcf, 0x6d, 0x55, 0xee, 0x05, 0x19, 0xb7, 0xc3, 0x37, 0xa7, 0xa8, 0xa4, 0xbd, 0xf1, 0xac,
    0x8e, 0x39, 0x20, 0x59, 0xcd, 0xfc, 0x50, 0x16, 0x81, 0x2d, 0xeb, 0xba, 0x95, 0xe9, 0x38, 0xa5,
};

static const uint8_t Digest[] = {
    0xe4, 0x2c, 0x9f, 0x12, 0xf7, 0xd2, 0x67, 0x3a, 0x23, 0xea, 0x85, 0x61, 0xeb, 0xb2, 0xc2, 0x19,
    0xdc, 0xd9, 0xf1, 0xaa
};

uint8_t dst[1024 >> 3] = {0};
uint32_t dst_size = 0;
uint32_t async_verify_flag;
static void rsa_encrypt_decrypt()
{
    int ret;
    uint8_t i;
    int index;
    csi_rsa_context_t context;

    ///<csi_rsa_hash_type_t hash_type = RSA_HASH_TYPE_SHA1;
    csi_rsa_state_t status;

    index = 1;

    for (i = 0; i < index; i++) {
        csi_rsa_init(&rsa, 0);
        context.padding_type = RSA_PADDING_MODE_PKCS1;
        context.key_bits = RSA_KEY_BITS_1024;
        context.n = (void *)modulus;
        context.e = (void *)publicExponent;

        ret = csi_rsa_encrypt(&rsa, &context, (void *)Digest, 20, (void *)dst);
        dst_size = 128;

        while (1) {
            csi_rsa_get_state(&rsa, &status);

            if (status.busy == 0) {
                break;
            }
        }

        context.n = (void *)modulus;
        context.d = (void *)privateExponent;

        ret = csi_rsa_decrypt(&rsa, &context, (void *)dst, dst_size, (void *)dst, (uint32_t *)&dst_size);

        while (1) {
            csi_rsa_get_state(&rsa, &status);

            if (status.busy == 0) {
                break;
            }
        }

        if (ret == 0) {
            printf("&rsa encrypt and decrypt: %s\n", memcmp(dst, Digest, 20) ? "Fail" : "Pass");
        }

        csi_rsa_uninit(&rsa);
    }

    printf("test &rsa successfully\n");
}


static void rsa_sign_verify()
{
    uint8_t i;
    uint8_t result;
    int index;
    csi_rsa_context_t context;
    csi_rsa_hash_type_t hash_type = RSA_HASH_TYPE_SHA1;
    csi_rsa_state_t status;

    index = 1;

    for (i = 0; i < index; i++) {
        //&rsa = csi_rsa_initialize(i, rsa_crypto);
        csi_rsa_init(&rsa, 0);
        csi_rsa_detach_callback(&rsa);
        // csi_rsa_attach_callback(&rsa,rsa_example_cb,NULL);
        context.padding_type = RSA_PADDING_MODE_PKCS1;
        context.key_bits = RSA_KEY_BITS_1024;

        context.n = (void *)modulus;
        context.d = (void *)privateExponent;
        csi_rsa_sign(&rsa, &context, (void *)Digest, 20, (void *)dst, hash_type);
        dst_size = 128;

        while (1) {
            csi_rsa_get_state(&rsa, &status);

            if (status.busy == 0) {
                break;
            }
        }

        printf("&rsa sign: %s\n", memcmp(dst, Digest_signature_pkcs1_padding_out, dst_size) ? "Fail" : "Pass");
        context.n = (void *)modulus;
        context.e = (void *)publicExponent;
        result = csi_rsa_verify(&rsa, &context, (void *)Digest, 20, (void *)dst, dst_size,  hash_type);

        while (1) {
            csi_rsa_get_state(&rsa, &status);

            if (status.busy == 0) {
                break;
            }
        }

        printf("&rsa verify: %s\n", result ? "Pass" : "Fail");

        csi_rsa_uninit(&rsa);
    }
}


/*****************************************************************************
test_rsa: main function of the &rsa test

INPUT: NULL

RETURN: NULL

*****************************************************************************/
int example_rsa(void)
{
    rsa_sign_verify();
    rsa_encrypt_decrypt();
    return 0;
}

int main(void)
{
    board_init();
    printf("hello\n");
    return example_rsa();
}

```





