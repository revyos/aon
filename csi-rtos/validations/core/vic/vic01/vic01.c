/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

#include <stdio.h>
#include "dtest.h"
#include "test_device.h"

#define USER1_INT_PRIO   3
#define USER2_INT_PRIO   2
#define USER3_INT_PRIO   1
#define VIC_THRESHOLD   0
#define USER1_IRQ        3
#define USER2_IRQ        4
#define USER3_IRQ        5
static volatile int tick_count;
extern uint32_t csi_vic_get_wakeup_irq(int32_t IRQn);
extern void csi_vic_set_threshold(uint32_t VectThreshold, uint32_t PrioThreshold);
extern void csi_vic_set_wakeup_irq(int32_t IRQn);
extern void csi_vic_clear_wakeup_irq(int32_t IRQn);

void user1_int_irq_handler(void)
{
    int VIC_Pend = 0;
    int VIC_Active = 0;
    int VIC_wakeup1 = 0;
    printf("--------start user1 irq handler!\n");

    VIC_Active = csi_vic_get_active(USER1_IRQ);
    ASSERT_TRUE(VIC_Active);

    if (VIC_Active) {
        printf("User1  VIC_Active passed!\n");
    }

    VIC_wakeup1 = csi_vic_get_wakeup_irq(USER1_IRQ);
    ASSERT_TRUE(VIC_wakeup1);

    if (VIC_wakeup1) {
        printf("User1 VIC_SetWakeupIRQ passed!\n");
    }

    csi_vic_clear_pending_irq(USER1_IRQ);
    csi_vic_disable_irq(USER1_IRQ);

    __disable_irq();
    csi_vic_set_pending_irq(USER2_IRQ);

    VIC_Pend = csi_vic_get_pending_irq(USER2_IRQ);
    __enable_irq();
    ASSERT_TRUE(VIC_Pend);

    if (VIC_Pend) {
        printf("User2 VIC_SetPending passed!\n");
    }

    printf("--------User1 irq handle passed!\n");
}

void user2_int_irq_handler(void)
{
    int VIC_Pend = 0;
    int VIC_Active = 0;
    int VIC_wakeup2 = 0;
    printf("--------start user2 irq handler!\n");

    VIC_Active = csi_vic_get_active(USER2_IRQ);
    ASSERT_TRUE(VIC_Active);

    if (VIC_Active) {
        printf("User2  VIC_Active passed!\n");
    }

    VIC_wakeup2 = csi_vic_get_wakeup_irq(USER2_IRQ);
    ASSERT_TRUE(VIC_wakeup2);

    if (VIC_wakeup2) {
        printf("User2 VIC_SetWakeupIRQ passed!\n");
    }

#if __CK80X < 3
    csi_vic_set_threshold(USER2_IRQ, VIC_THRESHOLD);
#endif
    csi_vic_clear_pending_irq(USER2_IRQ);
    csi_vic_disable_irq(USER2_IRQ);

    __disable_irq();
    csi_vic_set_pending_irq(USER3_IRQ);

    VIC_Pend = csi_vic_get_pending_irq(USER3_IRQ);
    __enable_irq();
    ASSERT_TRUE(VIC_Pend);

    if (VIC_Pend) {
        printf("User3 VIC_SetPending passed!\n");
    }

    printf("--------User2 irq handle passed!\n");
}
void user3_int_irq_handler(void)
{
    int VIC_Active = 0;
    int VIC_wakeup3 = 0;
    printf("--------Start user3 irq handler!\n");

    VIC_Active = csi_vic_get_active(USER3_IRQ);
    ASSERT_TRUE(VIC_Active);

    if (VIC_Active) {
        printf("User3  VIC_Active passed!\n");
    }

    VIC_wakeup3 = csi_vic_get_wakeup_irq(USER3_IRQ);
    ASSERT_TRUE(VIC_wakeup3);

    if (VIC_wakeup3) {
        printf("User3 VIC_SetWakeupIRQ passed!\n");
    }

    csi_vic_clear_pending_irq(USER3_IRQ);
    csi_vic_disable_irq(USER3_IRQ);
    printf("--------User3 irq handle passed!\n");
}


