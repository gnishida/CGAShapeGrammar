#include "RotateRule.h"
#include "CGA.h"

namespace cga {

RotateRule::RotateRule(float x, float y, float z, const std::string& output_name) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->output_name = output_name;
}

void RotateRule::apply(Object* obj, std::list<Object*>& stack) {
	// to be fixed
	//stack.push_back(obj->rotate(output_name, x, y, z));
}

}
