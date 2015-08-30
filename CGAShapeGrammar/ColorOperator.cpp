#include "ColorOperator.h"
#include "CGA.h"

namespace cga {

ColorOperator::ColorOperator(const glm::vec3& color) {
	this->color = color;
}

Object* ColorOperator::apply(Object* obj, std::list<Object*>& stack) {
	obj->_color = color;
	return obj;
}

}
