#ifndef PHEAP_INDEX_H
#define PHEAP_INDEX_H

#include "klass.hpp"
#include <libpmemobj.h>

// structs
struct klass_index_node {
	klass_index_node* next_node;	// pointer to next node
	char* id;
	Klass* klass;		// klass stored in current node
};

class PheapIndex {	

	private:
	struct klass_index_node* klass_index_head;
	struct klass_index_node* klass_index_tail;
	static PheapIndex* singleton;

	public:
	void *operator new(size_t  num_bytes) {
		void* result;
		// TX_BEGIN(get_pop()) {
			// result = pmemobj_direct(pmemobj_tx_alloc(num_bytes, 0));
		// } TX_END
		result = ph_malloc(num_bytes);
		return result;
	}

	void add_klass(char* klass_id, Klass* klass);
	Klass* get_class(char* id);
	static PheapIndex* instance();

};

#endif		// PHEAP_INDEX_H
