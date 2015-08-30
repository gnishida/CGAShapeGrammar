#include "RotateOperator.h"
#include "CGA.h"

namespace cga {

RotateOperator::RotateOperator(float xAngle, float yAngle, float zAngle) {
	this->xAngle = xAngle;
	this->yAngle = yAngle;
	this->zAngle = zAngle;
}

Object* RotateOperator::apply(Object* obj, std::list<Object*>& stack) {
	obj->rotate(obj->_name, xAngle, yAngle, zAngle);
	return obj;
}

}
