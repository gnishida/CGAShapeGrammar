#include "ShapeLOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

ShapeLOperator::ShapeLOperator(float frontWidth, float leftWidth) {
	this->frontWidth = frontWidth;
	this->leftWidth = leftWidth;
}

Shape* ShapeLOperator::apply(Shape* obj, std::list<Shape*>& stack) {
	return obj->shapeL(obj->_name, frontWidth, leftWidth);
}

}
