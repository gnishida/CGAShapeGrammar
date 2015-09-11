#include "TranslateOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

TranslateOperator::TranslateOperator(int mode, int coordSystem, const SingleValue& x, const SingleValue& y, const SingleValue& z) {
	this->name = "translate";
	this->mode = mode;
	this->coordSystem = coordSystem;
	this->x = x;
	this->y = y;
	this->z = z;
}

Shape* TranslateOperator::apply(Shape* obj, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	float actual_x = ruleSet.evalFloat(x.value, obj);
	float actual_y = ruleSet.evalFloat(y.value, obj);
	float actual_z = ruleSet.evalFloat(z.value, obj);
	obj->translate(mode, coordSystem, actual_x, actual_y, actual_z);
	return obj;
}

}