#pragma once

#include "Rule.h"
#include <glm/gtc/matrix_transform.hpp>

namespace cga {

class ColorOperator : public Operator {
private:
	glm::vec3 color;

public:
	ColorOperator(const glm::vec3& color);

	Shape* apply(Shape* obj, const RuleSet& ruleSet, std::list<Shape*>& stack);
};

}