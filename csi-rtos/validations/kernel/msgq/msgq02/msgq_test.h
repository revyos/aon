#include <csi_kernel.h>
#include <stdio.h>
#include <stdlib.h>     //add this headfile in order to eliminate the warning for using malloc

#define CSI_TEST_MSGQ_PRIO  KPRIO_HIGH0
#define CSI_TEST_MSGQ_MIN_PRIO  KPRIO_LOW7

#define CSI_TEST_MSGQ_STACK    1024

#ifndef BUF_QUEUE_TEST_H
#define BUF_QUEUE_TEST_H

#define BUFQUEUE_VAL_CHK(value) do {ASSERT_TRUE(value); if ((int)(value) == 0) \
	    { \
            test_case_critical_enter(); \
			test_case_check_err++;  \
			test_case_critical_exit(); \
			printf("message queue test is [FAIL %d], func %s, line %d\n", \
					++test_case_check_err, __FUNCTION__, __LINE__); \
		}  \
	 } while (0)

struct handle_status_check {
	void        *handle;
	k_status_t  status;
	int         equal;
};

typedef struct handle_status_check handle_status_check_t;


void msgq_new_interface_test();
void msgq_del_interface_test();
void msgq_put_interface_test();
void msgq_get_interface_test();
void msgq_get_count_interface_test();
void msgq_flush_interface_test();
void msgq_func_test();
void msgq_func_test2();
void msgq_func_test3();
void msgq_func_test4();

#endif
