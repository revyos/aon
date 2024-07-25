# AES

## 设备说明

AES可以实现AES（高级加密标准）编码算法，即Rijndael算法。AES支持三种不同长度的密钥，称为AES-128、AES-192和AES-256。此外，AES还支持ECB（电子码本）模式和CBC（密码块链）模式。

对于AES算法，输入块、输出块和状态的长度为128位。这由Nb=4表示，它反映了状态中32位字的数量。密钥K的长度是128、192或256位。密钥长度由Nk=4、6、8表示，它反映了密码密钥中32位字的数目。在算法执行期间要执行的轮数取决于密钥大小。轮数用Nr表示，其中Nr=10时，Nk=4；Nr=12时，Nk=6；Nr=14时，Nk=8。

## 接口列表

| 函数                    | 说明                   |
| ----------------------- | ---------------------- |
| csi_aes_init            | 初始化                 |
| csi_aes_uninit          | 去初始化               |
| csi_aes_set_encrypt_key | 设置加密密钥           |
| csi_aes_set_decrypt_key | 设置解密密钥           |
| csi_aes_ecb_encrypt     | 加密计算（ecb模式）    |
| csi_aes_ecb_decrypt     | 解密计算（ecb模式）    |
| csi_aes_cbc_encrypt     | 加密计算（cbc模式）    |
| csi_aes_cbc_decrypt     | 解密计算（cbc模式）    |
| csi_aes_cfb1_encrypt    | 加密计算（cfb1模式）   |
| csi_aes_cfb1_decrypt    | 解密计算（cfb1模式）   |
| csi_aes_cfb8_encrypt    | 加密计算（cfb8模式）   |
| csi_aes_cfb8_decrypt    | 解密计算（cfb8模式）   |
| csi_aes_cfb128_decrypt  | 加密计算（cfb128模式） |
| csi_aes_cfb128_encrypt  | 解密计算（cfb128模式） |
| csi_aes_ofb_encrypt     | 加密计算（ofb模式）    |
| csi_aes_ofb_decrypt     | 解密计算（ofb模式）    |
| csi_aes_ctr_encrypt     | 加密计算（ctr模式）    |
| csi_aes_ctr_decrypt     | 解密计算（ctr模式）    |

## 接口详细描述

### csi_aes_init

```c
csi_error_t csi_aes_init(csi_aes_t *aes, uint32_t idx)
```

* 功能描述:

  * 通过索引号初始化对应的aes实例，返回结果值。

* 参数:

  * `aes`: 设备句柄（需要用户申请句柄空间）。

  * `idx`: 控制器号。

* 返回值:

  * CSI_OK: 初始化成功。

  * CSI_ERROR: 初始化失败。

#### csi_aes_t

| 成员              | 类型      | 说明    |
| :------------- | :--------- | :--------- |
| dev | csi_dev_t | csi设备统一句柄     |
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

### csi_aes_uninit

```c
void csi_aes_uninit(csi_aes_t *aes)
```

* 功能描述:

  * aes实例反初始化，该接口会停止aes实例正在进行的工作（如果有），并且释放相关的软硬件资源。

* 参数:

  * `aes`: 实例句柄。

* 返回值:

  * 无。

------

### csi_aes_set_encrypt_key

```c
csi_error_t csi_aes_set_encrypt_key(csi_aes_t *aes, void *key, csi_aes_key_bits_t key_len)
```

* 功能描述:

  * 设置aes算法加密密钥。
  
* 参数:

  * `aes`: 实例句柄。

  * `key`: 密钥指针。

  * `key_len`: 密钥长度。

* 返回值:

  * CSI_OK: 配置成功。

  * CSI_ERROR: 配置失败。

---

#### csi_aes_key_bits_t

| 类型                 | 说明          |
| -------------------- | ------------- |
| AES_KEY_LEN_BITS_128 | 密钥长度128位 |
| AES_KEY_LEN_BITS_192 | 密钥长度192位 |
| AES_KEY_LEN_BITS_256 | 密钥长度256位 |

------

### csi_aes_set_decrypt_key

```c
csi_error_t csi_aes_set_decrypt_key(csi_aes_t *aes, void *key, csi_aes_key_bits_t key_len)
```

* 功能描述:

  * 设置aes算法解密密钥。

* 参数:

  * `aes`: 实例句柄。

  * `key`: 密钥指针。

  * `key_len`: 密钥长度。

