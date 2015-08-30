#pragma once

#include "Rule.h"

namespace cga {

class OffsetOperator : public Operator {
private:
	float offsetDistance;

public:
	OffsetOperator(float offsetDistance);

	Object* apply(Object* obj, std::list<Object*>& stack);
};

}