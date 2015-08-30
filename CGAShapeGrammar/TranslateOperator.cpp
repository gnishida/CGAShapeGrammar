#include "TranslateOperator.h"
#include "CGA.h"

namespace cga {

TranslateOperator::TranslateOperator(const glm::vec3& vec) {
	this->vec = vec;
}

Object* TranslateOperator::apply(Object* obj, std::list<Object*>& stack) {
	obj->translate(vec);
	return obj;
}

}