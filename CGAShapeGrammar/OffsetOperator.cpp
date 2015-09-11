#include "OffsetOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

OffsetOperator::OffsetOperator(float offsetDistance) {
	this->name = "offset";
	this->offsetDistance = offsetDistance;
}

Shape* OffsetOperator::apply(Shape* shape, const RuleSet& ruleSet,  std::list<Shape*>& stack) {
	return shape->offset(shape->_name, offsetDistance);
}

}
