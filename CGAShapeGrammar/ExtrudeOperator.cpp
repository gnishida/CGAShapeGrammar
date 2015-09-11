#include "ExtrudeOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

ExtrudeOperator::ExtrudeOperator(const std::string& height) {
	this->name = "extrude";
	this->height = height;
}

Shape* ExtrudeOperator::apply(Shape* obj, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	float fHeight = ruleSet.eval(height);

	return obj->extrude(obj->_name, fHeight);
}

}
