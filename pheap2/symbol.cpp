#include "symbol.h"
#include <libpmemobj.h>

char* Symbol::get_name() {
	return name;
}

void Symbol::set_name(char* symbol_name) {
	//TX_BEGIN(get_pop()) {
		//pmemobj_tx_add_range_direct(&name, sizeof(char*));
		name = symbol_name;
		//pmemobj_persist(get_pop(), &class_name, sizeof (char*));
	//} TX_END
}
