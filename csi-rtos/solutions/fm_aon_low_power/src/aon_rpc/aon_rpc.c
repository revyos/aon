#include <stdint.h>
#include <drv/mbox.h>

#include "syslog.h"
#include "board_config.h"
#include "aon_common.h"
#include "aon_rpc.h"
#include "aon_lpm.h"

#ifndef CONFIG_RPC_QUEUE_SIZE
#define QUEUE_SIZE 5
#else
#define QUEUE_SIZE CONFIG_RPC_QUEUE_SIZE
#endif

#ifndef CONFIG_RPC_MAX_MSG_PROCESS
#define CONFIG_RPC_MAX_MSG_PROCESS 10
#endif

typedef struct {
        aon_rpc_svc_en  svc;
        rpc_svc_process func;
} rpc_process_t;

typedef struct {
        aon_rpc_msg_t items[QUEUE_SIZE];
        int           front;
        int           rear;
} rpc_queue;

const low_priority_svc_t low_priority_svc_list[] = {
	/*fix me: AON_RPC_SVC_LPM put in this will cause wakeup faild*/
    //{AON_RPC_SVC_LPM, LPM_FUNC_AON_REQUIRE_STR},
    {AON_RPC_SVC_LPM, LPM_FUNC_AON_CPUHP},
};

static uint8_t       rx_data[AON_RPC_MAX_LEN];
static int           target_ap_channel_id = EXAMPLE_MBOX_TARGET_CPU;
static int           init_flag            = 0;
static rpc_queue     g_rpc_quque;
static rpc_process_t g_rpc_process[CONFIG_RPC_MAX_MSG_PROCESS] = {0x0};

extern csi_mbox_t mbox;
void              aon_rpc_dispatch(void *data);

void initializeQueue(rpc_queue *cq)
{
        cq->front = 0;
        cq->rear  = 0;
}

bool isFull(rpc_queue *cq)
{
        if ((cq->front == cq->rear + 1) ||
            (cq->front == 0 && cq->rear == QUEUE_SIZE - 1)) {
                return true;
        }
        return false;
}

bool isEmpty(rpc_queue *cq)
{
        if (cq->front == cq->rear) {
                return true;
        }
        return false;
}

int enqueue(rpc_queue *cq, aon_rpc_msg_t *element)
{
        if (isFull(cq)) {
                LOG_E("\nQueue is full!\n");
                return -1;
        } else {
                memcpy(&cq->items[cq->rear], element, sizeof(aon_rpc_msg_t));
                cq->rear++;
                if (cq->rear >= QUEUE_SIZE) {
                        cq->rear = cq->rear - QUEUE_SIZE;
                }
        }
        return 0;
}

aon_rpc_msg_t *dequeue(rpc_queue *cq)
{
        aon_rpc_msg_t *element;
        if (isEmpty(cq)) {
                return NULL;
        } else {
                element = &cq->items[cq->front];
                cq->front++;
                if (cq->front >= QUEUE_SIZE) {
                        cq->front = cq->front - QUEUE_SIZE;
                }
                return (element);
        }
}

void dumpBuf(unsigned char *buf, int len)
{
        int i;

        for (i = 0; i < len; i += 4) {
                if (i % 16 == 0) {
                        printf("\n");
                        printf("0x%x  ", (unsigned int)(buf + i));
                }
                printf("Info[%d]:%02x %02x %02x %02x ", i / 4, buf[i],
                       buf[i + 1], buf[i + 2], buf[i + 3]);
        }
        printf("\n");
}

static void aon_low_power_isr(csi_mbox_t *mbox, csi_mbox_event_t event,
                              uint32_t channel_id, uint32_t received_len,
                              void *arg)
{
        if (event == MBOX_EVENT_RECEIVED) {
                csi_mbox_receive(mbox, channel_id, rx_data, received_len);
                /* In irq handler, there need 20us delay to send the message to AP, don't why ? */
                aon_rpc_dispatch((void *)rx_data);
        } else {
                LOG_E("ERR->Error event handle\n");
        }
}

static bool is_rpc_msg_high_priority(aon_rpc_svc_en svc_id, uint8_t func_id)
{

        for (int i = 0; i < ARRAY_SIZE(low_priority_svc_list); i++) {
                if (low_priority_svc_list[i].rpc == svc_id &&
                    low_priority_svc_list[i].func == func_id) {
                        return false;
                }
        }
        return true;
}

void aon_rpc_msg_process(aon_rpc_msg_t *rx_msg, aon_rpc_msg_t *tx_msg)
{
        for (int i = 0; i < ARRAY_SIZE(g_rpc_process); i++) {
                if (g_rpc_process[i].svc == RPC_GET_SVC_ID(rx_msg) &&
                    g_rpc_process[i].func) {
                        g_rpc_process[i].func(rx_msg, tx_msg);
                        return;
                }
        }

        LOG_E("ipc_err: bad service (%d)\n", RPC_GET_SVC_ID(rx_msg));
        dumpBuf((uint8_t *)rx_msg, 28);
        /* Fill in error response */
        RPC_SET_HEAD(tx_msg, AON_RPC_VERSION, 1, RPC_SVC_MSG_TYPE_ACK,
                     RPC_SVC_MSG_NO_NEED_ACK, RPC_GET_SVC_ID(rx_msg),
                     RPC_GET_FUNC(rx_msg), AON_ERR_NOTFOUND);
}

