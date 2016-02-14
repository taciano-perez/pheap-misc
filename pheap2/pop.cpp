#include "pop.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <libpmemobj.h>
#include <unistd.h>

#define PATH "./pheap_file"
#define LAYOUT "klass"

PMEMobjpool *pop = NULL;


PMEMobjpool *get_pop() {
	if (pop == NULL) {
		if (access(PATH, F_OK) != 0) {
			if ((pop = pmemobj_create(PATH, LAYOUT,
				PMEMOBJ_MIN_POOL, S_IRWXU)) == NULL) {
				std::cerr << "failed to create pool" << std::endl;
				return NULL;
			}
		} else {
			if ((pop = pmemobj_open(PATH, LAYOUT)) == NULL) {
				std::cerr << "failed to open pool" << std::endl;
				return NULL;
			}
		}
	}
	return pop;
}