void user1_int(IRQn_Type IRQn, uint32_t priority)
{
    int VIC_Priority = 0;
    printf("Testing User1 int  function!\n");
    csi_vic_set_prio(IRQn, priority);
    csi_vic_set_wakeup_irq(IRQn);
    csi_vic_enable_irq(IRQn);
    VIC_Priority = csi_vic_get_prio(IRQn);
    ASSERT_TRUE(VIC_Priority == USER1_INT_PRIO);

    if (VIC_Priority == USER1_INT_PRIO) {
        printf("User1 VIC_SetPriority IRQ passed!\n");
    }

    printf("----------------------------------------\n");
}

void user2_int(IRQn_Type IRQn, uint32_t priority)
{
    int VIC_Priority = 0;
    printf("Testing User2 int  function!\n");
    csi_vic_set_prio(IRQn, priority);
    csi_vic_set_wakeup_irq(IRQn);
    csi_vic_enable_irq(IRQn);
    VIC_Priority = csi_vic_get_prio(IRQn);
    ASSERT_TRUE(VIC_Priority == USER2_INT_PRIO);

    if (VIC_Priority == USER2_INT_PRIO) {
        printf("User2 VIC_SetPriority IRQ passed!\n");
    }

    printf("----------------------------------------\n");
}

void user3_int(IRQn_Type IRQn, uint32_t priority)
{
    int VIC_Priority = 0;
    printf("Testing User3 int  function!\n");
    csi_vic_set_prio(IRQn, priority);
    csi_vic_set_wakeup_irq(IRQn);
    csi_vic_enable_irq(IRQn);
    VIC_Priority = csi_vic_get_prio(IRQn);
    ASSERT_TRUE(VIC_Priority == USER3_INT_PRIO);

    if (VIC_Priority == USER3_INT_PRIO) {
        printf("User3 VIC_SetPriority IRQ passed!\n");
    }

    printf("----------------------------------------\n");
}
int test_vic_test(void)
{
    int VIC_Priority;
    int VIC_Pend = 0;
    int VIC_wakeup = 1;
    int psr = 0;

    printf("-*************Testing functions VIC function********************\n");
    __enable_irq();
    __set_PSR(0x80000140);
    psr =  __get_PSR();
    printf("the psr is %x\r\n", psr);
    csi_coret_config(10000, CORET_IRQn);
    csi_vic_enable_irq(CORET_IRQn);

    while (1) {
        if (tick_count > 10000) {

            csi_vic_set_pending_irq(USER1_IRQ);
            VIC_Pend = csi_vic_get_pending_irq(USER1_IRQ);
            ASSERT_TRUE(VIC_Pend);

            if (VIC_Pend) {
                printf("User1 VIC_SetPending passed!\n");
            }

            break;
        }
    }

    VIC_Priority = csi_vic_get_prio(CORET_IRQn);
    printf("CORET_INT Prioriyt =%d \n", VIC_Priority);
    csi_vic_disable_irq(CORET_IRQn);
    user3_int(USER3_IRQ, USER3_INT_PRIO);
    user2_int(USER2_IRQ, USER2_INT_PRIO);
    user1_int(USER1_IRQ, USER1_INT_PRIO);

    csi_vic_clear_wakeup_irq(USER1_IRQ);
    VIC_wakeup = csi_vic_get_wakeup_irq(USER1_IRQ);
    ASSERT_TRUE(!VIC_wakeup);

    if (!VIC_wakeup) {
        printf("User1 VIC_ClearWakeupIRQ passed!\n");
    }

    VIC_wakeup = 1;

    csi_vic_clear_wakeup_irq(USER2_IRQ);
    VIC_wakeup = csi_vic_get_wakeup_irq(USER2_IRQ);
    ASSERT_TRUE(!VIC_wakeup);

    if (!VIC_wakeup) {
        printf("User2 VIC_ClearWakeupIRQ passed!\n");
    }

    VIC_wakeup = 1;

    csi_vic_clear_wakeup_irq(USER3_IRQ);
    VIC_wakeup = csi_vic_get_wakeup_irq(USER3_IRQ);

    ASSERT_TRUE(!VIC_wakeup);

    if (!VIC_wakeup) {
        printf("User3 VIC_ClearWakeupIRQ passed!\n");
    }

    return 0;
}

int main(void)
{
    return test_vic_test();
}
