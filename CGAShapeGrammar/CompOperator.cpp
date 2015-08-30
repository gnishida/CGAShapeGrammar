#include "CompOperator.h"
#include "CGA.h"
#include "Rectangle.h"
#include "Polygon.h"

namespace cga {

CompOperator::CompOperator(const std::string& front_name, const std::string& side_name, const std::string& top_name, const std::string& bottom_name) {
	this->front_name = front_name;
	this->side_name = side_name;
	this->top_name = top_name;
	this->bottom_name = bottom_name;
}

Object* CompOperator::apply(Object* obj, std::list<Object*>& stack) {
	Object* front = NULL;
	std::vector<Object*> sides;
	Object* top = NULL;
	Object* bottom = NULL;
	
	obj->comp(front_name, &front, side_name, sides, top_name, &top, bottom_name, &bottom);
	if (front != NULL) {
		stack.push_back(front);
	}
	if (sides.size() > 0) {
		stack.insert(stack.end(), sides.begin(), sides.end());
	}
	if (top != NULL) {
		stack.push_back(top);
	}
	if (bottom != NULL) {
		stack.push_back(bottom);
	}

	return NULL;
}

}
