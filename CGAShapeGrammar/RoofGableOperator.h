#pragma once

#include "Rule.h"

namespace cga {

class RoofGableOperator : public Operator {
private:
	std::string angle;

public:
	RoofGableOperator(const std::string& angle);

	Shape* apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack);
};

}