#pragma once

#include "Rule.h"

namespace cga {

class OffsetOperator : public Operator {
private:
	float offsetDistance;

public:
	OffsetOperator(float offsetDistance);

	Shape* apply(Shape* obj, std::list<Shape*>& stack);
};

}