#include "RoofGableOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

RoofGableOperator::RoofGableOperator(const std::string& angle) {
	this->name = "roofGable";
	this->angle = angle;
}

Shape* RoofGableOperator::apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	float actual_angle = ruleSet.evalFloat(angle, shape);
	return shape->roofGable(shape->_name, actual_angle);
}

}
