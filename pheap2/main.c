#include <stdio.h>
#include "klass.hpp"
#include "pop.h"
#include "pheap_index.h"

/*
 * .
 */
int main(int argc, char *argv[])
{
	PheapIndex* idx;
	Klass* klass;
	Klass* klass2;
	Symbol* symbol;
	if (ph_isnew()) {
		printf("Init'ing heap...\n");
		struct pheap* my_pheap = get_pheap();
		printf("pheap=%p\n", my_pheap);
		idx = new PheapIndex;
		klass = new Klass;
		printf("klass=%p\n", klass);
		klass2 = new Klass;
		printf("klass2=%p\n", klass2);
		char* class_name = (char*) ph_malloc(3);
		strcpy(class_name, "AB");
		klass->set_class_name(class_name);
		char* k_n = klass->get_class_name();
		printf("class name: %s\n", k_n);
		symbol = new Symbol;
		char* symbol_name = (char*) ph_malloc(3);
		strcpy(symbol_name, "BC");
		symbol->set_name(symbol_name);
		klass->set_symbol(symbol);
		idx->add_klass(class_name, klass);
		idx->add_klass(symbol_name, klass2);
	} else {
		printf("Getting PheapIndex* in a very dirty way...\n");
		struct pheap* my_pheap = get_pheap();
		printf("pheap=%p\n", my_pheap);
		//klass = (Klass*) my_pheap->data;
		idx = (PheapIndex*) my_pheap->data;
		klass = idx->get_class((char*)"AB");
		if (klass == NULL) printf("Klass not found!");
		char* k_n = klass->get_class_name();
		printf("class name: %s\n", k_n);
		printf("symbol name: %s\n", klass->get_symbol()->get_name());
		klass2 = idx->get_class((char*)"BC");
	}
	
	ph_close();
	
}

