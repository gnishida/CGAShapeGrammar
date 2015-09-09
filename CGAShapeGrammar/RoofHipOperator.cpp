#include "RoofHipOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

RoofHipOperator::RoofHipOperator(float angle) {
	this->angle = angle;
}

Shape* RoofHipOperator::apply(Shape* obj, std::list<Shape*>& stack) {
	return obj->roofHip(obj->_name, angle);
}

}
