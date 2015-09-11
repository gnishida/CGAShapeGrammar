#pragma once

#include "Rule.h"
#include <map>

namespace cga {

class CompOperator : public Operator {
private:
	std::map<std::string, std::string> name_map;

public:
	CompOperator(const std::map<std::string, std::string>& name_map);
	Shape* apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack);
};

}
