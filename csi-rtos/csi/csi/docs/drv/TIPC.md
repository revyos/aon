# TIPC
-----

## 说明

　　可信IP控制器（TIPC）是在 CSKY TEE 安全系统中一个辅助安全模块，它负责配置 IP 在非可信世界 的访问权限，TIPC 只能被可信世界访问，任何非可信世界的访问都将无效。

## 接口列表
PIN的CSI接口说明如下所示：

|   函数                  |     说明                 |
|-------------------------|-------------------------|
|csi_dev_secure_config            |配置TIPC模块是否可信            |
|soc_tipc_enable_secure           |配置TIPC模块可信                |
|soc_tipc_disenable_secure        |配置TIPC模块不可信              |

## 接口详细说明

### csi_dev_secure_config

```c
csi_error_t csi_dev_secure_config(csi_dev_t *dev, bool is_secure)
```

- 功能描述:

  - 配置TIPC模块是否可信。

- 参数:

  - dev: CSI设备
  - is_secure:  是否可信。

- 返回值:
  - 错误码csi_error_t。

```c
void soc_tipc_enable_secure(soc_tipc_module_t ip);
```
- 功能描述:

  - 配置TIPC模块为可信模式。

- 参数:

  - ip:  IP在TICP中的使能位的值。

- 返回值:
  - 无返回值

```c
void soc_tipc_disable_secure(soc_tipc_module_t ip);
```
- 功能描述:

  - 配置TIPC模块为不可信模式。

- 参数:

  - ip:  IP在TICP中的使能位的值。

- 返回值:
  - 无返回值


## 使用示例-结合TIPC的example进行讲解
- 测试方法
  - 通过在可信系统下访问IP，修改IP的可信属性，然后向IP的地址写再读出来看看是否一致，可信系统下对任何状态的IP都是可读可写的。
  - 通过在不可信系统下访问IP，修改IP的可信属性，然后向IP的地址写再读出来看看是否一致，不可信系统下对不可信IP是可读可写的，对可信IP时不可写的，写操作会失败。

- example内接口列表如下，大的分为两个模式，可信系统下访问IP，和非可信系统下访问IP

- 可信系统模式下的接口，系统一开始默认处于可信状态。

|   函数                  |     说明                 |
|------------------------ |-------------------------|
|tipc_trust_sys_test      |可信系统模式下测试         |
|------------------------ |-------------------------|
|tipc_trust_sys_untrust_module |可信系统模式下访问不可信IP|
|------------------------ |-------------------------|
|tipc_trust_sys_trust_module   |可信系统模式下访问可信IP  |

- 上面的接口在可信系统模式下测试，默认系统就处于可信状态，直接测试就可以。
- 而TIPC下的各个IP默认处于不可信模式，可以通过 csi_dev_secure_config 来改变IP的可信属性。
- 访问各个IP的写接口 drv_tipc_set_module_rw_addr，读接口为 drv_tipc_get_module_rw_addr，可以根据比较写入是否成功来判断测试是否成功。


- 不可信可信系统模式下的接口

- 测试非可信模式系统时先调用下面两句代码，将系统设置为可信模式。

 __set_PSR(0x80000000);
__set_PSR(0x80000140);

|   函数                  |     说明                 |
|------------------------ |-------------------------|
|tipc_untrust_sys_test    |不可信系统模式下测试       |
|------------------------ |-------------------------|
|tipc_untrust_sys_trust_module |不可信系统模式下访问可信IP|
|------------------------ |-------------------------|
|tipc_untrust_sys_untrust_module |不可信系统模式下访问不可信IP|

- 此时系统已处于非可信模式，调用 csi_dev_secure_config 接口来修改IP的可信属性，来测试。
- 分别测试不可信系统模式下访问可信IP、不可信系统模式下访问不可信IP。
- 访问各个IP的写接口 drv_tipc_set_module_rw_addr，读接口为 drv_tipc_get_module_rw_addr，可以根据比较写入是否成功来判断测试是否成功。

