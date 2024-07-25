#include "aonsys_clkgen_reg_define.h"
#include "apsys_clkgen_reg_define.h"
#include "vpsys_sysreg_define.h"
#include "visys_sysreg_define.h"
#include "miscsys_sys_define.h"

#define CLK_GATE(name, ereg, ebit) \
{				   \
	.clk_name = name,	   \
	.clken_addr = ereg,	   \
	.en_bit = ebit,		   \
}

typedef enum  _LIGHT_CLKGATE_NAME {
	MISCSYS_SDIO0,
	MISCSYS_SDIO1,
	APSYS_I2C0,
	APSYS_I2C1,
	APSYS_I2C2,
	APSYS_I2C3,
	APSYS_I2C4,
	APSYS_I2C5,
	APSYS_PWM,
	VPSYS_G2D,
	VPSYS_FCE,
	VISYS_ISP0,
	VISYS_ISP1,
	VISYS_ISP_RY,
}LIGHT_CLKGATE_NAME;

typedef struct {
	LIGHT_CLKGATE_NAME clk_name;
	uint32_t clken_addr;
	uint32_t en_bit;
} clk_gate_t;

typedef struct  {
	char *ip_name;
	LIGHT_CLKGATE_NAME clk_name;
} clk_match_list_t;

bool ip_clk_is_active(LIGHT_CLKGATE_NAME clk_name);
