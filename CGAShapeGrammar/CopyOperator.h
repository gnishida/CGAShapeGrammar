#pragma once

#include "Rule.h"

namespace cga {

class CopyOperator : public Operator {
private:
	std::string copy_name;

public:
	CopyOperator(const std::string& copy_name);

	Shape* apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack);
};

}