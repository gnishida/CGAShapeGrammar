#pragma once

#include "Rule.h"
#include <glm/gtc/matrix_transform.hpp>

namespace cga {

class SizeOperator : public Operator {
private:
	SingleValue xSize;
	SingleValue ySize;
	SingleValue zSize;

public:
	SizeOperator(const SingleValue& xSize, const SingleValue& ySize, const SingleValue& zSize);

	Shape* apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack);
};

}