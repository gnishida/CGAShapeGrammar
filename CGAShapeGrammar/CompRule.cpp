#include "CompRule.h"
#include "CGA.h"
#include "Rectangle.h"
#include "Polygon.h"

namespace cga {

CompRule::CompRule(const std::string& front_name, const std::string& side_name, const std::string& top_name, const std::string& bottom_name) {
	this->front_name = front_name;
	this->side_name = side_name;
	this->top_name = top_name;
	this->bottom_name = bottom_name;
}

void CompRule::apply(Object* obj, std::list<Object*>& stack) {
	Rectangle* front;
	std::vector<Rectangle*> sides;
	Polygon* top;
	Polygon* bottom;
	
	obj->comp(front_name, &front, side_name, sides, top_name, &top, bottom_name, &bottom);
	if (!front_name.empty()) {
		stack.push_back(front);
	}
	if (!side_name.empty()) {
		stack.insert(stack.end(), sides.begin(), sides.end());
	}
	if (!top_name.empty()) {
		stack.push_back(top);
	}
	if (!bottom_name.empty()) {
		stack.push_back(bottom);
	}
}

}
