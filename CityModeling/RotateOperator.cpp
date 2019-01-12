#include "RotateOperator.h"

namespace procedural_generation {

	RotateOperator::RotateOperator(float xAngle, float yAngle, float zAngle, const std::string& outputName) : Operator("rotate"), _xAngle(xAngle), _yAngle(yAngle), _zAngle(zAngle), _outputName(outputName) {
	}

	void RotateOperator::apply(std::shared_ptr<Shape> shape, std::queue<std::shared_ptr<Shape>>& derivationQueue) const {
		shape->rotate(_xAngle, _yAngle, _zAngle);
		shape->_name = _outputName;
		derivationQueue.push(shape);
	}

}
