#pragma once

#include "Rule.h"
#include <glm/gtc/matrix_transform.hpp>

namespace cga {

class TranslateOperator : public Operator {
private:
	int mode;
	int coordSystem;
	Value x;
	Value y;
	Value z;

public:
	TranslateOperator(int mode, int coordSystem, const Value& x, const Value& y, const Value& z);
	Shape* apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack);
};

}