* 返回值:

  * CSI_OK: 配置成功。

  * CSI_ERROR: 配置失败。

------

### csi_aes_ecb_encrypt

```c
csi_error_t csi_aes_ecb_encrypt(csi_aes_t *aes, void *in, void *out, uint32_t size)
```

* 功能描述:

  * ecb模式加密接口。

* 参数:

  * `aes`: 实例句柄。

  * `in`: 输入数据指针。

  * `out`: 输出数据指针。

  * `size`: 输入数据长度（16字节对齐）。
  
* 返回值:

  * CSI_OK: 加密成功。

  * CSI_ERROR: 加密失败。

------

### csi_aes_ecb_decrypt

```c
csi_error_t csi_aes_ecb_decrypt(csi_aes_t *aes, void *in, void *out, uint32_t size)
```

* 功能描述:

  * ecb模式解密接口。

* 参数:

  * `aes`: 实例句柄。

  * `in`: 输入数据指针。

  * `out`: 输出数据指针。

  * `size`: 输入数据长度（16字节对齐）。

* 返回值:

  * CSI_OK: 解密成功。

  * CSI_ERROR: 解密失败。

------

### csi_aes_cbc_encrypt

```c
csi_error_t csi_aes_cbc_encrypt(csi_aes_t *aes, void *in, void *out, uint32_t size, void *iv)
```

* 功能描述:

  * cbc模式加密接口。

* 参数:

  * `aes`: 实例句柄。

  * `in`: 输入数据指针。

  * `out`: 输出数据指针。

  * `size`: 输入数据长度（16字节对齐）。

  * `iv`：初始化向量

* 返回值:

  * CSI_OK: 加密成功。

  * CSI_ERROR: 加密失败。

------

### csi_aes_cbc_decrypt

```c
csi_error_t csi_aes_cbc_decrypt(csi_aes_t *aes, void *in, void *out, uint32_t size, void *iv)
```

* 功能描述:

  * cbc模式解密接口。

* 参数:

  * `aes`: 实例句柄。

  * `in`: 输入数据指针。

  * `out`: 输出数据指针。

  * `size`: 输入数据长度（16字节对齐）。

  * `iv`：初始化向量

* 返回值:

  * CSI_OK: 解密成功。

  * CSI_ERROR: 解密失败。

------

### csi_aes_cfb1_encrypt

```c
csi_error_t csi_aes_cfb1_encrypt(csi_aes_t *aes, void *in, void *out, uint32_t size, void *iv)
```

* 功能描述:

  * cfb1模式加密接口。

* 参数:

  * `aes`: 实例句柄。

  * `in`: 输入数据指针。

  * `out`: 输出数据指针。

  * `size`: 输入数据长度（16字节对齐）。

  * `iv`：初始化向量

* 返回值:

  * CSI_OK: 加密成功。

  * CSI_ERROR: 加密失败。

------

### csi_aes_cfb1_decrypt

```c
csi_error_t csi_aes_cfb1_decrypt(csi_aes_t *aes, void *in, void *out, uint32_t size, void *iv)
```

* 功能描述:

  * cfb1模式解密接口。

* 参数:

  * `aes`: 实例句柄。

  * `in`: 输入数据指针。

  * `out`: 输出数据指针。

  * `size`: 输入数据长度（16字节对齐）。

  * `iv`：初始化向量

* 返回值:

  * CSI_OK: 解密成功。
  
  * CSI_ERROR: 解密失败。

---

### csi_aes_cfb8_encrypt

```c
csi_error_t csi_aes_cfb8_encrypt(csi_aes_t *aes, void *in, void *out, uint32_t size, void *iv)
```

* 功能描述:

  * cfb8模式加密接口。

* 参数:

  * `aes`: 实例句柄。

  * `in`: 输入数据指针。

  * `out`: 输出数据指针。

  * `size`: 输入数据长度（16字节对齐）。

  * `iv`：初始化向量

* 返回值:

  * CSI_OK: 加密成功。

  * CSI_ERROR: 加密失败。

------

### csi_aes_cfb8_decrypt

```c
csi_error_t csi_aes_cfb8_decrypt(csi_aes_t *aes, void *in, void *out, uint32_t size, void *iv)
```

* 功能描述:

  * cfb8模式解密接口。

* 参数:

  * `aes`: 实例句柄。

  * `in`: 输入数据指针。

  * `out`: 输出数据指针。

  * `size`: 输入数据长度（16字节对齐）。

  * `iv`：初始化向量

