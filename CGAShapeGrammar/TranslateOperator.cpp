#include "TranslateOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

TranslateOperator::TranslateOperator(int mode, int coordSystem, const glm::vec3& vec) {
	this->name = "translate";
	this->mode = mode;
	this->coordSystem = coordSystem;
	this->vec = vec;
}

Shape* TranslateOperator::apply(Shape* obj, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	obj->translate(mode, coordSystem, vec);
	return obj;
}

}