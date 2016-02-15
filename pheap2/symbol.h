#ifndef SYMBOL_H
#define SYMBOL_H

#include <libpmemobj.h>

#include "pop.h"

class Symbol {

	private:
	char* name;

	public:
	void *operator new(size_t  num_bytes) {
		void* result;
		// TX_BEGIN(get_pop()) {
			// result = pmemobj_direct(pmemobj_tx_alloc(num_bytes, 0));
		// } TX_END
		result = ph_malloc(num_bytes);
		return result;
	}
	char* get_name();
	void set_name(char* symbol_name);

};

#endif		// SYMBOL_H
