#include "CopyOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

CopyOperator::CopyOperator(const std::string& copy_name) {
	this->copy_name = copy_name;
}

Shape* CopyOperator::apply(Shape* obj, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	Shape* copy = obj->clone(copy_name);
	stack.push_back(copy);

	return obj->clone(obj->_name);
}

}
