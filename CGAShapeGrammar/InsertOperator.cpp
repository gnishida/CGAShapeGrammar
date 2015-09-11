#include "InsertOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

InsertOperator::InsertOperator(const std::string& geometryPath) {
	this->name = "insert";
	this->geometryPath = geometryPath;
}

Shape* InsertOperator::apply(Shape* obj, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	return obj->insert(obj->_name, geometryPath);
}

}
