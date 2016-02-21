#include "pheap_index.h"
#include <stdio.h>

#include "pop.h"

// Singleton pointer
PheapIndex* PheapIndex::singleton = 0;

PheapIndex* PheapIndex::instance() {
	PheapIndex* idx;
	if (ph_isnew()) {
		printf("Init'ing pheap...\n");
		struct pheap* my_pheap = get_pheap();
		printf("pheap_base=%p\n", my_pheap);
		idx = new PheapIndex;
		// since this is our first allocation, PheapIndex will be at the address of pheap->data
		printf("PheapIndex=%p\n", my_pheap->data);
	} else {
		printf("Loading pheap\n");
		struct pheap* my_pheap = get_pheap();
		printf("pheap_base=%p\n", my_pheap);
		idx = (PheapIndex*) my_pheap->data;
		printf("PheapIndex=%p\n", my_pheap->data);
	}
	return idx;
}

void PheapIndex::add_klass(char* klass_id, Klass* klass) {
	struct klass_index_node* node = (struct klass_index_node*) ph_malloc(sizeof(struct klass_index_node));
	node->next_node = NULL;
	node->id = klass_id;
	node->klass = klass;
	printf("adding to index klass id=%s at %p\n", klass_id, klass);
	
	if (klass_index_head == NULL) {
		// init linked list
		klass_index_head = node;
		klass_index_tail = node;
	} else {
		// add to existing linked list
		klass_index_tail->next_node = node;
		klass_index_tail = node;
	}
}

Klass* PheapIndex::get_class(char* id) {
	// check for empty index
	if (klass_index_head == NULL) {
		printf("index is uninitialized!");
		return NULL;
	}
	struct klass_index_node* node = klass_index_head;
	while (1) {
		if (strcmp(id, node->id) == 0) {	// is this the node we're looking for?
			printf("retrieving klass %s at %p\n", id, node->klass);
			return node->klass;
		}
		// get next node
		if (node->next_node != NULL) {
			node = node->next_node;
		} else {
			printf("id %s not found!", id);
			break;	// end of index and found nothing
		}
	}
	return NULL;
}

