#include "CompOperator.h"
#include "CGA.h"
#include "Rectangle.h"
#include "Polygon.h"

namespace cga {

CompOperator::CompOperator(const std::map<std::string, std::string>& name_map) {
	this->name_map = name_map;
}

Shape* CompOperator::apply(Shape* obj, std::list<Shape*>& stack) {
	std::vector<Shape*> shapes;
	
	obj->comp(name_map, shapes);
	stack.insert(stack.end(), shapes.begin(), shapes.end());

	return NULL;
}

}
