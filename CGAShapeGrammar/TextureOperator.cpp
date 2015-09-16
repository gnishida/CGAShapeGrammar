#include "TextureOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

TextureOperator::TextureOperator(const std::string& texture) {
	this->name = "texture";
	this->texture = texture;
}

boost::shared_ptr<Shape> TextureOperator::apply(boost::shared_ptr<Shape>& shape, const RuleSet& ruleSet, std::list<boost::shared_ptr<Shape> >& stack) {
	shape->texture(ruleSet.evalString(texture, shape));
	return shape;
}

}
