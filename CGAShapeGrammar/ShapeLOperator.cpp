#include "ShapeLOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

ShapeLOperator::ShapeLOperator(float frontWidth, float leftWidth) {
	this->name = "shapeL";
	this->frontWidth = frontWidth;
	this->leftWidth = leftWidth;
}

Shape* ShapeLOperator::apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	return shape->shapeL(shape->_name, frontWidth, leftWidth);
}

}
