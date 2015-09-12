#pragma once

#include "Rule.h"

namespace cga {

class SetupProjectionOperator : public Operator {
private:
	int axesSelector;
	SingleValue texWidth;
	SingleValue texHeight;

public:
	SetupProjectionOperator(int axesSelector, const SingleValue& texWidth, const SingleValue& texHeight);
	Shape* apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack);
};

}