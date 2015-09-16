#include "RoofHipOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

RoofHipOperator::RoofHipOperator(const std::string& angle) {
	this->name = "roofHip";
	this->angle = angle;
}

boost::shared_ptr<Shape> RoofHipOperator::apply(boost::shared_ptr<Shape>& shape, const RuleSet& ruleSet, std::list<boost::shared_ptr<Shape> >& stack) {
	float actual_angle = ruleSet.evalFloat(angle, shape);
	return shape->roofHip(shape->_name, actual_angle);
}

}
