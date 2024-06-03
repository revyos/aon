# IRQ

## 说明

IRQ用于管理系统中的中断相关的功能，包括低功耗的中断唤醒，中断服务函数的注册等。

## 接口列表

| 函数               | 说明                  |
| :---------------- | :-------------------- |
| csi_irq_enable    | 使能中断号对应的中断|
| csi_irq_disable   | 禁止中断号对应的中断 |
| csi_irq_attach    | 注册中断号的服务函数|
| csi_irq_detach    | 注销中断号对应的服务函数 |
| csi_irq_priority   | 设置中断优先级 |
| csi_irq_is_enabled | 查询中断是否使能 |
| csi_irq_enable_wakeup  | 使能中断号唤醒功能 |
| csi_irq_disable_wakeup | 禁止中断号唤醒功能 |
| csi_irq_context        | 判断是否在中断处理中 |
| do_irq                 | 统一中断入口 |

## 接口详细说明

### csi_irq_enable

```c
__ALWAYS_STATIC_INLINE void csi_irq_enable(uint32_t irq_num)
```

* 功能描述:

  * 使能中断号对应的中断

* 参数：

  * `irq_num`: 中断号 

------

### csi_irq_disable

```c
__ALWAYS_STATIC_INLINE void csi_irq_disable(uint32_t irq_num)
```

* 功能描述:

  * 关闭中断号对应的中断

* 参数：

  * `irq_num`: 中断号 

### csi_irq_attach

```c
void csi_irq_attach(uint32_t irq_num, void *irq_handler, csi_dev_t *dev)
```

* 功能描述:

  * 注册中断号与设备对应的中断服务函数

* 参数：

  * `irq_num`: 中断号 
  * `irq_handler`: 中断服务函数
  * `dev`: CSI设备

------

### csi\_irq\_detach

```c
void csi_irq_detach(uint32_t irq_num)
```

* 功能描述:

  * 注销中断号对应的中断服务函数

* 参数：

  * `irq_num`: 中断号 

------

### csi_irq_priority

```c
__ALWAYS_STATIC_INLINE void csi_irq_priority(uint32_t irq_num, uint32_t priority)
```

* 功能描述:

  * 设置中断的优先级。

* 参数：

  * `irq`: 中断号 
  * `priority`:优先级

------

### csi_irq_is_enabled

```c
static inline bool csi_irq_is_enabled(uint32_t irq_num)
```

* 功能描述:

  * 查看中断是否使能

* 参数：

  * `irq_num`: 中断号 

* 返回值:

  * true表示已经使能，false表示未使能

------

### csi_irq_enable_wakeup

```c
__ALWAYS_STATIC_INLINE void csi_irq_enable_wakeup(uint32_t irq_num)
```

* 功能描述:

  * 使能对应中断的唤醒功能，用于低功耗模式唤醒

* 参数：

  * `irq_num`: 中断号 

------

### csi_irq_disable_wakeup

```c
__ALWAYS_STATIC_INLINE void csi_irq_disable_wakeup(uint32_t irq_num)
```

* 功能描述:

  * 关闭对应中断的唤醒功能，用于低功耗模式唤醒

* 参数：

  * `irq_num`: 中断号 

------

### csi_irq_context

```c
bool csi_irq_context(void)
```

* 功能描述:

  * 查询当前是否处于中断中

* 返回值：

  * true为处于中断，false为不处于中断中

------

### do_irq

```c
void do_irq(uint32_t irq_num)
```

* 功能描述:

  * 统一中断入口函数

* 参数：

  * `irq_num`: 中断号 

------
