#pragma once

#include "Operator.h"

namespace procedural_generation {

	class TextureOperator : public Operator {
	public:
		TextureOperator(std::string texturePath, const std::string& outputName);
		virtual void apply(ShapePtr shape, std::queue<ShapePtr>& derivationQueue) const override;

	private:
		std::string _texturePath;
		std::string _outputName;
	};

}
