#include "TranslateOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

TranslateOperator::TranslateOperator(int mode, int coordSystem, const glm::vec3& vec) {
	this->mode = mode;
	this->coordSystem = coordSystem;
	this->vec = vec;
}

Shape* TranslateOperator::apply(Shape* obj, std::list<Shape*>& stack) {
	obj->translate(mode, coordSystem, vec);
	return obj;
}

}