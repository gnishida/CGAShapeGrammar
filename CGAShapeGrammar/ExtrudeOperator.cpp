#include "ExtrudeOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

ExtrudeOperator::ExtrudeOperator(const std::string& height) {
	this->name = "extrude";
	this->height = height;
}

Shape* ExtrudeOperator::apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	float actual_height = ruleSet.evalFloat(height, shape);

	return shape->extrude(shape->_name, actual_height);
}

}
