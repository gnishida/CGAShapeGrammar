#include "ShapeLOperator.h"
#include "CGA.h"

namespace cga {

ShapeLOperator::ShapeLOperator(float frontWidth, float leftWidth) {
	this->frontWidth = frontWidth;
	this->leftWidth = leftWidth;
}

Object* ShapeLOperator::apply(Object* obj, std::list<Object*>& stack) {
	return obj->shapeL(obj->_name, frontWidth, leftWidth);
}

}
