#include "ColorOperator.h"

namespace procedural_generation {

	ColorOperator::ColorOperator(float red, float green, float blue, float alpha, const std::string& outputName) : Operator("color"), _red(red), _green(green), _blue(blue), _alpha(alpha), _outputName(outputName) {
	}

	void ColorOperator::apply(std::shared_ptr<Shape> shape, std::queue<std::shared_ptr<Shape>>& derivationQueue) const {
		shape->_color = glm::vec4(_red, _green, _blue, 1.0f);
		shape->_name = _outputName;
		derivationQueue.push(shape);
	}

}
