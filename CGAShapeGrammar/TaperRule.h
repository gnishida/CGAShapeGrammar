#pragma once

#include "Rule.h"

namespace cga {

class TaperRule : public Rule {
private:
	float height;
	float top_ratio;

public:
	TaperRule(float height, float top_ratio, const std::string& output_name);

	void apply(Object* obj, std::list<Object*>& stack);
};

}