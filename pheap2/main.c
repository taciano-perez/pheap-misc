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
	
	idx = PheapIndex::instance();
	if (ph_isnew()) {
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
		// test heap limits
		while (1) {
			ph_malloc(1024);
		}
	} else {
		klass = idx->get_class((char*)"AB");
		if (klass == NULL) printf("Klass not found!");
		char* k_n = klass->get_class_name();
		printf("class name: %s\n", k_n);
		printf("symbol name: %s\n", klass->get_symbol()->get_name());
		klass2 = idx->get_class((char*)"BC");
	}
	
	ph_close();
	
}

