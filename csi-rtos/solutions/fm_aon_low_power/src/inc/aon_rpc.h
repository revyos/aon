#ifndef __RPC_SVC_H_
#define __RPC_SVC_H_

#include <stdint.h>

#ifndef CONFIG_AON_RPC_VERSION
#define AON_RPC_VERSION          2U
#endif

#define AON_RPC_MAX_LEN          28

#define RPC_SVC_MSG_TYPE_DATA     0
#define RPC_SVC_MSG_TYPE_ACK      1
#define RPC_SVC_MSG_NEED_ACK      0
#define RPC_SVC_MSG_NO_NEED_ACK   1

#define RPC_GET_VER(MESG)                     ((MESG)->version)
#define RPC_SET_VER(MESG, VER)                ((MESG)->version = (VER))
#define RPC_GET_SIZE(MESG)                    ((MESG)->size)
#define RPC_SET_SIZE(MESG, SIZE)                   ((MESG)->size = (SIZE))
#define RPC_GET_SVC_ID(MESG)             ((MESG)->svc & 0x3F)
#define RPC_SET_SVC_ID(MESG, ID)         ((MESG)->svc |= 0x3F & (ID))
#define RPC_GET_SVC_FLAG_MSG_TYPE(MESG)      (((MESG)->svc & 0x80) >> 7)
#define RPC_SET_SVC_FLAG_MSG_TYPE(MESG, TYPE)      ((MESG)->svc |= (TYPE) << 7)
#define RPC_GET_SVC_FLAG_ACK_TYPE(MESG)      (((MESG)->svc & 0x40) >> 6)
#define RPC_SET_SVC_FLAG_ACK_TYPE(MESG, ACK)      ((MESG)->svc |= (ACK) << 6)
#define RPC_GET_FUNC(MESG)          ((MESG)->func)
#define RPC_SET_FUNC(MESG, FUNC)          ((MESG)->func = (FUNC))
#define RPC_SET_ERR_CODE(MESG, ERR)      ((MESG)->data[0] = (ERR))



#define RPC_SET_BE64(MESG, OFFSET, SET_DATA)                                                                                  do {uint8_t* data = (uint8_t*)(MESG);       \
                                                                                                                               data[OFFSET + 7]  =     (SET_DATA) & 0xFF;  \
                                                                                                                               data[OFFSET + 6]  = ((SET_DATA) & 0xFF00) >> 8; \
                                                                                                                               data[OFFSET + 5]  = ((SET_DATA) & 0xFF0000) >> 16; \
                                                                                                                               data[OFFSET + 4]  = ((SET_DATA) & 0xFF000000) >> 24; \
                                                                                                                               data[OFFSET + 3]  = ((SET_DATA) & 0xFF00000000) >> 32; \
                                                                                                                               data[OFFSET + 2]  = ((SET_DATA) & 0xFF0000000000) >> 40; \
                                                                                                                               data[OFFSET + 1]  = ((SET_DATA) & 0xFF000000000000) >> 48; \
                                                                                                                               data[OFFSET + 0]  = ((SET_DATA) & 0xFF00000000000000) >> 56; \
                                                                                                                              } while(0)

#define RPC_SET_BE32(MESG, OFFSET, SET_DATA)                                                                          do { uint8_t* data = (uint8_t*)(MESG);    \
                                                                                                                               data[OFFSET +  3]  =     (SET_DATA) & 0xFF;  \
                                                                                                                               data[OFFSET + 2]  = ((SET_DATA) & 0xFF00) >> 8; \
                                                                                                                               data[OFFSET + 1]  = ((SET_DATA) & 0xFF0000) >> 16; \
                                                                                                                               data[OFFSET + 0]  = ((SET_DATA) & 0xFF000000) >> 24; \
                                                                                                                              } while(0)
#define RPC_SET_BE16(MESG, OFFSET, SET_DATA)                                                                          do { uint8_t* data = (uint8_t*)(MESG);   \
                                                                                                                               data[OFFSET  + 1]  = (SET_DATA) & 0xFF; \
                                                                                                                               data[OFFSET + 0]  = ((SET_DATA) & 0xFF00) >> 8; \
                                                                                                                          } while(0)
