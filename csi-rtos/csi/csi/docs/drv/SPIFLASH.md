# SPIFLASH
## 设备说明

SPI-FLASH 的物理介质是NOR-FLASH，是一种非易失性的内存，适合存放程序指令，程序只读属性数据据，文件系统，用户参数配置数据等。

## 接口列表
spiflash的CSI接口说明如下所示：

|   函数                      |     说明              |
|-----------------------------|----------------------|
|csi_spiflash_spi_init        |SPI-FLASH 初始化基于SPI控制器的设备初始化          |
|csi_spiflash_qspi_init       |SPI-FLASH 反初始化基于SPI控制器的实例        |
|csi_spiflash_spi_uninit      |SPI-FLASH 反初始化设置回调函数           |
|csi_spiflash_qspi_uninit     |SPI-FLASH 反初始化基于QSPI控制器的实例     |
|csi_spiflash_get_flash_info  |SPI-FLASH 获取设备能力信息       |
|csi_spiflash_read            |SPI-FLASH 读操作 |
|csi_spiflash_program         |SPI-FLASH 写操作       |
|csi_spiflash_erase           |SPI-FLASH 擦除flash         |
|csi_spiflash_read_reg        |SPI-FLASH 读状态寄存器    |
|csi_spiflash_write_reg       |SPI-FLASH 写状态寄存器    |
|csi_spiflash_lock            |SPI-FLASH 安全使能    |
|csi_spiflash_unlock          |SPI-FLASH 安全禁止    |
|csi_spiflash_is_locked       |SPI-FLASH 安全查询|
|csi_spiflash_config_data_line|SPI-FLASH 配置总线线数|
|csi_spiflash_frequence       |SPI-FLASH 配置频率|
|csi_spiflash_release_power_down |SPI-FLASH 退出低功耗|
|csi_spiflash_power_down      |SPI-FLASH 进入低功耗|

## 接口详细说明

### csi_spiflash_spi_init

```c
csi_error_t csi_spiflash_spi_init(csi_spiflash_t *spiflash, uint32_t spi_idx, void *spi_cs_callback)
```

- 功能描述:

  使用SPI控制器初始化对应的SPI-FLASH实例。

- 参数:

  - `spiflash`: 设备句柄（需要用户申请句柄空间）。
  - `idx`: SPI设备ID。
  - `spi_cs_callback`: 软件片选控制函数。

- 返回值: 

  - 错误码csi_error_t

* * *
### csi_spiflash_t

| 成员         | 类型                 | 说明                  |
| ------------ | ------------------- | --------------------- |
| spi_qspi     | csi_spi_qspi_t      | 设备统一句柄           |
| spi_cs_callback    | void (*spi_cs_callback)(csi_gpio_pin_state_t value) | 软件片选控制函数       |
| flash_prv_info | void                | flash设备信息         |
| spi_send     | csi_error_t (*spi_send)(void *spi, uint8_t cmd, uint32_t addr, uint32_t addr_size, const void *data, uint32_t size)  | 指向发送缓存的地址      |
| spi_receive  |  csi_error_t (*spi_receive)(void *spi, uint8_t cmd, uint32_t addr, uint32_t addr_size, void *data, uint32_t size) | 发送数据的大小          |
| priv         | void*            | 设备私有变量       |

* * *
#### csi_spi_callback_t

```c
typedef void (*csi_spi_cs_callback_t)(csi_gpio_pin_state_t value);
```

| 成员  | 类型        | 说明                |
| ----- | ---------- | ------------------- |
| value | csi_gpio_pin_state_t  | 引脚状态  |

* * *
#### csi_spiflash_addr_mode_t 

| 类型 | 说明    |
| ------------- | --------- |
| FLASH_ADDR_24 | 24位地址 |
| FLASH_ADDR_32 | 32位地址 |

* * *
#### csi_spiflash_lock_region_t

| 类型 | 说明      |
| --------------- | ------------ |
| LOCK_TP_NONE    | 无保护        |
| LOCK_TP_TOP     | Upper region |
| LOCK_TP_BOTTOM  | Lower region |
| LOCK_TP_DUAL    | 32位地址      |

* * *
#### csi_spiflash_info_t

