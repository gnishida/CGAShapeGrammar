#include "OffsetOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

OffsetOperator::OffsetOperator(float offsetDistance) {
	this->offsetDistance = offsetDistance;
}

Shape* OffsetOperator::apply(Shape* obj, std::list<Shape*>& stack) {
	return obj->offset(obj->_name, offsetDistance);
}

}
