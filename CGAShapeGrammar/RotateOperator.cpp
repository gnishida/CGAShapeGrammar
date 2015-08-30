#include "RotateOperator.h"
#include "CGA.h"

namespace cga {

RotateOperator::RotateOperator(float xAngle, float yAngle, float zAngle) {
	this->xAngle = xAngle;
	this->yAngle = yAngle;
	this->zAngle = zAngle;
}

Shape* RotateOperator::apply(Shape* obj, std::list<Shape*>& stack) {
	obj->rotate(obj->_name, xAngle, yAngle, zAngle);
	return obj;
}

}
