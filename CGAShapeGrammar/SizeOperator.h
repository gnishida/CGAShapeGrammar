#pragma once

#include "Rule.h"
#include <glm/gtc/matrix_transform.hpp>

namespace cga {

class SizeOperator : public Operator {
private:
	glm::vec3 size;

public:
	SizeOperator(const glm::vec3& size);

	Shape* apply(Shape* obj, std::list<Shape*>& stack);
};

}