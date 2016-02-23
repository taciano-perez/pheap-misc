#include "pop.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <libpmemobj.h>
#include <unistd.h>

POBJ_LAYOUT_BEGIN(pheap_layout);
POBJ_LAYOUT_ROOT(pheap_layout, struct pheap);
POBJ_LAYOUT_END(pheap_layout);

/*
 * (internal) function to get pool
 */
PMEMobjpool *get_pop() {
	if (!pop_initd) {
		if (access(PATH, F_OK) != 0) {
			// create persistent heap
			int pool_size = HEAP_SIZE+HEAP_SIZE;
			if (pool_size < PMEMOBJ_MIN_POOL) pool_size = PMEMOBJ_MIN_POOL;
			if ((pop = pmemobj_create(PATH, POBJ_LAYOUT_NAME(pheap_layout), pool_size, 0666)) == NULL) {
			
				perror("pmemobj_create");
				return NULL;
			}
			// get pheap pointer
			root = pmemobj_root(pop, sizeof(pheap));
			pheap = (struct pheap*) pmemobj_direct(root);
			// initialize pheap
			pheap->freespace = HEAP_SIZE;
			pheap->next_free = pheap->data;
			
			pop_isnew = true;
			pop_initd = true;
		} else {
			if ((pop = pmemobj_open(PATH, POBJ_LAYOUT_NAME(pheap_layout))) == NULL) {
				std::cerr << "failed to open pool" << std::endl;
				return NULL;
			}
			// get pheap pointer
			root = pmemobj_root(pop, sizeof(pheap));
			pheap = (struct pheap*) pmemobj_direct(root);

			pop_initd = true;
		}

	}
	return pop;
}

/*
 * Returns pheap
 */
struct pheap* get_pheap() {
	return pheap;
}

/*
 * allocation function
 */
void* ph_malloc(int len) {
	printf("freespace=%d\n", pheap->freespace);
	void* ret_addr;
	if (pheap->freespace < len) {
		// prevent heap overrun
		printf("error: pheap is out of space\n");
		return NULL;
	}
	//TX_BEGIN(get_pop()) {
		ret_addr = (void*)pheap->next_free;
		//pmemobj_tx_add_range_direct(&pheap->next_free, sizeof(char*));
		pheap->next_free += len;
		//pmemobj_tx_add_range_direct(&pheap->freespace, sizeof(int));
		pheap->freespace -= len;
		//pmemobj_tx_add_range_direct(ret_addr, sizeof(len));
		//TX_MEMSET((char*)ret_addr, 0, len);
		memset((char*)ret_addr, 0, len);
	//} TX_END
	
	return ret_addr;
}

bool ph_isnew() {
	get_pop();	// force init
	return pop_isnew;
}

void ph_close() {
	// clean up
	pmemobj_close(pop);

}

/*
 * Heap dump function
 */
int dump_heap(void) {
	printf("Base address: %p\n", pheap);
	printf("Free space: %d\n", pheap->freespace);
	printf("Next free: %p\n", pheap->next_free);
	printf("Offset:%lu\n", (unsigned long)pheap->next_free - (unsigned long)pheap->data);
	return 0;
}


//#define LAYOUT "klass"

