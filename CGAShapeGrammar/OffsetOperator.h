#pragma once

#include "Rule.h"

namespace cga {

class OffsetOperator : public Operator {
private:
	std::string offsetDistance;
	int offsetSelector;

public:
	OffsetOperator(const std::string& offsetDistance, int offsetSelector);

	Shape* apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack);
};

}