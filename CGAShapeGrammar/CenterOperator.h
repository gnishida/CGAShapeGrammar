#pragma once

#include "Rule.h"

namespace cga {

class CenterOperator : public Operator {
private:
	int axesSelector;

public:
	CenterOperator(int axesSelector);

	Shape* apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack);
};

}