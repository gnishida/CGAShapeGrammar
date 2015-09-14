#include "CenterOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

CenterOperator::CenterOperator(int axesSelector) {
	this->name = "center";
	this->axesSelector = axesSelector;
}

Shape* CenterOperator::apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	shape->center(axesSelector);

	return shape;
}

}
