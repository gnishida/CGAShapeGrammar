#pragma once

#include "Rule.h"

namespace cga {

class SplitOperator : public Operator {
private:
	int direction;
	std::vector<Value*> sizes;
	std::vector<std::string> output_names;

public:
	SplitOperator(int direction, const std::vector<Value*>& sizes, const std::vector<std::string>& output_names);
	Object* apply(Object* obj, std::list<Object*>& stack);
};

}
