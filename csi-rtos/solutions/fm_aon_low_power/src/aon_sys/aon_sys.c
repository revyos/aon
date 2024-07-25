#include <stdint.h>

#include "aon_sys.h"
#include "aon_rpc.h"
#include "aon_common.h"
#include "drv/pmic.h"
#include "syslog.h"

static uint32_t aon_sram_reservemem_addr;
static uint32_t aon_sram_reservemem_init = 0;

#define AON_SRAM_BASE_ADDR 0xFFEF8000
#define LIGHT_AON_SRAM_LEN 0x10000
#define LIGHT_AON_SRAM_RESERV (LIGHT_AON_SRAM_LEN - 0x100)

int set_aon_reservemem_addr(uint32_t offset)
{
        if (offset >= LIGHT_AON_SRAM_LEN || offset < LIGHT_AON_SRAM_RESERV) {
                LOG_D("offset:0x%x exceed!\n", offset);
                return AON_ERR_PARM;
        }
        aon_sram_reservemem_init = 1;
        aon_sram_reservemem_addr = AON_SRAM_BASE_ADDR + offset;

        LOG_D("aon_sram_reservemem_addr:0x%x offset:0x%x\n",
              aon_sram_reservemem_addr, offset);
        return AON_ERR_NONE;
}

int set_aon_rebootmode(chip_rebootmode_index_en mode)
{
        if (!aon_sram_reservemem_init) {
                LOG_D("aon_sram_reservemem_init not initialized!\n");
                return AON_ERR_PARM;
        }
        LOG_D("mode:0x%x\n", mode);

        *(uint32_t *)aon_sram_reservemem_addr = mode;
        return AON_ERR_NONE;
}

int check_aon_rebootmode(void)
{
        uint32_t    flag = 0;
        uint8_t     ret;
        csi_pmic_t *pmic = aon_pmic_get_handler();
        if (!pmic) {
                LOG_E("Get pmic faild");
                return AON_ERR_FAIL;
        }

        ret = csi_pmic_get_powerup_flag(pmic, &flag);
        if (ret)
                return AON_ERR_FAIL;
        if (flag)
                set_aon_rebootmode(CHIP_EVENT_PMIC_ONKEY);

        return AON_ERR_NONE;
}

static void sys_dispatch(aon_rpc_msg_t *msg, aon_rpc_msg_t *tx_msg)
{
        uint8_t    func     = RPC_GET_FUNC(msg);
        aon_err_en result   = AON_ERR_NONE;
        uint8_t    ack_size = 0;

        switch (func) {
                case SYS_FUNC_AON_RESERVE_MEM: {
                        uint32_t offset;
                        RPC_GET_BE32(msg->data, 0U, &offset);
                        result = set_aon_reservemem_addr(offset);
                        if (result != AON_ERR_NONE) {
                                LOG_D("set aon reservemem failed\n");
                        }

                        result = check_aon_rebootmode();
                        if (result != AON_ERR_NONE) {
                                LOG_D("check pmic rebootmode failed\n");
                        }
                        ack_size = 1U;
                } break;
                        /* Handle default */
                default: {
                        ack_size = 1;
                        result   = AON_ERR_NOTFOUND;
                } break;
        }
        /* Fill in header */
        RPC_SET_HEAD(tx_msg, AON_RPC_VERSION, ack_size, RPC_SVC_MSG_TYPE_ACK,
                     RPC_SVC_MSG_NO_NEED_ACK, RPC_GET_SVC_ID(msg), func,
                     result);
        return;
}

int aon_sys_init()
{
        return aon_rpc_svc_cb_register(AON_RPC_SVC_SYS, sys_dispatch);
}
