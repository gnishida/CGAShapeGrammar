#include "OffsetRule.h"
#include "CGA.h"

namespace cga {

OffsetRule::OffsetRule(float offsetDistance, const std::string& output_name) {
	this->offsetDistance = offsetDistance;
	this->output_name = output_name;
}

void OffsetRule::apply(Object* obj, std::list<Object*>& stack) {
	stack.push_back(obj->offset(output_name, offsetDistance));
}

}
