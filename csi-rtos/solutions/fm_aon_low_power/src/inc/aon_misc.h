#ifndef __AON_MISC_H_
#define __AON_MISC_H_

enum aon_rpc_misc_svc_en {
    MISC_FUNC_UNKNOWN      =  0U,
    MISC_FUNC_SET_CONTROL  =  1U,
    MISC_FUNC_GET_CONTROL  =  2U,
    MISC_FUNC_REGDUMP_CFG  =  3U,
};

#define RegDumpLen		8
#define NameDumpLen		20
#define RegWidth		4
#define BaseDumpLen		4
#define SizeDumpLen		4
#define EnDumpLen		7
#define MaxDumpLen		64

#define RD_NAME(attr)		((attr)->name_info.match_name)
#define RD_BASE(attr)		((attr)->base_info.match_base)
#define RD_EN(attr)			((attr)->en_info.match_en)
#define RD_SIZE(attr)		((attr)->size_info.match_size)

typedef struct {
		uint32_t dump_start;
		uint32_t dump_end;
		uint32_t info_start;
		uint32_t info_end;
		bool dump_flag;
} mapinfo_t;

struct aon_rd_name_info {
		const char match_name[RegDumpLen];
		char name[NameDumpLen];
};

struct aon_rd_base_info {
		const char match_base[BaseDumpLen];
		uint32_t base;

};

struct aon_rd_size_info {
		const char match_size[SizeDumpLen];
		uint32_t size;
};

struct aon_rd_en_info {
		const char match_en[EnDumpLen];
		bool en;
};

typedef struct {
		struct aon_rd_name_info name_info;
		struct aon_rd_base_info base_info;
		struct aon_rd_size_info size_info;
		struct aon_rd_en_info en_info;
} reginfo_t;

int aon_misc_init();
void aon_regdump_handler();
csi_error_t rd_set_mem(reginfo_t *RegInfo, uint32_t *dest_base, uint32_t dest_end);
csi_error_t rd_init(reginfo_t *RegInfo, mapinfo_t *Map, uint32_t start_addr, uint32_t cfg_offset, uint32_t size);
csi_error_t rd_read_cfg(reginfo_t *RegInfo, uint32_t *CfgAddr, uint32_t offset);


#endif
