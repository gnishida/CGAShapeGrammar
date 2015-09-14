#include "RoofHipOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

RoofHipOperator::RoofHipOperator(const std::string& angle) {
	this->name = "roofHip";
	this->angle = angle;
}

Shape* RoofHipOperator::apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	float actual_angle = ruleSet.evalFloat(angle, shape);
	return shape->roofHip(shape->_name, actual_angle);
}

}
