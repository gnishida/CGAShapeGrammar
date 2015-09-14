#pragma once

#include "Rule.h"

namespace cga {

class OffsetOperator : public Operator {
private:
	std::string offsetDistance;

public:
	OffsetOperator(const std::string& offsetDistance);

	Shape* apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack);
};

}