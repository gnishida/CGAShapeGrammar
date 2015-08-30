#pragma once

#include "Rule.h"

namespace cga {

class RoofHipOperator : public Operator {
private:
	float angle;

public:
	RoofHipOperator(float angle);

	Shape* apply(Shape* obj, std::list<Shape*>& stack);
};

}