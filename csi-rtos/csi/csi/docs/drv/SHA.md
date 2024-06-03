# SHA

## 设备说明

​		安全哈希算法（Secure Hash Algorithm）主要适用于数字签名标准（Digital Signature Standard DSS）里面定义的数字签名算法（Digital Signature Algorithm DSA）。对于长度小于2^64位的消息，SHA1会产生一个160位的消息摘要。 

​		SHA-1是一种数据加密算法，该算法的思想是接收一段明文，然后以一种不可逆的方式将它转换成一段（通常更小）密文，也可以简单的理解为取一串输入码（称为预映射或信息），并把它们转化为长度较短、位数固定的输出序列即散列值（也称为信息摘要或信息认证代码）的过程。 

​		单向散列函数的安全性在于其产生散列值的操作过程具有较强的单向性。如果在输入序列中嵌入密码，那么任何人在不知道密码的情况下都不能产生正确的散列值，从而保证了其安全性。SHA-1将输入流按照每块512位（64个字节）进行分块，并产生20个字节的被称为信息认证代码或信息摘要的输出。

## 接口列表

SHA的CSI接口如下所示：

| 函数                    | 说明             |
| ----------------------- | ---------------- |
| csi_sha_init            | 初始化           |
| csi_sha_uninit          | 去初始化         |
| csi_sha_attach_callback | 注册回调函数     |
| csi_sha_detach_callback | 注销回调函数     |
| csi_sha_start           | 开始计算         |
| csi_sha_update          | 积累计算         |
| csi_sha_update_async    | 积累计算（异步） |
| csi_sha_finish          | 结束计算         |
| csi_sha_get_state       | 获取状态         |


## 接口详细说明

### csi_sha_init



#### csi_sha_t

| 成员                                                         | 类型            | 说明             |
| :----------------------------------------------------------- | :-------------- | :--------------- |
| dev                                                          | csi_dev_t       | csi设备统一句柄  |
| (*callback)(csi_sha_t *sha, csi_sha_event_t event, void *arg) | void            | 用户回调函数     |
| *arg                                                         | void            | 回调函数对应传参 |
| *dma_in                                                      | csi_dma_ch_t    | DMA通道句柄      |
| state                                                        | csi_sha_state_t | sha状态读取句柄  |
| *priv                                                        | void            | 对接接口句柄地址 |

#### csi_dev_t

| 成员        | 类型     | 说明             |
| :---------- | :------- | :--------------- |
| reg_base    | uint32_t | 外设基址         |
| irq_num     | uint8_t  | 外设中断号       |
| idx         | uint8_t  | 外设ID           |
| irq_handler | 函数指针 | 外设中断函数地址 |

#### csi_error_t

| 类型            | 说明       |
| :-------------- | :--------- |
| CSI_OK          | 返回成功   |
| CSI_ERROR       | 返回错误   |
| CSI_BUSY        | 返回忙碌   |
| CSI_TIMEOUT     | 返回超时   |
| CSI_UNSUPPORTED | 返回不支持 |

### csi_sha_attach_callback

```c
csi_error_t csi_sha_attach_callback(csi_sha_t *sha, void *callback, void *arg)
```

* 功能描述:

  * 注册回调函数。

* 参数:

  * `sha`: 实例句柄。
  * `callback`: sha实例的事件回调函数（一般在中断上下文执行）。
  * `arg`: 回调函数参数（可选，由用户定义）。

* 返回值:

  * CSI_OK: 执行成功。
  * CSI_ERROR: 执行失败。

#### callback

```c
void (*callback)(csi_sha_t *sha, csi_sha_event_t event, void *arg)
```

其中sha为设备句柄，event 为传给回调函数的事件类型，arg为用户自定义的回调函数对应的参数。sha 回调事件枚举类型csi_sha_event_t定义如下：

| 事件类型           | 事件说明               |
| ------------------ | ---------------------- |
| SHA_EVENT_COMPLETE | 完成加密，等待数据读出 |
| SHA_EVENT_ERROR    | 错误事件               |

------

### csi_sha_detach_callback

```c
void csi_sha_detach_callback(csi_sha_t *sha);
```

* 功能描述:

  * 注销回调函数。
* 参数:

  * `sha`: 实例句柄。
* 返回值:

  * 无

---

### csi_sha_uninit

```c
void csi_sha_uninit(csi_sha_t *sha)
```

* 功能描述:

  * sha实例反初始化，该接口会停止sha实例正在进行的工作（如果有），并且释放相关的软硬件资源。

* 参数:

  * `sha`: 实例句柄。

* 返回值:

  * CSI_OK: 执行成功。
  * CSI_ERROR: 执行失败。

------

