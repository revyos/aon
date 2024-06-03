#ifndef __AON_LPM_H_
#define __AON_LPM_H_


enum aon_rpc_lpm_svc_en {
    LPM_FUNC_UNKNOWN          = 0U,
    LPM_FUNC_AON_REQUIRE_STR  = 1U,
    LPM_FUNC_AON_RESUME_STR	  = 2U,
    LPM_FUNC_AON_RESUME_STD	  = 3U,
    LPM_FUNC_AON_CPUHP		  = 4U,
};

int  aon_lpm_init(uint32_t wakeup_flag);

#endif