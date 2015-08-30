#include "TextureOperator.h"
#include "CGA.h"

namespace cga {

TextureOperator::TextureOperator(const std::string& texture) {
	this->texture = texture;
}

Object* TextureOperator::apply(Object* obj, std::list<Object*>& stack) {
	obj->texture(texture);
	return obj;
}

}
