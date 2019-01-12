#include "ExtrudeOperator.h"

namespace procedural_generation {

	ExtrudeOperator::ExtrudeOperator(const float height, const std::string& outputName) : Operator("extrude"), _height(height), _outputName(outputName) {
	}

	void ExtrudeOperator::apply(std::shared_ptr<Shape> shape, std::queue<std::shared_ptr<Shape>>& derivationQueue) const {
		std::shared_ptr<Shape> result = shape->extrude(_outputName, _height);
		derivationQueue.push(result);
	}

}