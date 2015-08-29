#pragma once

#include "Rule.h"

namespace cga {

class RotateRule : public Rule {
private:
	float x;
	float y;
	float z;

public:
	RotateRule(float x, float y, float z, const std::string& output_name);

	void apply(Object* obj, std::list<Object*>& stack);
};

}