#include "ExtrudeOperator.h"
#include "CGA.h"

namespace cga {

ExtrudeOperator::ExtrudeOperator(float height) {
	this->height = height;
}

Object* ExtrudeOperator::apply(Object* obj, std::list<Object*>& stack) {
	return obj->extrude(obj->_name, height);
}

}
