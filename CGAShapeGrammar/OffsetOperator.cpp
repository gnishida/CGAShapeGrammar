#include "OffsetOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

OffsetOperator::OffsetOperator(const std::string& offsetDistance, int offsetSelector) {
	this->name = "offset";
	this->offsetDistance = offsetDistance;
	this->offsetSelector = offsetSelector;
}

Shape* OffsetOperator::apply(Shape* shape, const RuleSet& ruleSet,  std::list<Shape*>& stack) {
	float actual_offsetDistancet = ruleSet.evalFloat(offsetDistance, shape);

	return shape->offset(shape->_name, actual_offsetDistancet, offsetSelector);
}

}
