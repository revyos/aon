#include <stdio.h>
#include <drv/common.h>
#include <stdint.h>
#include "aon_common.h"
#include "aon_rpc.h"
#include "aon_misc.h"
#include "aon_wdt.h"
#include "syslog.h"
#include "power.h"

extern aon_wdt_t g_aon_wdt;
static reginfo_t g_reginfo;
static mapinfo_t g_map;
extern csi_perip_reg_t g_soc_reg[];
/**
 * rd_init - Initialize RegDump Service
 * @start_addr: Register Dump start address
 * @cfg_offset: Register Dump configuration address
 * @size: Register Dump reverved memory size
 *
 * Reg dump Mapping :
 *				       _
 * |       | --- start_addr		|
 * |	   |				|
 * |	   |				| size
 * |_______|   _		        |
 * |       |    | cfg_offset		|
 * |       |   _|		       _|
 */
csi_error_t rd_init(reginfo_t *RegInfo, mapinfo_t *Map, uint32_t start_addr, uint32_t cfg_offset, uint32_t size)
{
	csi_error_t ret = CSI_OK;
	char dumpflag[MaxDumpLen] = {0};

	Map->dump_start = start_addr;
	Map->dump_end = start_addr + size - cfg_offset;
	Map->info_start = start_addr + size - cfg_offset;
	Map->info_end = start_addr + size;

	Map->dump_flag = true;

	/* Initialize RegDump match members */
	strncpy((char *)&RD_NAME(RegInfo), "IP_name", strlen("IP_name"));
	strncpy((char *)&RD_BASE(RegInfo), "Addr", strlen("Addr"));
	strncpy((char *)&RD_EN(RegInfo), "Enable", strlen("Enable"));
	strncpy((char *)&RD_SIZE(RegInfo), "Size", strlen("Size"));

	return ret;
}

void dec_to_string(uint32_t value, char *buffer, uint32_t width)
{
    static const char digits [] = "0123456789abcdef";
    char *ptr = buffer;

    while (width > 0) {
        uint32_t digit = value % 16;
        value /= 16;
        *ptr++ = digits[digit];
		width--;
    }

    *ptr = '\0';

    for (char *head = buffer, *tail = ptr - 1; head < tail; ++head, --tail) {
        char temp = *head;
        *head = *tail;
        *tail = temp;
    }
}

/**
 * rd_reg_write - Set specific contents
 * @ cur_base: the cur ptr of dest_base.
 * @ contents:
 * @ lenth: content lenth
 * @ move_ptr: if move the ptr after write
 *
 */
void rd_reg_write(uint32_t *cur_base, char *contents, uint8_t lenth, bool move_ptr)
{
	if(cur_base == NULL) {
	    	LOG_E("invalid target address provided. \n");
	    	return ;
	}

	char reg_buffer[MaxDumpLen + 1U] = {" "};

	strncpy((char *)*cur_base, reg_buffer, lenth + 1U);
	strncpy(reg_buffer, contents, lenth);
	strcat(reg_buffer, "\n");
	strncpy((char *)*cur_base, reg_buffer, strlen(reg_buffer));
	if(move_ptr)
	    *cur_base += strlen(reg_buffer);
}

bool reg_need_check(uint32_t addr, csi_perip_reg_t *info)
{
	csi_perip_reg_t *match_info = &g_soc_reg;

	while(match_info->dev_tag)
	{
		if(match_info->reg_base == addr) {
			*info = *match_info;
			return true;
		}
		match_info++;
	}
	return false;
}

bool reg_is_inrange(uint32_t addr, csi_perip_reg_t *info)
{
    reg_range_t node;
    uint32_t addr_start;
    uint32_t addr_end;

    for(uint8_t i = 0; i < info->reg_count; i++)
    {
	node = info->range_list[i];
	addr_start = info->reg_base + (uint32_t)node.start_offset;
	addr_end = info->reg_base + (uint32_t)node.end_offset;
	if((addr >= addr_start && addr <= addr_end))
	return true;
    }
    return false;
}

/**
 * rd_set_mem - Read Register Value from specific Start Addr with Offset
 * @ cur_base: the cur ptr of dest_base.
 * @ source_base: the register addr that reads from.
 * @ num: numbers of registers in one line.
 *
 * Tempelete:
 * reg\:\ reg1\ reg2\ reg3\ reg4\ \n
 */
