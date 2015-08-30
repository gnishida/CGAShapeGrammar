#pragma once

#include "Rule.h"

namespace cga {

class ExtrudeOperator : public Operator {
private:
	float height;

public:
	ExtrudeOperator(float height);

	Object* apply(Object* obj, std::list<Object*>& stack);
};

}