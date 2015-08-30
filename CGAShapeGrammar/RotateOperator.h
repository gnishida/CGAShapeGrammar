#pragma once

#include "Rule.h"

namespace cga {

class RotateOperator : public Operator {
private:
	float xAngle;
	float yAngle;
	float zAngle;

public:
	RotateOperator(float xAngle, float yAngle, float zAngle);

	Object* apply(Object* obj, std::list<Object*>& stack);
};

}