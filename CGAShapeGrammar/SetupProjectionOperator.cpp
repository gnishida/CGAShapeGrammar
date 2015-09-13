#include "SetupProjectionOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

SetupProjectionOperator::SetupProjectionOperator(int axesSelector, const Value& texWidth, const Value& texHeight) {
	this->name = "setupProjection";
	this->axesSelector = axesSelector;
	this->texWidth = texWidth;
	this->texHeight = texHeight;
}

Shape* SetupProjectionOperator::apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	float actual_texWidth;
	float actual_texHeight;

	if (texWidth.type == Value::TYPE_RELATIVE) {
		actual_texWidth = shape->_scope.x * ruleSet.evalFloat(texWidth.value, shape);
	} else {
		actual_texWidth = ruleSet.evalFloat(texWidth.value, shape);
	}
	if (texHeight.type == Value::TYPE_RELATIVE) {
		actual_texHeight = shape->_scope.x * ruleSet.evalFloat(texHeight.value, shape);
	} else {
		actual_texHeight = ruleSet.evalFloat(texHeight.value, shape);
	}


	shape->setupProjection(axesSelector, actual_texWidth, actual_texHeight);

	return shape;
}

}
