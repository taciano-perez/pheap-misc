#ifndef KLASS_HPP
#define KLASS_HPP

#include <libpmemobj.h>

#include "pop.h"

class Klass {

	private:
	char* class_name;

	public:
	void *operator new(size_t  num_bytes) {
		void* result;
		TX_BEGIN(get_pop()) {
			result = pmemobj_direct(pmemobj_tx_alloc(num_bytes, 0));
		} TX_END
		return result;
	}
	char* get_class_name();
	void set_class_name(const char* klass_name);

};

#endif
