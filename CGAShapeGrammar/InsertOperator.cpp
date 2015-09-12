#include "InsertOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

InsertOperator::InsertOperator(const std::string& geometryPath) {
	this->name = "insert";
	this->geometryPath = geometryPath;
}

Shape* InsertOperator::apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	return shape->insert(shape->_name, ruleSet.evalString(geometryPath, shape));
}

}
