#include "SizeOperator.h"

namespace procedural_generation {

	SizeOperator::SizeOperator(const Value& xSize, const Value& ySize, const Value& zSize, const bool centered, const std::string& outputName) : Operator("size"), _xSize(xSize), _ySize(ySize), _zSize(zSize), _centered(centered), _outputName(outputName) {
	}

	void SizeOperator::apply(ShapePtr shape, std::queue<ShapePtr>& derivationQueue) const {
		float actual_xSize;
		float actual_ySize;
		float actual_zSize;

		if (_xSize.type == Value::TYPE_RELATIVE) {
			actual_xSize = shape->_scope.x * _xSize.value;
		}
		else {
			actual_xSize = _xSize.value;
		}

		if (_ySize.type == Value::TYPE_RELATIVE) {
			actual_ySize = shape->_scope.y * _ySize.value;
		}
		else {
			actual_ySize = _ySize.value;
		}

		if (_zSize.type == Value::TYPE_RELATIVE) {
			actual_zSize = shape->_scope.z * _zSize.value;
		}
		else {
			actual_zSize = _zSize.value;
		}

		shape->size(actual_xSize, actual_ySize, actual_zSize, _centered);
		shape->_name = _outputName;
		derivationQueue.push(shape);
	}

}