#include "ColorOperator.h"
#include "CGA.h"

namespace cga {

ColorOperator::ColorOperator(const glm::vec3& color) {
	this->color = color;
}

Shape* ColorOperator::apply(Shape* obj, std::list<Shape*>& stack) {
	obj->_color = color;
	return obj;
}

}
