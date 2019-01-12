#include "SetupProjectionOperator.h"

namespace procedural_generation {

	SetupProjectionOperator::SetupProjectionOperator(AxisDirection axesSelector, const Value& texWidth, const Value& texHeight, const std::string& outputName) : Operator("setupProjection"), _axesSelector(axesSelector), _texWidth(texWidth), _texHeight(texHeight), _outputName(outputName) {
	}

	void SetupProjectionOperator::apply(std::shared_ptr<Shape> shape, std::queue<std::shared_ptr<Shape>>& derivationQueue) const {
		float actual_texWidth;
		float actual_texHeight;

		if (_texWidth.type == Value::TYPE_RELATIVE) {
			actual_texWidth = shape->_scope.x * _texWidth.value;
		} else {
			actual_texWidth = _texWidth.value;
		}
		if (_texHeight.type == Value::TYPE_RELATIVE) {
			actual_texHeight = shape->_scope.y * _texHeight.value;
		} else {
			actual_texHeight = _texHeight.value;
		}


		shape->setupProjection(_axesSelector, actual_texWidth, actual_texHeight);
		shape->_name = _outputName;
		derivationQueue.push(shape);
	}

}
