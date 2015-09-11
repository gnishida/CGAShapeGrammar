#include "TextureOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

TextureOperator::TextureOperator(const std::string& texture) {
	this->name = "texture";
	this->texture = texture;
}

Shape* TextureOperator::apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	shape->texture(ruleSet.evalString(texture, shape));
	return shape;
}

}
