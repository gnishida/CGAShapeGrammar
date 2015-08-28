#include "ExtrudeRule.h"
#include "CGA.h"

namespace cga {

ExtrudeRule::ExtrudeRule(float height, const std::string& output_name) {
	this->height = height;
	this->output_name = output_name;
}

void ExtrudeRule::apply(Object* obj, std::list<Object*>& stack) {
	stack.push_back(obj->extrude(output_name, height));
}

}
