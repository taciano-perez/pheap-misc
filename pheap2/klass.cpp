#include "klass.hpp"

char* Klass::get_class_name() {
	return class_name;
}

void Klass::set_class_name(const char* klass_name) {
	class_name = klass_name;
}
