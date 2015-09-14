#pragma once

#include "Rule.h"

namespace cga {

class RoofHipOperator : public Operator {
private:
	std::string angle;

public:
	RoofHipOperator(const std::string& angle);

	Shape* apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack);
};

}