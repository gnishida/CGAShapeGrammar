#pragma once

#include "Rule.h"

namespace cga {

class OffsetRule : public Rule {
private:
	float offsetDistance;

public:
	OffsetRule(float offsetDistance, const std::string& output_name);

	void apply(Object* obj, std::list<Object*>& stack);
};

}