#pragma once

#include "Rule.h"
#include <glm/gtc/matrix_transform.hpp>

namespace cga {

class TranslateOperator : public Operator {
private:
	int mode;
	int coordSystem;
	SingleValue x;
	SingleValue y;
	SingleValue z;

public:
	TranslateOperator(int mode, int coordSystem, const SingleValue& x, const SingleValue& y, const SingleValue& z);
	Shape* apply(Shape* obj, const RuleSet& ruleSet, std::list<Shape*>& stack);
};

}
