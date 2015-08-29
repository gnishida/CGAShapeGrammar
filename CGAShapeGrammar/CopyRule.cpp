#include "CopyRule.h"
#include "CGA.h"

namespace cga {

CopyRule::CopyRule(const std::string& copy_name, const std::string& output_name) {
	this->copy_name = copy_name;
	this->output_name = output_name;
}

void CopyRule::apply(Object* obj, std::list<Object*>& stack) {
	Object* copy = obj->clone(copy_name);
	stack.push_back(copy);

	Object* orig = obj->clone(output_name);
	stack.push_back(orig);
}

}
