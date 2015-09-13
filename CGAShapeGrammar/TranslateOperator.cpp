#include "TranslateOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

TranslateOperator::TranslateOperator(int mode, int coordSystem, const Value& x, const Value& y, const Value& z) {
	this->name = "translate";
	this->mode = mode;
	this->coordSystem = coordSystem;
	this->x = x;
	this->y = y;
	this->z = z;
}

Shape* TranslateOperator::apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	float actual_x = ruleSet.evalFloat(x.value, shape);
	float actual_y = ruleSet.evalFloat(y.value, shape);
	float actual_z = ruleSet.evalFloat(z.value, shape);
	shape->translate(mode, coordSystem, actual_x, actual_y, actual_z);
	return shape;
}

}