### csi_sha_start

```c
csi_error_t csi_sha_start(csi_sha_t *sha, csi_sha_context_t *context)
```

* 功能描述:
  * 配置sha工作模式，设置数据源寄存器和数据目的寄存器；
* 参数:
  * `sha` : 实例句柄，定义见csi_sha_t
  * `context`：sha的上下文现场，用于支持多线程功能
  * `mode`：sha 的模式
* 返回值:
  * CSI_OK: 执行成功。
  * CSI_ERROR: 执行失败。

---

#### csi_sha_context_t

| 成员       | 类型           | 说明                                     |
| ---------- | -------------- | ---------------------------------------- |
| mode       | csi_sha_mode_t | sha工作模式                              |
| total[2]   | uint32_t       | 缓存已经计算的明文字节数                 |
| state[16]  | uint32_t       | 缓存消息摘要中间状态                     |
| buffer[32] | uint32_t       | 缓存最后一包未计算的原始数据（不足一包） |

#### csi_sha_mode_t

| 类型         | 说明                              |
| ------------ | --------------------------------- |
| SHA_MODE_1   | SHA-1版本（消息摘要长度160bit）   |
| SHA_MODE_256 | SHA-256版本（消息摘要长度256bit） |
| SHA_MODE_224 | SHA-224版本（消息摘要长度224bit） |
| SHA_MODE_512 | SHA-512版本（消息摘要长度512bit） |
| SHA_MODE_384 | SHA-384版本（消息摘要长度384bit） |

------

###  csi_sha_update 

```c
csi_error_t csi_sha_update(csi_sha_t *sha, csi_sha_context_t *context, const void *input, uint32_t size)
```

* 功能描述:

  * 根据明文信息持续计算整数倍的明文包，并将最后一组（不足一包明文）缓存到context等待最后finish操作。

* 参数:

  * `sha` : 实例句柄，定义见csi_sha_t
  * `context`：sha的上下文现场，用于支持多线程功能
  * `input`：输入明文
  * `size`：明文长度

* 返回值:

  * CSI_OK: 执行成功。
  * CSI_ERROR: 执行失败。

---

### csi_sha_update_async

```c
csi_error_t csi_sha_update_async(csi_sha_t *sha, csi_sha_context_t *context, const void *input, uint32_t size)
```

* 功能描述:

  * （异步方式）根据明文信息持续计算整数倍的明文包，并将最后一组（不足一包明文）缓存到context等待最后finish操作。

* 参数:

  * `sha` : 实例句柄，定义见csi_sha_t
  * `context`：sha的上下文现场，用于支持多线程功能
  * `output`：输出结果
  * `out_size`: 输出结果大小

* 返回值:

  * CSI_OK: 执行成功。
  * CSI_ERROR:执行失败。

------

### csi_sha_finish

```c
csi_error_t csi_sha_finish(csi_sha_t *sha, csi_sha_context_t *context, void *output, uint32_t *out_size)
```

* 功能描述:

  * 配合csi_sha_update接口进行长明文操作后，最后填充数据包并计算。
* 参数:

  * `sha` : 实例句柄，定义见csi_sha_t
  * `context`：sha的上下文现场，用于支持多线程功能
  * `output`：输出结果
  * `out_size`: 输出结果大小（单位：字节）
* 返回值:

  * CSI_OK: 执行成功。

  * CSI_ERROR: 执行失败。

------

### csi_sha_get_state

```c
csi_sha_state_t csi_sha_get_state(csi_sha_t *sha， csi_sha_state_t *state)
```

* 功能描述:

  * 获取sha当前工作状态。

* 参数:

  * `sha`: 实例句柄，定义见csi_sha_t。
  * `state`: sha工作状态

* 返回值:

  * CSI_OK:执行成功。
  * CSI_ERROR: 执行失败。

---

#### csi_sha_state_t

| 成员  | 类型     | 说明            |
| ----- | -------- | --------------- |
| busy  | uint32_t | bit0:忙状态位   |
| error | uint32_t | bit1:异常状态位 |

------

## 初始化示例

```c
#include <stdio.h>
#include <soc.h>
#include <drv/sha.h>

static csi_sha_t g_sha;

int main(void)
{
    int ret = 0;

    /* init sha 0 */
    ret = csi_sha_init(&g_sha, 0);
    
    return ret;
}
```

## 同步接口使用示例

### SHA1-单次加密示例

