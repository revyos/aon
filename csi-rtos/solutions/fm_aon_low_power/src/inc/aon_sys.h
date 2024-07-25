

#ifndef __AON_SYS_H_
#define __AON_SYS_H_

#include "drv/pmic.h"

enum aon_rpc_sys_svc_en {
    SYS_FUNC_UNKNOWN          = 0U,
    SYS_FUNC_AON_RESERVE_MEM  = 1U,
};

int aon_sys_init();
int set_aon_rebootmode(chip_rebootmode_index_en mode);

#endif