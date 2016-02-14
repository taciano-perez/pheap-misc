#include <stdio.h>
#include <string.h>
#include <libpmemobj.h>

#define PATH "./pheap_file"

#define	HEAP_SIZE 1048576

struct pheap {
	int freespace;				// free heap space (in bytes)
	// FIXME: can pmemobj_open be forced to map always to the same base address?
	void *next_free;			// pointer to next free allocation space
	char data[HEAP_SIZE];		// heap contents
};

POBJ_LAYOUT_BEGIN(pheap_layout);
POBJ_LAYOUT_ROOT(pheap_layout, struct pheap);
POBJ_LAYOUT_END(pheap_layout);

static PMEMobjpool *pop;	// PMEM object pool
static PMEMoid root;		// root PMEMoid
static struct pheap *pheap;		// root direct pointer

/*
 * allocation function
 */
void* ph_malloc(int len) {
	void* ret_addr;
	TX_BEGIN(pop) {
		ret_addr = pheap->next_free;
		pheap->next_free += len;
		pmemobj_tx_add_range_direct(pheap->next_free, sizeof(void*));
		pheap->freespace -= len;
		pmemobj_tx_add_range_direct(&pheap->freespace, sizeof(int));
		TX_MEMSET((char*)ret_addr, 0, len);
	} TX_END
	
	return ret_addr;
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

/*
 * Simple heap example, without ability to reuse free space or compaction.
 */
int main(int argc, char *argv[])
{
	
	// create persistent heap
	int pool_size = sizeof(pheap);
	if (pool_size < PMEMOBJ_MIN_POOL) pool_size = PMEMOBJ_MIN_POOL;
	if ((pop = pmemobj_create(PATH, POBJ_LAYOUT_NAME(pheap_layout), pool_size, 0666)) == NULL) {
	
		perror("pmemobj_create");
		return 1;
	}

	// initialize heap
	root = pmemobj_root(pop, HEAP_SIZE);
	pheap = (struct pheap*) pmemobj_direct(root);
	pheap->freespace = HEAP_SIZE;
	pheap->next_free = pheap->data;
	
	// allocate example
	dump_heap();
	char* c1 = ph_malloc(12);
	dump_heap();
	char* c2 = ph_malloc(15);
	dump_heap();
	
	// clean up
	pmemobj_close(pop);
}

