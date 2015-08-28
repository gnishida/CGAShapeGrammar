#include "SetTextureRule.h"
#include "CGA.h"

namespace cga {

SetTextureRule::SetTextureRule(const std::string& texture, const std::string& output_name) {
	this->texture = texture;
	this->output_name = output_name;
}

void SetTextureRule::apply(Object* obj, std::list<Object*>& stack) {
	Object* obj2 = obj->clone();
	obj2->_name = output_name;
	
	obj2->setTexture(texture);
	stack.push_back(obj2);
}

}
