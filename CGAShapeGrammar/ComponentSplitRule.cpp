#include "ComponentSplitRule.h"
#include "CGA.h"

namespace cga {

ComponentSplitRule::ComponentSplitRule(const std::string& front_name, const std::string& side_name, const std::string& top_name, const std::string& bottom_name) {
	this->front_name = front_name;
	this->side_name = side_name;
	this->top_name = top_name;
	this->bottom_name = bottom_name;
}

void ComponentSplitRule::apply(Object* obj, std::list<Object*>& stack) {
	Rectangle* front;
	std::vector<Rectangle*> sides;
	Polygon* top;
	Polygon* bottom;
	
	obj->componentSplit(front_name, &front, side_name, sides, top_name, &top, bottom_name, &bottom);
	stack.push_back(front);
	stack.insert(stack.end(), sides.begin(), sides.end());
	stack.push_back(top);
	stack.push_back(bottom);
}

}
