#include "OffsetOperator.h"
#include "CGA.h"

namespace cga {

OffsetOperator::OffsetOperator(float offsetDistance) {
	this->offsetDistance = offsetDistance;
}

Object* OffsetOperator::apply(Object* obj, std::list<Object*>& stack) {
	return obj->offset(obj->_name, offsetDistance);
}

}
