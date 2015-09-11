#include "ColorOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

ColorOperator::ColorOperator(const glm::vec3& color) {
	this->color = color;
}

Shape* ColorOperator::apply(Shape* obj, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	obj->_color = color;
	return obj;
}

}