| 成员         | 类型                 | 说明                  |
| ------------ | ------------------- | --------------------- |
| flash_name   | char                | spiflash名称          |
| flash_id     | uint32_t            | JEDEC ID              |
| flash_size   | uint32_t            | spiflash大小          |
| xip_addr     | uint32_t            | XIP地址               |
| sector_size  | uint32_t            | 扇区大小              |
| page_size    | uint32_t            | 页大小                |
| protect_blk_size    | uint32_t     | 保护区域单位           |
| protect_type    | csi_spiflash_lock_region_t    | 保护区域类型 |

### csi_spiflash_qspi_init

```c
csi_error_t csi_spiflash_qspi_init(csi_spiflash_t *spiflash, uint32_t qspi_idx, void *spi_cs_callback)
```

- 功能描述:

- 使用qspi控制器初始化对应的SPI-FLASH实例。

- 参数:

  - `spiflash`: 实例句柄。 
  - `qspi_idx`: 设备ID。 
  - `qspi_cs_callback`: 软件片选控制函数。

- 返回值

  - 错误码csi_error_t

### csi_spiflash_spi_uninit
```c
void csi_spiflash_spi_uninit(csi_spiflash_t *spiflash)
```

- 功能描述:

- 反初始化基于spi控制器的SPI-FLASH实例, 并且释放相关的软硬件资源。

- 参数:

  - `spiflash`: 实例句柄。 
- 返回值: 
  - 无

### csi_spiflash_qspi_uninit
```c
void csi_spiflash_qspi_uninit(csi_spiflash_t *spiflash)
```

- 功能描述:

- 反初始化基于qspi控制器的SPI-FLASH实例, 并且释放相关的软硬件资源。

- 参数:

  - `spiflash`: 实例句柄。 
- 返回值: 
  - 无


### csi_spiflash_get_flash_info
```c
csi_error_t csi_spiflash_get_flash_info(csi_spiflash_t *spiflash, csi_spiflash_info_t *flash_info);
```
- 功能描述:

- SPI-FLASH 获取设备能力信息。

- 参数:

  - `spiflash`: 实例句柄。 
  - `flash_info`: 保存flash信息的结构体指针。
- 返回值: 
  - 错误码csi_error_t


* * *
#### csi_spiflash_read

```c
int32_t csi_spiflash_read(csi_spiflash_t *spiflash, uint32_t offset, void *data, uint32_t size);
```

- 功能描述:

- 读取SPI-FLASH数据。

- 参数:

  - `spiflash`: 实例句柄。 
  - `offset`: flash的偏移地址。 
  - `data`: 指向接收数据缓存。 
  - `size`: 接收数据的大小。 
- 返回值

  - 错误码csi_error_t


### csi_spiflash_program

```c
int32_t csi_spiflash_program(csi_spiflash_t *spiflash, uint32_t offset, const void *data, uint32_t size);
```

- 功能描述:

- SPI-FLASH写数据。

- 参数:

  - `spiflash`: 实例句柄。 
  - `offset`: flash的偏移地址。 
  - `data`: 指向发送数据的缓存。 
  - `size`: 发送数据的大小。 

- 返回值

  - 错误码csi_error_t


### csi_spiflash_erase

```c
csi_error_t csi_spiflash_erase(csi_spiflash_t *spiflash, uint32_t offset, uint32_t size);
```

- 功能描述:

- 擦除SPI-FLASH数据。

- 参数:

  - `spiflash` : 实例句柄。
  - `offset` : flash的偏移地址，地址必须为擦除单位大小对齐。
  - `mode` : 擦除长度必须为擦除单位的整数倍。

- 返回值: 

  - 错误码csi_error_t

### csi_spiflash_read_reg

```c
csi_error_t csi_spiflash_read_reg(csi_spiflash_t *spiflash, uint8_t cmd_code, uint8_t *data, uint32_t size);
```

- 功能描述:

-  SPI-FLASH 读状态寄存器。

- 参数:

  - `spiflash`: 实例句柄
  - `cmd_code`: 读取spi 寄存器的命令字
  - `data`: 指向接收的数据缓存
  - `size`: 数据大小

- 返回值: 

  - 错误码csi_error_t

### csi_spiflash_write_reg
```c
csi_error_t csi_spiflash_write_reg(csi_spiflash_t *spiflash, uint8_t cmd_code, uint8_t *data, uint32_t size);
```

- 功能描述:

-  SPI-FLASH 写状态寄存器。

- 参数:

  - `spiflash`: 实例句柄
  - `cmd_code`: 写spi 寄存器的命令字
  - `data`: 指向发送的数据缓存
  - `size`: 数据大小

- 返回值: 

  - 错误码csi_error_t


