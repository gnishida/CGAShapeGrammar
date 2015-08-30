#pragma once

#include "Rule.h"

namespace cga {

class ExtrudeOperator : public Operator {
private:
	float height;

public:
	ExtrudeOperator(float height);

	Shape* apply(Shape* obj, std::list<Shape*>& stack);
};

}