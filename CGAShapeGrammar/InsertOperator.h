#pragma once

#include "Rule.h"

namespace cga {

class InsertOperator : public Operator {
private:
	std::string geometryPath;

public:
	InsertOperator(const std::string& geometryPath);

	Object* apply(Object* obj, std::list<Object*>& stack);
};

}