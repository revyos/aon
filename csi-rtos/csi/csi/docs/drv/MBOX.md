# MBOX
-----

## 设备说明

　　MBOX（mail box）在多核SOC 系统中，核间通信是多核协同工作的保证。MAILBOX是以中断触发方式提供核间通信的IP。支持发送CPU 向接收CPU 传递除中断事件外的信息量，以信息寄存器组的方式体现。

## 接口列表

MBOX的CSI接口说明如下所示：

|   函数                  |     说明                 |
|-------------------------|-------------------------|
|csi_mbox_init              |初始化       |
|csi_mbox_uninit            |去初始化     |
|csi_mbox_attach_callback     |注册回调函数      |
|csi_mbox_detach_callback |注销回调函数    |
|csi_mbox_send       | 消息发送     |
|csi_mbox_receive      | 消息接收     |

## 接口详细说明

### csi_mbox_init

```c
csi_error_t csi_mbox_init(csi_mbox_t *mbox, uint32_t idx)
```

* 功能描述:

  * 通过设备ID初始化对应的mbox实例。

* 参数:
  
  * `mbox`: 设备句柄（需要用户申请句柄空间）。

  * `idx`: 设备ID。

* 返回值:

  * 错误码csi_error_t。
  
---

#### csi_mbox_t

| 成员              | 类型      | 说明    |
| :------------- | :--------- | :--------- |
| dev | csi_dev_t | csi设备统一句柄     |
| callback | void (*callback)(csi_mbox_t *mbox, csi_mbox_event_t event, uint32_t channel_id, uint32_t received_len, void *arg) | 用户回调函数     |
| arg | void * | 回调函数对应传参      |
| priv | void * | 设备私有变量 |

------

### csi_mbox_uninit

```c
void csi_mbox_uninit(csi_mbox_t *mbox)
```

* 功能描述:

  * mbox实例反初始化，该接口会停止mbox实例正在进行的工作，并且释放相关的软硬件资源。

* 参数:

  * `mbox`: 设备句柄。

* 返回值:

  * 无。

------

### csi_mbox_attach_callback

```c
csi_error_t csi_mbox_attach_callback(csi_mbox_t *mbox, void *callback, void *arg)
```

* 功能描述:

  * 注册回调函数。

* 参数:

  * `mbox`: 设备句柄。
  
  * `callback`: 中断回调函数。
  
  * `arg`: 回调函数对应的传参，由用户自定义。

* 返回值:

  * 错误码csi_error_t。


####  callback

```c
void (*callback)(csi_mbox_t *mbox, csi_mbox_event_t event, uint32_t channel_id, uint32_t received_len, void *arg);
```

其中mbox为设备句柄，event为事件类型，channel_id为通道号产生的事件，received_len为已接收到的字符长度，arg为用户自定义的回调函数对应的参数。 

---

#### csi_mbox_event_t

| 类型                     | 说明         |
| ------------------------ | ------------ |
| MBOX_EVENT_SEND_COMPLETE | 发送完成事件 |
| MBOX_EVENT_RECEIVED      | 接收事件     |

---

### csi_mbox_detach_callback

```c
void csi_mbox_detach_callback(csi_mbox_t *mbox)
```

* 功能描述:

  * 注销回调函数。

* 参数:

  * `mbox`: 设备句柄。

* 返回值：

  * 无。

---

## 初始化示例

```c
#include <stdio.h>
#include <soc.h>
#include <drv/mbox.h>

static csi_mbox_t g_mbox;

int main(void)
{
    csi_error_t ret = 0;

    /* init mbox 0 */
    ret = csi_mbox_init(&g_mbox, 0);
    
    return ret;
}
```

## 发送模式示例

(当前CPU0，目标CPU1)

