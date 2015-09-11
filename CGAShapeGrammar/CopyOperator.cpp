#include "CopyOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

CopyOperator::CopyOperator(const std::string& copy_name) {
	this->name = "copy";
	this->copy_name = copy_name;
}

Shape* CopyOperator::apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	Shape* copy = shape->clone(copy_name);
	stack.push_back(copy);

	return shape;
}

}
