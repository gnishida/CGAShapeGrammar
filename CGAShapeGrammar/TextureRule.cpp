#include "TextureRule.h"
#include "CGA.h"

namespace cga {

TextureRule::TextureRule(const std::string& texture, const std::string& output_name) {
	this->texture = texture;
	this->output_name = output_name;
}

void TextureRule::apply(Object* obj, std::list<Object*>& stack) {
	Object* obj2 = obj->clone(output_name);
	
	obj2->texture(texture);
	stack.push_back(obj2);
}

}
