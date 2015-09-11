#include "TaperOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

TaperOperator::TaperOperator(float height, float top_ratio) {
	this->name = "taper";
	this->height = height;
	this->top_ratio = top_ratio;
}

Shape* TaperOperator::apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	Shape* new_shape = shape->taper(shape->_name, height, top_ratio);
	delete shape;
	return new_shape;
}

}
