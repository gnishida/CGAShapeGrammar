#pragma once

#include "Rule.h"

namespace cga {

class ExtrudeRule : public Rule {
private:
	float height;

public:
	ExtrudeRule(float height, const std::string& output_name);

	void apply(Object* obj, std::list<Object*>& stack);
};

}