void aon_rpc_dispatch(void *data)
{
        aon_rpc_msg_t *rx_msg = data;
        aon_rpc_msg_t  tx_msg = {0};

        memset((uint8_t *)&tx_msg, 0x0, AON_RPC_MAX_LEN);

        if (RPC_GET_SIZE(rx_msg) == 0U) {
                LOG_E("ipc_err: bad size (%d)\n", RPC_GET_SIZE(rx_msg));
                RPC_SET_HEAD(&tx_msg, AON_RPC_VERSION, 1, RPC_SVC_MSG_TYPE_ACK,
                             RPC_SVC_MSG_NO_NEED_ACK, AON_RPC_SVC_UNKNOWN, 0,
                             AON_ERR_IPC);
                goto TX;
        }

        if (RPC_GET_VER(rx_msg) < AON_RPC_VERSION) {
                LOG_E("ipc_err: bad version (%d)\n", RPC_GET_VER(rx_msg));
                RPC_SET_HEAD(&tx_msg, AON_RPC_VERSION, 1, RPC_SVC_MSG_TYPE_ACK,
                             RPC_SVC_MSG_NO_NEED_ACK, RPC_GET_SVC_ID(rx_msg),
                             RPC_GET_FUNC(rx_msg), AON_ERR_VERSION);
                goto TX;
        }

        if (is_rpc_msg_high_priority(RPC_GET_SVC_ID(rx_msg),
                                     RPC_GET_FUNC(rx_msg)) == true) {
                aon_rpc_msg_process(rx_msg, &tx_msg);
        } else {
                enqueue(&g_rpc_quque, rx_msg);
                return;
        }

TX:
        /* write the response to AP side */
        if ((RPC_GET_SVC_FLAG_ACK_TYPE(rx_msg) == RPC_SVC_MSG_NEED_ACK) &&
            RPC_GET_SIZE(&tx_msg)) {
                RPC_SET_SVC_FLAG_MSG_TYPE(&tx_msg, RPC_SVC_MSG_TYPE_ACK);
                RPC_SET_SVC_FLAG_ACK_TYPE(&tx_msg, RPC_SVC_MSG_NO_NEED_ACK);
                /*4(head) + data*/
                csi_mbox_send(&mbox, target_ap_channel_id, &tx_msg,
                              4 + tx_msg.size);
        }
}

int aon_rpc_init()
{
        int ret = 0;
        LOG_D("mbox index:%d ap_channel_id:%d\n", EXAMPLE_MBOX_IDX,
              target_ap_channel_id);
        initializeQueue(&g_rpc_quque);

        /* register aon_low_power_isr */
        ret = csi_mbox_attach_callback(&mbox, aon_low_power_isr, NULL);
        if (ret != CSI_OK) {
                LOG_E("ERR->csi_mbox_attach_callback failed\n");
                return -1;
        }
        init_flag = 1;
        return 0;
}

int aon_rpc_svc_cb_register(aon_rpc_svc_en svc, rpc_svc_process func)
{

        if (!init_flag) {
                LOG_E("Call aon_rpc_init first\n");
                return -1;
        }
        int valid_index = -1;
        for (int i = 0; i < ARRAY_SIZE(g_rpc_process); i++) {
                if (g_rpc_process[i].svc == svc) {
                        g_rpc_process[i].func = func;
                        return 0;
                }
                if (valid_index == -1 && !g_rpc_process[i].func) {
                        valid_index = i;
                }
        }

        if (valid_index == -1) {
                LOG_E("svc cb register faild");
                return -1;
        }

        g_rpc_process[valid_index].svc  = svc;
        g_rpc_process[valid_index].func = func;

        return 0;
}

int aon_rpc_process_loop()
{
        if (!init_flag) {
                LOG_E("Call aon_rpc_init first\n");
                return -1;
        }

        aon_rpc_msg_t *rx_msg = NULL;
        aon_rpc_msg_t  tx_msg = {0};
        while (1) {
                rx_msg = dequeue(&g_rpc_quque);
                if (rx_msg) {
                        aon_rpc_msg_process(rx_msg, &tx_msg);
                        if ((RPC_GET_SVC_FLAG_ACK_TYPE(rx_msg) ==
                             RPC_SVC_MSG_NEED_ACK) &&
                            RPC_GET_SIZE(&tx_msg)) {
                                RPC_SET_SVC_FLAG_MSG_TYPE(&tx_msg,
                                                          RPC_SVC_MSG_TYPE_ACK);
                                RPC_SET_SVC_FLAG_ACK_TYPE(
                                    &tx_msg, RPC_SVC_MSG_NO_NEED_ACK);
                                csi_mbox_send(&mbox, target_ap_channel_id,
                                              &tx_msg, sizeof(aon_rpc_msg_t));
                        }
                }
        }
}

int aon_rpc_send(void *data, int len)
{
        if (!init_flag) {
                LOG_E("Call aon_rpc_init first\n");
                return -1;
        }
        return csi_mbox_send(&mbox, target_ap_channel_id, data, len);
}