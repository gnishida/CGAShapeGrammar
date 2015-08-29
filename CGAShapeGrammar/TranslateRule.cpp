#include "TranslateRule.h"
#include "CGA.h"

namespace cga {

TranslateRule::TranslateRule(const glm::vec3& vec, const std::string& output_name) {
	this->vec = vec;
	this->output_name = output_name;
}

void TranslateRule::apply(Object* obj, std::list<Object*>& stack) {
	Object* obj2 = obj->clone(output_name);

	obj2->translate(vec);
	stack.push_back(obj2);
}

}