#include <csi_kernel.h>
#include <stdio.h>
//#include <csi_config.h>

#ifndef TASK_TEST_H
#define TASK_TEST_H

#define MYASSERT(value) do {ASSERT_TRUE(value); if ((int)(value) == 0) \
	    { \
		    test_case_critical_enter(); \
            test_case_check_err++;  \
            test_case_critical_exit(); \
		    printf("task test is [FAIL %d], func %s, line %d\n", \
	                ++test_case_check_err, __FUNCTION__, __LINE__); \
		}  \
	} while (0)

#define CSI_TEST_TASK_MAX_PRIO  KPRIO_HIGH0  
#define CSI_TEST_TASK_MIN_PRIO  KPRIO_LOW7
#define CSI_OVER_MAX_PRIO        70

#define CSI_TEST_TASK_STACK    768

struct handle_status_check {
	void        *handle;
	k_status_t  status;
	int         equal;
};

typedef struct handle_status_check handle_status_check_t;

typedef struct task_null_check {
    k_task_entry_t       task;
    char                 *name;
    void                 *arg;
    k_priority_t           prio;
    uint32_t      time_quanta;
    void               *stack;
    uint32_t       stack_size;
    int             nullcheck;
} task_null_check_t;

typedef struct task_new_check {
    k_task_entry_t     task;
    char             *name;
    void               *arg;
    k_priority_t         prio;
    uint32_t    time_quanta;
    void             *stack;
    uint32_t     stack_size;
} task_new_check_t;

typedef struct task_del_check {
    k_task_handle_t handle;
    k_status_t      ret;
    char            *casename;
} task_del_check_t;

void task_new_interface_test();
void task_del_interface_test();
void task_get_stat_interface_test();
void task_set_prio_interface_test();
void task_get_prio_interface_test();
void task_suspend_interface_test();
void task_terminate_interface_test();
void task_resume_interface_test();
void task_list_interface_test();
void create_task_test();
void delete_task_test();
void exit_task_test();
void get_task_count_test();
void get_current_handle_test();
void get_task_stat_test();
void list_task_test();
void resume_task_test();
void set_get_task_prio_test();
void suspend_task_test();
void task_quanta_test();
void terminate_task_test();
void yield_task_test();

#endif /* TASK_TEST_H */
