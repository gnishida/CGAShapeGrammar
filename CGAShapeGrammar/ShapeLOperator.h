#pragma once

#include "Rule.h"

namespace cga {

class ShapeLOperator : public Operator {
private:
	float frontWidth;
	float leftWidth;

public:
	ShapeLOperator(float frontWidth, float leftWidth);

	Object* apply(Object* obj, std::list<Object*>& stack);
};

}