### csi_spiflash_lock
```c
csi_error_t csi_spiflash_lock(csi_spiflash_t *spiflash, uint32_t offset, uint32_t size);
```

- 功能描述:

-  SPI-FLASH 安全使能。

- 参数:

  - `spiflash`: 实例句柄
  - `offset`: flash 保护的开始地址，地址必须protect block size 对齐，请参考csi_spiflash_info_t
查看flash的保护单位size
  - `size`:  flash 保护的区域大小，大小为 protect blck size的整数倍

- 返回值: 

  - 错误码csi_error_t


### csi_spiflash_unlock
```c
csi_error_t csi_spiflash_unlock(csi_spiflash_t *spiflash, uint32_t offset, uint32_t size);
```
- 功能描述:

-  SPI-FLASH 安全禁止。

- 参数:

  - `spiflash`: 实例句柄
  - `offset`: flash 保护的开始地址，地址必须protect block size 对齐，请参考csi_spiflash_info_t
查看flash的保护单位size。
  - `size`:   flash 保护的区域大小，大小为 protect blck size的整数倍

- 返回值: 

  - 错误码csi_error_t


### csi_spiflash_is_locked
```c
int csi_spiflash_is_locked(csi_spiflash_t *spiflash, uint32_t offset, uint32_t size);
```
- 功能描述:

-  SPI-FLASH 安全查询。

- 参数:

  - `spiflash`: 实例句柄
  - `offset`:  flash 偏移地址。
  - `size`:    flash 查询的大小，大小必须为protect_blk_size 的整数倍

- 返回值: 

  - 错误码csi_error_t

### csi_spiflash_config_data_line
```c
csi_error_t csi_spiflash_config_data_line(csi_spiflash_t *spiflash, csi_spiflash_data_line_t line);
```
- 功能描述:

-  SPI-FLASH 配置总线线数。

- 参数:

  - `spiflash`: 实例句柄
  - `line`:   总线线数

- 返回值: 

  - 错误码csi_error_t

* * *
#### csi_spiflash_data_line_t

| 类型 | 说明     |
| ------------- | --------- |
| SPIFLASH_DATA_1_LINE | 单线模式 |
| SPIFLASH_DATA_2_LINES | 双线模式 |
| SPIFLASH_DATA_4_LINES | 四线模式 |

### csi_spiflash_frequence
```c
uint32_t csi_spiflash_frequence(csi_spiflash_t *spiflash, uint32_t hz);
```
- 功能描述:

-  SPI-FLASH 配置频率。

- 参数:

  - `spiflash`: 实例句柄
  - `hz`:  频率 

- 返回值: 

  - 实际频率值 


### csi_spiflash_release_power_down
```c
csi_error_t csi_spiflash_release_power_down(csi_spiflash_t *spiflash);
```
- 功能描述:

-  SPI-FLASH 退出低功耗模式。

- 参数:

  - `spiflash`: 实例句柄

- 返回值:

  - 错误码csi_error_t


### csi_spiflash_power_down
```c
void csi_spiflash_power_down(csi_spiflash_t *spiflash);
```
- 功能描述:

-  SPI-FLASH 进入低功耗模式。

- 参数:

  - `spiflash`: 实例句柄

- 返回值:

  - 无


## 基于QSPI控制器的SPIFLASH示例


```c
#include <stdio.h>
#include <string.h>

#include <soc.h>
#include <drv/tick.h>
#include <csi_config.h>
#include <drv/spiflash.h>
#include <board_init.h>
#include <board_config.h>

/* example parameters */
#define EXAMPLE_FLASH_START_ADDRESS        0x50
#define EXAMPLE_FLASH_WRITE_SIZE           0x250


#define CHECK_RETURN(ret)                \
    do {                                 \
        if (ret != CSI_OK) {             \
            return -1;                   \
        }                                \
    } while(0);     

static csi_spiflash_t spiflash;

int main(void)
{
    int ret = 0;
    csi_spiflash_info_t info;
    uint8_t tx_data[EXAMPLE_FLASH_WRITE_SIZE];
    uint8_t rx_data[EXAMPLE_FLASH_WRITE_SIZE];
    uint32_t address, size;

    board_init();

    /* Initialize SPIFLASH based on spi controler */
    ret = csi_spiflash_qspi_init(&spiflash, 0);
    CHECK_RETURN(ret);

    /* Get SPIFLASH information */
    ret = csi_spiflash_get_flash_info(&spiflash, &info);
    CHECK_RETURN(ret);

    /* Chip erase */
    ret = csi_spiflash_erase(&spiflash, 0, info.flash_size);
    CHECK_RETURN(ret);
    
    /* Prefill tx_data buffer */
    memset(tx_data, 0xaa, sizeof(tx_data));

    /* Write data to flash */
    address = EXAMPLE_FLASH_START_ADDRESS;
    size    = EXAMPLE_FLASH_WRITE_SIZE;
    ret = csi_spiflash_program(&spiflash, address, tx_data, size);

    if(ret != size){
        return -1;
    }

    /* Read data from SPIFLASH */
    address = EXAMPLE_FLASH_START_ADDRESS;
    size    = EXAMPLE_FLASH_WRITE_SIZE;
    ret = csi_spiflash_read(&spiflash, address, rx_data, size);

    if(ret != size){
        return -1;
    }

    /* Data compare */
    ret = memcmp(tx_data, rx_data, size);
    CHECK_RETURN(ret);

    csi_spiflash_qspi_uninit(&spiflash);
    return 0;
}

```


