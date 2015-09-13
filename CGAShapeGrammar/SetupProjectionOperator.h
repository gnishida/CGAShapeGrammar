#pragma once

#include "Rule.h"

namespace cga {

class SetupProjectionOperator : public Operator {
private:
	int axesSelector;
	Value texWidth;
	Value texHeight;

public:
	SetupProjectionOperator(int axesSelector, const Value& texWidth, const Value& texHeight);
	Shape* apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack);
};

}