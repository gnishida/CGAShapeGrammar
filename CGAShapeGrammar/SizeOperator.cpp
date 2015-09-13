#include "SizeOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

SizeOperator::SizeOperator(const Value& xSize, const Value& ySize, const Value& zSize) {
	this->name = "size";
	this->xSize = xSize;
	this->ySize = ySize;
	this->zSize = zSize;
}

Shape* SizeOperator::apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	float actual_xSize;
	float actual_ySize;
	float actual_zSize;

	if (xSize.type == Value::TYPE_RELATIVE) {
		actual_xSize = shape->_scope.x * ruleSet.evalFloat(xSize.value, shape);
	} else {
		actual_xSize = ruleSet.evalFloat(xSize.value, shape);
	}

	if (ySize.type == Value::TYPE_RELATIVE) {
		actual_ySize = shape->_scope.y * ruleSet.evalFloat(ySize.value, shape);
	} else {
		actual_ySize = ruleSet.evalFloat(ySize.value, shape);
	}

	if (zSize.type == Value::TYPE_RELATIVE) {
		actual_zSize = shape->_scope.z * ruleSet.evalFloat(zSize.value, shape);
	} else {
		actual_zSize = ruleSet.evalFloat(zSize.value, shape);
	}

	shape->size(actual_xSize, actual_ySize, actual_zSize);
	return shape;
}

}
