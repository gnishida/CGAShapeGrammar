#include "TranslateOperator.h"

namespace procedural_generation {

	TranslateOperator::TranslateOperator(const TranslateMode translateMode, const CoordinateSystem coordSystem, const Value& x, const Value& y, const Value& z) : Operator("translate"), _translateMode(translateMode), _coordSystem(coordSystem), _x(x), _y(y), _z(z) {
	}

	void TranslateOperator::apply(ShapePtr shape, std::queue<ShapePtr>& derivationQueue) const {
		float actual_x;
		float actual_y;
		float actual_z;

		if (_x.type == Value::TYPE_RELATIVE) {
			actual_x = shape->_scope.x * _x.value;
		}
		else {
			actual_x = _x.value;
		}

		if (_y.type == Value::TYPE_RELATIVE) {
			actual_y = shape->_scope.y * _y.value;
		}
		else {
			actual_y = _y.value;
		}

		if (_z.type == Value::TYPE_RELATIVE) {
			actual_z = shape->_scope.z * _z.value;
		}
		else {
			actual_z = _z.value;
		}

		shape->translate(_translateMode, _coordSystem, actual_x, actual_y, actual_z);
		derivationQueue.push(shape);
	}

}
