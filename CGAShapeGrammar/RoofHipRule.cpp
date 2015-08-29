#include "RoofHipRule.h"
#include "CGA.h"

namespace cga {

RoofHipRule::RoofHipRule(float angle, const std::string& output_name) {
	this->angle = angle;
	this->output_name = output_name;
}

void RoofHipRule::apply(Object* obj, std::list<Object*>& stack) {
	stack.push_back(obj->roofHip(output_name, angle));
}

}
