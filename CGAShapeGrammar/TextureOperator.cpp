#include "TextureOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

TextureOperator::TextureOperator(const std::string& texture) {
	this->name = "texture";
	this->texture = texture;
}

Shape* TextureOperator::apply(Shape* obj, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	obj->texture(ruleSet.evalString(texture, obj));
	return obj;
}

}
