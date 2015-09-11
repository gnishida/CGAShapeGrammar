#include "SizeOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

SizeOperator::SizeOperator(const SingleValue& xSize, const SingleValue& ySize, const SingleValue& zSize) {
	this->name = "size";
	this->xSize = xSize;
	this->ySize = ySize;
	this->zSize = zSize;
}

Shape* SizeOperator::apply(Shape* obj, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	float actual_xSize;
	float actual_ySize;
	float actual_zSize;

	if (xSize.type == Value::TYPE_RELATIVE) {
		actual_xSize = obj->_scope.x * ruleSet.evalFloat(xSize.value, obj);
	} else {
		actual_xSize = ruleSet.evalFloat(xSize.value, obj);
	}

	if (ySize.type == Value::TYPE_RELATIVE) {
		actual_ySize = obj->_scope.y * ruleSet.evalFloat(ySize.value, obj);
	} else {
		actual_ySize = ruleSet.evalFloat(ySize.value, obj);
	}

	if (zSize.type == Value::TYPE_RELATIVE) {
		actual_zSize = obj->_scope.z * ruleSet.evalFloat(zSize.value, obj);
	} else {
		actual_zSize = ruleSet.evalFloat(zSize.value, obj);
	}

	obj->size(actual_xSize, actual_ySize, actual_zSize);
	return obj;
}

}
