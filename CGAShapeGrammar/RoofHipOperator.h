#pragma once

#include "Rule.h"

namespace cga {

class RoofHipOperator : public Operator {
private:
	float angle;

public:
	RoofHipOperator(float angle);

	Object* apply(Object* obj, std::list<Object*>& stack);
};

}