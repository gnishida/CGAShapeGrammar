#pragma once

#include "Rule.h"

namespace cga {

class TaperOperator : public Operator {
private:
	float height;
	float top_ratio;

public:
	TaperOperator(float height, float top_ratio);

	Object* apply(Object* obj, std::list<Object*>& stack);
};

}