```c
#include <stdio.h>
#include <soc.h>
#include <drv/sha.h>

#define SHA_MESSAGE          ("abc")

#define CHECK_RETURN(ret)                           \
        do {                                        \
            if (ret != 0) {                         \
                return -1;                          \
            }                                       \
        } while(0);
        
static csi_sha_t g_sha;
static csi_sha_context_t   g_sha_context;

int main(void)
{
    int ret = 0;
    uint8_t out[64];            ///< calculation result of sha
    uint8_t in[1024];           ///< message of sha prepare calculation
    uint32_t out_size = 0;		///< bytes of calculation result
    const uint8_t expect[20] = {0xa9, 0x99, 0x3e, 0x36, 0x47, 0x06, 0x81, 0x6a, 0xba, 0x3e, 0x25, 0x71, 0x78, 0x50, 0xc2, 0x6c, 0x9c, 0xd0, 0xd8, 0x9d};
    
    /* STEP 1: init sha */
	ret = csi_sha_init(&g_sha, 0);
    CHECK_RETURN(ret);
    
    /* STEP 2: clean context cache */
    memset((uint8_t *)g_sha_context.total, 0, sizeof(csi_sha_context_t));
    
    /* STEP 3: config sha-1*/
    ret = csi_sha_start(&g_sha, &g_sha_context, SHA_MODE_1);
    CHECK_RETURN(ret);
    
    /* STEP 4: clean output buff */
    memset(out, 0, sizeof(out));
    
    /* STEP 5: first encrypt characters(abc) */
    memcpy(in, SHA_MESSAGE, strlen(SHA_MESSAGE));
    ret = csi_sha_update(&g_sha, &g_sha_context, in, strlen(SHA_MESSAGE));
    CHECK_RETURN(ret);
    
    /* STEP 6: finish encrypt and output result */
    ret = csi_sha_finish(&g_sha, &g_sha_context, out, &out_size);
    CHECK_RETURN(ret);
    
    /* STEP 7: verification output results */
    ret = memcmp(out, expect, out_size);
    CHECK_RETURN(ret);
    
    return ret;
}
```

### SHA1-累计加密示例

```c
#include <stdio.h>
#include <soc.h>
#include <drv/sha.h>

#define SHA_MESSAGE_1          ("a")
#define SHA_MESSAGE_2          ("b")
#define SHA_MESSAGE_3          ("c")

#define CHECK_RETURN(ret)                           \
        do {                                        \
            if (ret != 0) {                         \
                return -1;                          \
            }                                       \
        } while(0);
        
static csi_sha_t g_sha;
static csi_sha_context_t   g_sha_context;

int main(void)
{
    int ret = 0;
    uint8_t out[64];            ///< calculation result of sha
    uint8_t in[1024];           ///< message of sha prepare calculation
    uint32_t out_size = 0;		///< bytes of calculation result
    const uint8_t expect[20] = {0xa9, 0x99, 0x3e, 0x36, 0x47, 0x06, 0x81, 0x6a, 0xba, 0x3e, 0x25, 0x71, 0x78, 0x50, 0xc2, 0x6c, 0x9c, 0xd0, 0xd8, 0x9d};
    
    /* STEP 1: init sha */
	ret = csi_sha_init(&g_sha, 0);
    CHECK_RETURN(ret);
    
    /* STEP 2: clean context cache */
    memset((uint8_t *)g_sha_context.total, 0, sizeof(csi_sha_context_t));
    
    /* STEP 3: config sha-1*/
    ret = csi_sha_start(&g_sha, &g_sha_context, SHA_MODE_1);
    CHECK_RETURN(ret);
    
    /* STEP 4: clean output buff */
    memset(out, 0, sizeof(out));
    
    /* STEP 5: first encrypt characters(abc) */
    memcpy(in, SHA_MESSAGE_1, strlen(SHA_MESSAGE_1));
    ret = csi_sha_update(&g_sha, &g_sha_context, in, strlen(SHA_MESSAGE_1));
    CHECK_RETURN(ret);

    /* STEP 6: first encrypt characters(abc) */
    memcpy(in, SHA_MESSAGE_2, strlen(SHA_MESSAGE_2));
    ret = csi_sha_update(&g_sha, &g_sha_context, in, strlen(SHA_MESSAGE_2));
    CHECK_RETURN(ret);

    /* STEP 7: first encrypt characters(abc) */
    memcpy(in, SHA_MESSAGE_3, strlen(SHA_MESSAGE_3));
    ret = csi_sha_update(&g_sha, &g_sha_context, in, strlen(SHA_MESSAGE_3));
    CHECK_RETURN(ret);
    
    /* STEP 8: finish encrypt and output result */
    ret = csi_sha_finish(&g_sha, &g_sha_context, out, &out_size);
    CHECK_RETURN(ret);
    
    /* STEP 9: verification output results */
    ret = memcmp(out, expect, out_size);
    CHECK_RETURN(ret);
    
    return ret;
}
```

