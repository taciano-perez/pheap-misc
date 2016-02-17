#ifndef POP_H
#define POP_H

#include <libpmemobj.h>

#define PATH "./pheap_file"
#define	HEAP_SIZE 16*1048576	// 16 MB

// structs
struct pheap {
	int freespace;				// free heap space (in bytes)
	// FIXME: can pmemobj_open be forced to map always to the same base address?
	char *next_free;			// pointer to next free allocation space
	char data[HEAP_SIZE];		// heap contents
};

// global variables
static PMEMobjpool *pop;		// PMEM object pool
static bool pop_initd = false;	// is pool initialized? (i.e., either created or opened)
static bool pop_isnew = false;	// is pool new? (i.e., created on this execution)
static PMEMoid root;			// root PMEMoid
static struct pheap *pheap;		// root direct pointer

// external interfaces
void* ph_malloc(int len);
int dump_heap(void);
bool ph_isnew();
struct pheap* get_pheap();
void ph_close();

// internal functions
PMEMobjpool* get_pop();

#endif		// POP_H
