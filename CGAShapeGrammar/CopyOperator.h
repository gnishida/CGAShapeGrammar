#pragma once

#include "Rule.h"

namespace cga {

class CopyOperator : public Operator {
private:
	std::string copy_name;

public:
	CopyOperator(const std::string& copy_name);

	Object* apply(Object* obj, std::list<Object*>& stack);
};

}