```c
#include <stdio.h>
#include <soc.h>
#include <drv/mbox.h>

#define EXAMPLE_MBOX_IDX                0

/**
 * if current CPU is 0
*/
#define EXAMPLE_MBOX_TARGET_CPU         0     ///< 0:CPU1    1:CPU2
/**
 * if current CPU is 1
*/
// #define EXAMPLE_MBOX_TARGET_CPU         0     ///< 0:CPU0    1:CPU2
/**
 * if current CPU is 2
*/
// #define EXAMPLE_MBOX_TARGET_CPU         0     ///< 0:CPU0    1:CPU1


#define CHECK_RETURN(ret)                           \
        do {                                        \
            if (ret != 0) {                         \
                return -1;                          \
            }                                       \
        } while(0);
        
csi_mbox_t mbox;
volatile uint8_t send_event_done = 0U;
volatile uint8_t receive_event_done = 0U;
uint8_t tx_data[] = "Hello, Are you Ready?";
uint8_t rx_data[64];

static void mailbox_event_cb_fun(csi_mbox_t *mbox, csi_mbox_event_t event, uint32_t channel_id, uint32_t received_len, void *arg)
{
    if (event == MBOX_EVENT_SEND_COMPLETE) {
        send_event_done = 1U;
    } else if (event == MBOX_EVENT_RECEIVED) {
        csi_mbox_receive(mbox, channel_id, rx_data, received_len);
        receive_event_done = 1U;
    } else {
        printf("ERR->Error event handle\n");
    }
}
 
int example_mailbox(int32_t idx)
{
    csi_error_t ret;

    printf("1->Mbox Sender\n");

    ret = csi_mbox_init(&mbox, idx);
	CHECK_RETURN(ret);

    ret = csi_mbox_attach_callback(&mbox, mailbox_event_cb_fun, NULL);
	CHECK_RETURN(ret);

    printf("2->Sender Send data is:'%s'\n", tx_data);

    ret = csi_mbox_send(&mbox, EXAMPLE_MBOX_TARGET_CPU, tx_data, sizeof(tx_data));
	CHECK_RETURN(ret);

    while (!send_event_done);

    printf("3->Sender send ok\n");

    while (!receive_event_done);

    printf("4->Sender Recv data is:'%s'\n", rx_data);

    csi_mbox_detach_callback(&mbox);
    csi_mbox_uninit(&mbox);

    printf("5->test mbox-sender successfully\n");

    return 0;
}

```

## 接收模式示例

(当前CPU1，目标CPU0)

```c
#include <stdio.h>
#include <soc.h>
#include <drv/mbox.h>

#define EXAMPLE_MBOX_IDX                0

/**
 * if current CPU is 0
*/
// #define EXAMPLE_MBOX_TARGET_CPU         0     ///< 0:CPU1    1:CPU2
/**
 * if current CPU is 1
*/
#define EXAMPLE_MBOX_TARGET_CPU         0     ///< 0:CPU0    1:CPU2
/**
 * if current CPU is 2
*/
// #define EXAMPLE_MBOX_TARGET_CPU         0     ///< 0:CPU0    1:CPU1


#define CHECK_RETURN(ret)                           \
        do {                                        \
            if (ret != 0) {                         \
                return -1;                          \
            }                                       \
        } while(0);
        
csi_mbox_t mbox;
volatile uint8_t send_event_done = 0U;
volatile uint8_t receive_event_done = 0U;
uint8_t tx_data[] = "Yes, I am ready!";
uint8_t rx_data[64];

static void mailbox_event_cb_fun(csi_mbox_t *mbox, csi_mbox_event_t event, uint32_t channel_id, uint32_t received_len, void *arg)
{
    if (event == MBOX_EVENT_SEND_COMPLETE) {
        send_event_done = 1U;
    } else if (event == MBOX_EVENT_RECEIVED) {
        csi_mbox_receive(mbox, channel_id, rx_data, received_len);
        receive_event_done = 1U;
    } else {
        printf("ERR->Error event handle\n");
    }
}

int example_mailbox(int32_t idx)
{
    csi_error_t ret;

    printf("1->Mbox Receiver\n");

    ret = csi_mbox_init(&mbox, idx);
	CHECK_RETURN(ret);

    ret = csi_mbox_attach_callback(&mbox, mailbox_event_cb_fun, NULL);
	CHECK_RETURN(ret);

    while (!receive_event_done);

    receive_event_done = 0;
    printf("2->Receiver Recv data is:'%s'\n", rx_data);

    printf("3->Receiver Send data is:'%s'\n", tx_data);

    ret = csi_mbox_send(&mbox, EXAMPLE_MBOX_TARGET_CPU, tx_data, sizeof(tx_data));
	CHECK_RETURN(ret);

    while (!send_event_done);

    printf("4->reply message ok\n");

    csi_mbox_detach_callback(&mbox);
    csi_mbox_uninit(&mbox);

    printf("5->test mbox-sender successfully\n");

    return 0;
}
```
