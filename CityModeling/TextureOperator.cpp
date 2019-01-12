#include "TextureOperator.h"
#include "Shape.h"

namespace procedural_generation {

	TextureOperator::TextureOperator(std::string texturePath, const std::string& outputName) : Operator("texture"), _texturePath(texturePath), _outputName(outputName) {
	}

	void TextureOperator::apply(ShapePtr shape, std::queue<ShapePtr>& derivationQueue) const {
		shape->_texturePath = _texturePath;
		shape->_name = _outputName;
		derivationQueue.push(shape);
	}

}