#define RPC_SET_U8(MESG, OFFSET, SET_DATA)                                                                          do { uint8_t* data = (uint8_t*)(MESG);  \
                                                                                                                         data[OFFSET]  = (SET_DATA) & 0xFF; \
																														                                                           } while(0)
#define RPC_GET_BE64(MESG, OFFSET, PTR)      do {uint8_t* data = (uint8_t*)(MESG); \
                                                *(uint32_t*)(PTR) = (data[OFFSET + 7] | data[OFFSET + 6] << 8 | data[OFFSET  + 5] << 16 | data[OFFSET + 4] << 24 | \
								     data[OFFSET + 3] << 32 | data[OFFSET + 2] << 40 | data[OFFSET + 1] << 48 | data[OFFSET + 0] << 56); \
											 } while(0)
#define RPC_GET_BE32(MESG, OFFSET, PTR)      do {uint8_t* data = (uint8_t*)(MESG); \
                                                *(uint32_t*)(PTR) = (data[OFFSET + 3] | data[OFFSET + 2] << 8 | data[OFFSET  + 1] << 16 | data[OFFSET + 0] << 24); \
											 } while(0)
#define RPC_GET_BE16(MESG, OFFSET, PTR)      do {uint8_t* data = (uint8_t*)(MESG);  \
                                                *(uint16_t*)(PTR) = (data[OFFSET  + 1] | data[OFFSET + 0] << 8); \
												} while(0)
#define RPC_GET_U8(MESG, OFFSET, PTR)       do {uint8_t* data = (uint8_t*)(MESG); \
                                               *(uint8_t*)(PTR) = (data[OFFSET]);  \
                                                } while(0)

#define RPC_SET_HEAD(MESG,VER,SIZE,TYPE,ACK,ID,FUNC,ERR)              {RPC_SET_VER(MESG,VER);RPC_SET_SIZE(MESG,SIZE);RPC_SET_SVC_FLAG_MSG_TYPE(MESG, TYPE);   \
                                                                       RPC_SET_SVC_FLAG_ACK_TYPE(MESG, ACK);RPC_SET_SVC_ID(MESG, ID);RPC_SET_FUNC(MESG, FUNC);  \
                                                                       RPC_SET_ERR_CODE(MESG, ERR);}
typedef struct __packed
{
	uint8_t version;
	uint8_t size;
	uint8_t svc;
	uint8_t func;
  uint8_t data[AON_RPC_MAX_LEN - 4];
	// union
	// {
	// 	uint32_t u32[(AON_RPC_MAX_MSG - 1U)];
	// 	uint16_t u16[(AON_RPC_MAX_MSG - 1U) * 2U];
	// 	uint8_t u8[(AON_RPC_MAX_MSG - 1U) * 4U];
	// } DATA;
} aon_rpc_msg_t;

typedef enum  {
  AON_RPC_SVC_UNKNOWN            = 0U,
  AON_RPC_SVC_PM                 = 1U,
  AON_RPC_SVC_MISC               = 2U,
  AON_RPC_SVC_AVFS               = 3U,
  AON_RPC_SVC_SYS                = 4U,
  AON_RPC_SVC_WDG                = 5U,
  AON_RPC_SVC_LPM                = 6U,

  AON_RPC_SVC_MAX                = 0xff,
}aon_rpc_svc_en;

struct pmic_info {
    char vendor_name[20];
    char device_name[20];
};

typedef struct  {
   aon_rpc_svc_en rpc;
   uint8_t        func;
} low_priority_svc_t;


typedef void (*rpc_svc_process)(aon_rpc_msg_t* msg, aon_rpc_msg_t* tx_msg);

int aon_rpc_init();

int aon_rpc_svc_cb_register(aon_rpc_svc_en svc, rpc_svc_process func);

int aon_rpc_process_loop();

int aon_rpc_send(void* data, int len);


#endif
