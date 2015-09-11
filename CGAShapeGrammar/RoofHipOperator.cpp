#include "RoofHipOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

RoofHipOperator::RoofHipOperator(float angle) {
	this->angle = angle;
}

Shape* RoofHipOperator::apply(Shape* obj, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	return obj->roofHip(obj->_name, angle);
}

}
