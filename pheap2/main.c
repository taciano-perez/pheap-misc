#include <stdio.h>
#include "klass.hpp"
#include "pop.h"

/*
 * .
 */
int main(int argc, char *argv[])
{
	Klass* klass;
	Symbol* symbol;
	if (ph_isnew()) {
		printf("Init'ing heap...\n");
		struct pheap* my_pheap = get_pheap();
		printf("pheap=%p\n", my_pheap);
		klass = new Klass;
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
	} else {
		printf("Getting Klass* in a very dirty way...\n");
		struct pheap* my_pheap = get_pheap();
		printf("pheap=%p\n", my_pheap);
		klass = (Klass*) my_pheap->data;
		char* k_n = klass->get_class_name();
		printf("class name: %s\n", k_n);
		printf("symbol name: %s\n", klass->get_symbol()->get_name());
	}
	
	ph_close();
	
}

