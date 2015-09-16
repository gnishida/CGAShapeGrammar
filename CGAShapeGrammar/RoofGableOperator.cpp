#include "RoofGableOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

RoofGableOperator::RoofGableOperator(const std::string& angle) {
	this->name = "roofGable";
	this->angle = angle;
}

boost::shared_ptr<Shape> RoofGableOperator::apply(boost::shared_ptr<Shape>& shape, const RuleSet& ruleSet, std::list<boost::shared_ptr<Shape> >& stack) {
	float actual_angle = ruleSet.evalFloat(angle, shape);
	return shape->roofGable(shape->_name, actual_angle);
}

}
