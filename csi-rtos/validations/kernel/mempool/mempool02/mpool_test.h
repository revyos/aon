#include <csi_kernel.h>
#include <stdio.h>


#ifndef MM_BLK_TEST_H
#define MM_BLK_TEST_H

#define CSI_TEST_MPOOL_PRIO  CSI_TEST_MAX_PRIO-1
#define CSI_TEST_MPOOL_MIN_PRIO  CSI_TEST_MIN_PRIO+1

#define CSI_TEST_MPOOL_STACK   1024

#define MYASSERT(value) do {ASSERT_TRUE(value);  if((int)(value) == 0) {printf("ERROR: %s:%d, ERRO    R-NO: %d\n", __FUNCTION__, __LINE__, value); }} while (0)

//extern k_mpool_handle_t mblk_pool_test;
//extern char mblk_pool[MBLK_POOL_SIZE];

//typedef uint8_t (*test_func_t)(void);


struct handle_status_check {
	void        *handle;
	k_status_t  status;
	int         equal;
};

typedef struct handle_status_check handle_status_check_t;

void mpool_new_interface_test();
void mpool_del_interface_test();
void mpool_alloc_interface_test();
void mpool_free_interface_test();
void mpool_func_test();

#endif  /* MM_BLK_TEST_H */