## 基于SPI控制器的SPIFLASH示例
使用前需要确保：

>1. 确保已经增加SPIFLASH信息到vendor列表

>2. 检查IO连接


```c

#include <stdio.h>
#include <string.h>

#include <soc.h>
#include <drv/tick.h>
#include <csi_config.h>
#include <drv/spiflash.h>
#include <board_init.h>
#include <board_config.h>

/* example parameters */
#define EXAMPLE_FLASH_START_ADDRESS        0x50
#define EXAMPLE_FLASH_WRITE_SIZE           0x250


#define CHECK_RETURN(ret)                \
    do {                                 \
        if (ret != CSI_OK) {             \
            return -1;                   \
        }                                \
    } while(0);     

static csi_gpio_t gpio;
static csi_spiflash_t spiflash;

static void example_pin_init(void)
{
    csi_pin_set_mux(EXAMPLE_PIN_SPI_MISO, EXAMPLE_PIN_SPI_MISO_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_SPI_MOSI, EXAMPLE_PIN_SPI_MOSI_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_SPI_CS,   EXAMPLE_PIN_SPI_CS_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_SPI_SCK,  EXAMPLE_PIN_SPI_SCK_FUNC);
}

static void soft_cs_pin_init(void)
{
    csi_pin_set_mux(EXAMPLE_PIN_SPI_CS, PIN_FUNC_GPIO);
    csi_gpio_init(&gpio, 0);
    csi_gpio_dir(&gpio, EXAMPLE_PIN_SPI_CS_MSK, GPIO_DIRECTION_OUTPUT);
}

static void ioctl(csi_gpio_pin_state_t value)
{
    csi_gpio_write(&gpio, EXAMPLE_PIN_SPI_CS_MSK, value);
}

int main(void)
{
    int ret = 0;
    csi_spiflash_info_t info;
    uint8_t tx_data[EXAMPLE_FLASH_WRITE_SIZE];
    uint8_t rx_data[EXAMPLE_FLASH_WRITE_SIZE];
    uint32_t address, size;

    board_init();
    example_pin_init();
    soft_cs_pin_init();

    /* Initialize SPIFLASH based on spi controler */
    ret = csi_spiflash_spi_init(&spiflash, 0, ioctl);
    CHECK_RETURN(ret);

    /* Get SPIFLASH information */
    ret = csi_spiflash_get_flash_info(&spiflash, &info);
    CHECK_RETURN(ret);

    /* Chip erase */
    ret = csi_spiflash_erase(&spiflash, 0, info.flash_size);
    CHECK_RETURN(ret);
    
    /* Prefill tx_data buffer */
    memset(tx_data, 0xaa, sizeof(tx_data));

    /* Write data to flash */
    address = EXAMPLE_FLASH_START_ADDRESS;
    size    = EXAMPLE_FLASH_WRITE_SIZE;
    ret = csi_spiflash_program(&spiflash, address, tx_data, size);

    if(ret != size){
        return -1;
    }

    /* Read data from SPIFLASH */
    address = EXAMPLE_FLASH_START_ADDRESS;
    size    = EXAMPLE_FLASH_WRITE_SIZE;
    ret = csi_spiflash_read(&spiflash, address, rx_data, size);

    if(ret != size){
        return -1;
    }

    /* Data compare */
    ret = memcmp(tx_data, rx_data, size);
    CHECK_RETURN(ret);

    csi_spiflash_spi_uninit(&spiflash);
    return 0;
}

```
