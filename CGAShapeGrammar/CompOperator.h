#pragma once

#include "Rule.h"

namespace cga {

class CompOperator : public Operator {
private:
	std::string front_name;
	std::string side_name;
	std::string top_name;
	std::string bottom_name;

public:
	CompOperator(const std::string& front_name, const std::string& side_name, const std::string& top_name, const std::string& bottom_name);
	Object* apply(Object* obj, std::list<Object*>& stack);
};

}
