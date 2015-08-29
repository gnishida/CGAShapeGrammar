#pragma once

#include "Rule.h"

namespace cga {

class RoofHipRule : public Rule {
private:
	float angle;

public:
	RoofHipRule(float angle, const std::string& output_name);

	void apply(Object* obj, std::list<Object*>& stack);
};

}