void rd_reg_fillline(uint32_t *cur_base, uint32_t *source_base, uint8_t num, bool check, csi_perip_reg_t *info)
{
	char line_buffer[MaxDumpLen] = {0};
	char reg_buffer[MaxDumpLen] = {" "};
	uint32_t reg_value = 0;
	uint32_t reg_addr = *source_base;

	line_buffer[0] = '\0';
	/* dump register adress first*/
	dec_to_string(reg_addr, reg_buffer, RegDumpLen);
	strcat(line_buffer, reg_buffer);
	strcat(line_buffer, ": ");
	/* dump the register contents, four for one line */
	for(uint8_t i=0; i<num ; i++)
	{
		reg_value = 0;
		if(!check || (check && !reg_is_inrange(*source_base, info)))
		    reg_value = *(volatile uint32_t *)*source_base;
		dec_to_string(reg_value, reg_buffer, RegDumpLen);
     		strcat(line_buffer, reg_buffer);
		strcat(line_buffer, " ");
		*source_base += RegWidth;
	}
	strcat(line_buffer, "\n");
	strncpy((char *)*cur_base, line_buffer, strlen(line_buffer));
	*cur_base += strlen(line_buffer);
}

/**
 * rd_set_mem - Read Register Value from specific Start Addr with Offset
 * @
 * @
 *
 * Tempelete:
 * IP_NAME
 * Address: xxxx xxxx xxxx xxxx
 * Address: xxxx xxxx xxxx xxxx
 */
csi_error_t rd_set_mem(reginfo_t *RegInfo, uint32_t *dest_base, uint32_t dest_end)
{
	csi_error_t ret = CSI_OK;
	uint32_t source_base = RegInfo->base_info.base;
	uint32_t source_size = RegInfo->size_info.size;
	uint32_t lines = source_size / (4 * RegWidth);		//four members for oneline
	uint8_t leftovers = (source_size %(4 * RegWidth)) / RegWidth;	//members in last line
	csi_perip_reg_t info;
	bool hole_check;

	/* dump IP name as title */
	rd_reg_write(dest_base, &RegInfo->name_info.name, NameDumpLen, true);

	/* check IP is active and dump enabled */
	if((RegInfo->en_info.en == false)
	    || (ip_is_active(&RegInfo->name_info.name) == false))
		return ret;
	/* check if there is specific address in the area that cannot be read */
	hole_check = reg_need_check(source_base, &info);

	/* dump register contents */
	for(uint32_t i=0; i < lines; i++)
	{
		rd_reg_fillline(dest_base, &source_base, 4, hole_check, &info);
	}
	if(leftovers)
	    rd_reg_fillline(dest_base, &source_base, leftovers, hole_check, &info);
	return ret;
}

/**
 * rd_read_cfg - Read Each specific IP dump cfg
 * @ CfgAddr: current IP config address.
 * @ RegInfo: register name, address, size and enable.
 *
 * [Bytes][   8   ][  8  ][  4  ][  4  ][  4  ][  4  ][   7   ][  1  ]
 *	  [IP_Name][xxxxx][Addr ][xxxxx][ Size][xxxxx][Enable ][xxxxx]
 */
csi_error_t rd_read_cfg(reginfo_t *RegInfo, uint32_t *CfgAddr, uint32_t offset)
{
	reginfo_t *GetInfo = (reginfo_t *)*CfgAddr;

	if(!strncmp(RD_NAME(GetInfo), RD_NAME(RegInfo), NameDumpLen)
		&& !strncmp(RD_BASE(GetInfo), RD_BASE(RegInfo), BaseDumpLen)
		&& !strncmp(RD_EN(GetInfo), RD_EN(RegInfo), EnDumpLen)
		&& !strncmp(RD_SIZE(GetInfo), RD_SIZE(RegInfo), SizeDumpLen))
	{
			*CfgAddr += offset;
			memcpy(RegInfo, GetInfo, sizeof(reginfo_t));
	}else {
			return CSI_ERROR;
	}
	LOG_I("IP_NAME:%s, IP_BASE:%d, IP_SIZE:%d, IP_ENABLE:%d\n",
				   	RegInfo->name_info.name, RegInfo->base_info.base,
					RegInfo->size_info.size, RegInfo->en_info.en);
	return CSI_OK;
}

