#include "RoofHipOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

RoofHipOperator::RoofHipOperator(float angle) {
	this->name = "roofHip";
	this->angle = angle;
}

Shape* RoofHipOperator::apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	return shape->roofHip(shape->_name, angle);
}

}
