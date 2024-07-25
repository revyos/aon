#include <csi_kernel.h>
#include <stdio.h>

#ifndef SEM_TEST_H
#define SEM_TEST_H


#define CSI_TEST_SEM_PRIO  KPRIO_HIGH0
#define CSI_TEST_SEM_MIN_PRIO  KPRIO_LOW7

#define CSI_TEST_SEM_STACK    1024


#define MYASSERT(value) do {ASSERT_TRUE(value); if ((int)(value) == 0) \
	 { \
		 test_case_critical_enter(); \
		 test_case_check_err++;  \
		 test_case_critical_exit(); \
		 printf("semaphore test is [FAIL %d], func %s, line %d\n", \
             ++test_case_check_err, __FUNCTION__, __LINE__); \
	 }  \
  } while (0)

struct handle_status_check {
	  void        *handle;
	  k_status_t  status;
	  int         equal;
} ;

typedef struct handle_status_check handle_status_check_t;

void sem_new_interface_test();
void sem_del_interface_test();
void sem_post_interface_test();
void sem_wait_interface_test();
void sem_func_test();
void sem_func_test2();
//void sem_func_test3();


#endif /* SEM_TEST_H */
