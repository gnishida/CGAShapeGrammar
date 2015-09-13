#pragma once

#include "Rule.h"
#include <glm/gtc/matrix_transform.hpp>

namespace cga {

class SizeOperator : public Operator {
private:
	Value xSize;
	Value ySize;
	Value zSize;

public:
	SizeOperator(const Value& xSize, const Value& ySize, const Value& zSize);

	Shape* apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack);
};

}