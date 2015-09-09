#include "ExtrudeOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

ExtrudeOperator::ExtrudeOperator(float height) {
	this->height = height;
}

Shape* ExtrudeOperator::apply(Shape* obj, std::list<Shape*>& stack) {
	return obj->extrude(obj->_name, height);
}

}
