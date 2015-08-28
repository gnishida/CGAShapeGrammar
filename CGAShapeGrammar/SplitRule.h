#pragma once

#include "Rule.h"

namespace cga {

class SplitRule : public Rule {
private:
	int direction;
	std::vector<Value*> sizes;
	std::vector<std::string> output_names;

public:
	SplitRule(int direction, const std::vector<Value*>& sizes, const std::vector<std::string>& output_names);
	void apply(Object* obj, std::list<Object*>& stack);
};

}
