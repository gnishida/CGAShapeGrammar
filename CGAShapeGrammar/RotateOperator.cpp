#include "RotateOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

RotateOperator::RotateOperator(float xAngle, float yAngle, float zAngle) {
	this->name = "rotate";
	this->xAngle = xAngle;
	this->yAngle = yAngle;
	this->zAngle = zAngle;
}

Shape* RotateOperator::apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	shape->rotate(shape->_name, xAngle, yAngle, zAngle);
	return shape;
}

}
