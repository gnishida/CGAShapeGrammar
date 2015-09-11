#include "CompOperator.h"
#include "CGA.h"
#include "Rectangle.h"
#include "Polygon.h"

namespace cga {

CompOperator::CompOperator(const std::map<std::string, std::string>& name_map) {
	this->name = "comp";
	this->name_map = name_map;
}

Shape* CompOperator::apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	std::vector<Shape*> shapes;
	
	shape->comp(name_map, shapes);
	stack.insert(stack.end(), shapes.begin(), shapes.end());

	return NULL;
}

}