* 返回值:

  * CSI_OK: 解密成功。

  * CSI_ERROR: 解密失败。

---

### csi_aes_cfb128_encrypt

```c
csi_error_t csi_aes_cfb128_encrypt(csi_aes_t *aes, void *in, void *out, uint32_t size, void *iv, uint32_t *num)
```

* 功能描述:

  * cfb128模式加密接口。

* 参数:

  * `aes`: 实例句柄。

  * `in`: 输入数据指针。
  
  * `out`: 输出数据指针。
  
  * `size`: 输入数据长度（16字节对齐）。
  
  * `iv`：初始化向量
  
  * `num`:  输出参数，计算了多少个cfb数据块 
  
* 返回值:

  * CSI_OK: 加密成功。

  * CSI_ERROR: 加密失败。

------

### csi_aes_cfb128_decrypt

```c
csi_error_t csi_aes_cfb128_decrypt(csi_aes_t *aes, void *in, void *out, uint32_t size, void *iv, uint32_t *num)
```

* 功能描述:

  * cfb128模式解密接口。

* 参数:

  * `aes`: 实例句柄。
  
  * `in`: 输入数据指针。
  
  * `out`: 输出数据指针。
  
  * `size`: 输入数据长度（16字节对齐）。
  
  * `iv`：初始化向量
  
  * `num`:  输出参数，计算了多少个cfb数据块 
  
* 返回值:

  * CSI_OK: 解密成功。

  * CSI_ERROR: 解密失败。

---

### csi_aes_ofb_encrypt

```c
csi_error_t csi_aes_ofb_encrypt(csi_aes_t *aes, void *in, void *out, uint32_t size, void *iv, uint32_t *num)
```

* 功能描述:

  * ofb模式加密接口。

* 参数:

  * `aes`: 实例句柄。
  
  * `in`: 输入数据指针。
  
  * `out`: 输出数据指针。
  
  * `size`: 输入数据长度（16字节对齐）。
  
  * `iv`：初始化向量

  * `num`:  输出参数，计算了多少个ofb数据块 
  
* 返回值:

  * CSI_OK: 加密成功。

  * CSI_ERROR: 加密失败。

------

### csi_aes_ofb_decrypt

```c
csi_error_t csi_aes_ofb_decrypt(csi_aes_t *aes, void *in, void *out, uint32_t size, void *iv, uint32_t *num)
```

* 功能描述:

  * ofb模式解密接口。

* 参数:

  * `aes`: 实例句柄。
  
  * `in`: 输入数据指针。
  
  * `out`: 输出数据指针。
  
  * `size`: 输入数据长度（16字节对齐）。
  
  * `iv`：初始化向量
  
  * `num`:  输出参数，计算了多少个ofb数据块 
  
* 返回值:

  * CSI_OK: 解密成功。

  * CSI_ERROR: 解密失败。

---

### csi_aes_ctr_encrypt

```c
csi_error_t csi_aes_ctr_encrypt(csi_aes_t *aes, void *in, void *out, uint32_t size, uint8_t nonce_counter[16], uint8_t stream_block[16], void *iv, uint32_t *num)
```

* 功能描述:

  * ctr模式加密接口。

* 参数:

  * `aes`: 实例句柄。

  * `in`: 输入数据指针。
  
  * `out`: 输出数据指针。

  * `size`: 输入数据长度（16字节对齐）。
  
  * `nonce_counter`：随机计数器128位
  
  * `stream_block`：流块
  
  * `iv`：初始化向量

  * `num`：输出参数，计算了多少个ctr数据块
  
* 返回值:

  * CSI_OK: 加密成功。

  * CSI_ERROR: 加密失败。

------

### csi_aes_ctr_decrypt

```c
csi_error_t csi_aes_ctr_decrypt(csi_aes_t *aes, void *in, void *out, uint32_t size, uint8_t nonce_counter[16], uint8_t stream_block[16], void *iv, uint32_t *num)
```

* 功能描述:

  * ctr模式解密接口。

* 参数:

  * `aes`: 实例句柄。

  * `in`: 输入数据指针。
  
  * `out`: 输出数据指针。
  
  * `size`: 输入数据长度（16字节对齐）。
  
  * `nonce_counter`：随机计数器128位
  
  * `stream_block`：流块
  
  * `iv`：初始化向量

  * `num`：输出参数，计算了多少个ctr数据块
  
