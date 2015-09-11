#pragma once

#include "Rule.h"
#include <glm/gtc/matrix_transform.hpp>

namespace cga {

class ColorOperator : public Operator {
private:
	std::vector<std::string> color;

public:
	ColorOperator(const std::vector<std::string>& color);

	Shape* apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack);

private:
	static void decodeRGB(const std::string& str, float& r, float& g, float& b);
};

}