#include "SizeOperator.h"
#include "CGA.h"

namespace cga {

SizeOperator::SizeOperator(const glm::vec3& size) {
	this->size = size;
}

Shape* SizeOperator::apply(Shape* obj, std::list<Shape*>& stack) {
	obj->size(size);
	return obj;
}

}
