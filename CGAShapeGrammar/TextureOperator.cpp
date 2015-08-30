#include "TextureOperator.h"
#include "CGA.h"

namespace cga {

TextureOperator::TextureOperator(const std::string& texture) {
	this->texture = texture;
}

Shape* TextureOperator::apply(Shape* obj, std::list<Shape*>& stack) {
	obj->texture(texture);
	return obj;
}

}
