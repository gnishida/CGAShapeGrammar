#include "RoofHipOperator.h"
#include "CGA.h"

namespace cga {

RoofHipOperator::RoofHipOperator(float angle) {
	this->angle = angle;
}

Object* RoofHipOperator::apply(Object* obj, std::list<Object*>& stack) {
	return obj->roofHip(obj->_name, angle);
}

}