* 返回值:

  * CSI_OK: 解密成功。

  * CSI_ERROR: 解密失败。

---

## 初始化示例

```C
#include <stdio.h>
#include <soc.h>
#include <drv/aes.h>

static csi_aes_t g_aes;

int main(void)
{
    int ret = 0;

    /* init aes 0 */
    ret = csi_aes_init(&g_aes, 0);
    
    return ret;
}
```

## 同步模式使用示例

### ECB-256模式

```c
#include <stdio.h>
#include <soc.h>
#include <drv/aes.h>

#define CHECK_RETURN(ret)                           \
        do {                                        \
            if (ret != 0) {                         \
                return -1;                          \
            }                                       \
        } while(0);

static csi_aes_t g_aes;

int main(void)
{
	int ret = 0;
    const uint8_t in[16] = "Hello, World!";
    const uint8_t key[32] = "Demo-Key";
    const uint8_t expect[16] =
        {0x7b, 0xab, 0x3a, 0xb2, 0xf3, 0x3c, 0xcf, 0xca, 
         0x20, 0x7a, 0xc9, 0x63, 0x7f, 0x02, 0x49, 0x55};
    uint8_t out[16];

    /* STEP 1: init aes */
    ret = csi_aes_init(&g_aes, 0);
    CHECK_RETURN(ret);

    /* STEP 2: config encrypt key */
    ret = csi_aes_set_encrypt_key(&g_aes, (void *)key, AES_KEY_LEN_BITS_256);
    CHECK_RETURN(ret);

    /* STEP 3: do encryption process */
    ret = csi_aes_ecb_encrypt(&g_aes, (void *)in, (void *)out, 16);
    CHECK_RETURN(ret);

    /* STEP 4: compare output result */
    ret = memcmp(out, expect, 16);
    CHECK_RETURN(ret);

    /* STEP 5: config decrypt key */
    ret = csi_aes_set_decrypt_key(&g_aes, (void *)key, AES_KEY_LEN_BITS_256);
    CHECK_RETURN(ret);

    /* STEP 6: do decryption process */
    ret = csi_aes_ecb_decrypt(&g_aes, (void *)expect, (void *)out, 16);
    CHECK_RETURN(ret);

    /* STEP 7: compare output result */
    ret = memcmp(out, in, 16);
    CHECK_RETURN(ret);

    return ret;
}
```

### CBC-256模式

```c
#include <stdio.h>
#include <soc.h>
#include <drv/aes.h>

#define CHECK_RETURN(ret)                           \
        do {                                        \
            if (ret != 0) {                         \
                return -1;                          \
            }                                       \
        } while(0);

static csi_aes_t g_aes;

int main(void)
{
	int ret = 0;
    const uint8_t in[16] = "Hello, World!";
    const uint8_t key[32] = "Demo-Key";
    const uint8_t iv[16] = {0};
    const uint8_t expect[16] =
        {0x7b, 0xab, 0x3a, 0xb2, 0xf3, 0x3c, 0xcf, 0xca, 
         0x20, 0x7a, 0xc9, 0x63, 0x7f, 0x02, 0x49, 0x55};
    uint8_t out[16];

    /* STEP 1: init aes */
    ret = csi_aes_init(&g_aes, 0);
    CHECK_RETURN(ret);

    /* STEP 2: config encrypt key */
    ret = csi_aes_set_encrypt_key(&g_aes, (void *)key, AES_KEY_LEN_BITS_256);
    CHECK_RETURN(ret);

    /* STEP 3: do encryption process */
    ret = csi_aes_cbc_encrypt(&g_aes, (void *)in, (void *)out, 16, (void *)iv);
    CHECK_RETURN(ret);

    /* STEP 4: compare output result */
    ret = memcmp(out, expect, 16);
    CHECK_RETURN(ret);

    /* STEP 5: clear init vector (!!!if not clear will cache last iv value) */
    memset((void *)iv, 0, 16);

    /* STEP 6: config decrypt key */
    ret = csi_aes_set_decrypt_key(&g_aes, (void *)key, AES_KEY_LEN_BITS_256);
    CHECK_RETURN(ret);

    /* STEP 7: do decryption process */
    ret = csi_aes_cbc_decrypt(&g_aes, (void *)expect, (void *)out, 16, (void *)iv);
    CHECK_RETURN(ret);

    /* STEP 8: compare output result */
    ret = memcmp(out, in, 16);
    CHECK_RETURN(ret);

    return ret;
}
```

