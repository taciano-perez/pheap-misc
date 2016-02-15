#include "klass.hpp"
#include <libpmemobj.h>

char* Klass::get_class_name() {
	return class_name;
}

void Klass::set_class_name(char* klass_name) {
	TX_BEGIN(get_pop()) {
		pmemobj_tx_add_range_direct(&class_name, sizeof(char*));
		class_name = klass_name;
		//pmemobj_persist(get_pop(), &class_name, sizeof (char*));
	} TX_END
}

Symbol* Klass::get_symbol() {
		return symbol;
}

void Klass::set_symbol(Symbol* s) {
	TX_BEGIN(get_pop()) {
		pmemobj_tx_add_range_direct(&symbol, sizeof(Symbol*));
		symbol = s;
		//pmemobj_persist(get_pop(), &class_name, sizeof (char*));
	} TX_END
}
