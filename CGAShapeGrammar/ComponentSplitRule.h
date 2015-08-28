#pragma once

#include "Rule.h"

namespace cga {

class ComponentSplitRule : public Rule {
private:
	std::string front_name;
	std::string side_name;
	std::string top_name;
	std::string bottom_name;

public:
	ComponentSplitRule(const std::string& front_name, const std::string& side_name, const std::string& top_name, const std::string& bottom_name);
	void apply(Object* obj, std::list<Object*>& stack);
};

}
