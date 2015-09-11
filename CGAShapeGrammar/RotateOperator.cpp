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

Shape* RotateOperator::apply(Shape* obj, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	obj->rotate(obj->_name, xAngle, yAngle, zAngle);
	return obj;
}

}