/**
 * rd_set_mem - Read Register Value from specific Start Addr with Offset
 * @
 * @
 *
 */
void aon_regdump_handler()
{
	csi_error_t ret = CSI_OK;
	uint32_t cfg_offset;
	uint32_t dump_ptr = g_map.dump_start;
	uint32_t sign_ptr;
	char magic_string[MaxDumpLen] = "DumpTrue";

	if(!g_map.dump_flag)
	{
		LOG_E("Dump flag is not set\n");
		return ;
	}

	/* set dump flag*/
	strcat(magic_string, "Current Dump IP name:");
	rd_reg_write(&dump_ptr, &magic_string, strlen(magic_string), false);
	sign_ptr = g_map.dump_start + strlen(magic_string);
	dump_ptr = g_map.dump_start + strlen(magic_string) + NameDumpLen + 1U;
	rd_reg_write(&dump_ptr, "\n", 1, true);
	cfg_offset = (uint32_t)g_map.info_start;

	while(1)
	{
			if ((cfg_offset < g_map.info_start) || (cfg_offset > g_map.info_end))
			{
				LOG_E("Info grabbing process is out of range, quit with error\n");
				goto OUT;
			}

			ret = rd_read_cfg(&g_reginfo, &cfg_offset, sizeof(g_reginfo));
			if(ret)
				goto OUT;
			rd_reg_write(&sign_ptr, &g_reginfo.name_info.name, NameDumpLen, false);
			ret = rd_set_mem(&g_reginfo, &dump_ptr, g_map.dump_end);
			if(ret)
				goto OUT;
	}
OUT:
	strncpy(magic_string, "------------- Ends here -------------",
			strlen("------------- Ends here -------------"));
	g_map.dump_flag = false;
	rd_reg_write(&dump_ptr, &magic_string, strlen(&magic_string), false);
	LOG_I("[RegDump]regdump finished\n");
	return ;
}


static void misc_dispatch(aon_rpc_msg_t *msg, aon_rpc_msg_t *tx_msg)
{
        uint8_t    func     = RPC_GET_FUNC(msg);
        uint8_t    ack_size = 0;
        aon_err_en result   = AON_ERR_NONE;
	csi_error_t ret;

        switch (func) {
                        /* Handle uknown function */
                case MISC_FUNC_UNKNOWN: {
                        ack_size = 1;
                        result   = AON_ERR_NOTFOUND;
                }
		break;
		case MISC_FUNC_REGDUMP_CFG:
		{
				uint32_t start_addr;
				uint32_t cfg_offset;
				uint32_t size;
				RPC_GET_BE64(msg->data, 0U, &start_addr);
				RPC_GET_BE64(msg->data, 8U, &size);
				RPC_GET_BE64(msg->data, 16U, &cfg_offset);

				ret = rd_init(&g_reginfo, &g_map, start_addr, cfg_offset, size);
				if(ret == CSI_UNSUPPORTED)
					result = AON_ERR_NOTFOUND;
				/* RegDump relys on DDR retention, in such case system repower
				 * should leave DDR out of the picture. */
				g_aon_wdt.hw_wdt.type = AON_HW_WDT_CRASH_DUMP;
				LOG_D("got MISC_FUNC_REGDUMP_CFG startaddr=%d,"
							   	"size=%d, cfg_offset=%d\n",
								start_addr, size, cfg_offset);
				ack_size = 1;
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

#if AON_TRACE_ON
        /* Return feedback parameters */
        LOG_D("ipc_debug:rx(%d)", RPC_GET_FUNC(msg));
        dumpBuf((uint8_t *)msg, 28);
        LOG_D("ipc_debug:tx(%d)", RPC_GET_FUNC(tx_msg));
        dumpBuf((uint8_t *)tx_msg, 28);
#endif

        /* Handle error debug */
        if (result != AON_ERR_NONE) {
                LOG_D("ipc_err: api=misc, func=%d, err=%d\n", func, result);
        }
        LOG_D("ipc: api=misc, func=%d, err=%d\n", func, result);
        return;

}

int aon_misc_init()
{
     init_regmap();
     return aon_rpc_svc_cb_register(AON_RPC_SVC_MISC, misc_dispatch);
}

