#include "TranslateOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

TranslateOperator::TranslateOperator(const glm::vec3& vec) {
	this->vec = vec;
}

Shape* TranslateOperator::apply(Shape* obj, std::list<Shape*>& stack) {
	obj->translate(vec);
	return obj;
}

}