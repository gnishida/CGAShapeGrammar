#pragma once

#include "Rule.h"

namespace cga {

class InsertOperator : public Operator {
private:
	std::string geometryPath;

public:
	InsertOperator(const std::string& geometryPath);

	Shape* apply(Shape* obj, const RuleSet& ruleSet, std::list<Shape*>& stack);
};

}