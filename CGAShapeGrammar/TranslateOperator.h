#pragma once

#include "Rule.h"
#include <glm/gtc/matrix_transform.hpp>

namespace cga {

class TranslateOperator : public Operator {
private:
	int mode;
	int coordSystem;
	glm::vec3 vec;

public:
	TranslateOperator(int mode, int coordSystem, const glm::vec3& vec);
	Shape* apply(Shape* obj, std::list<Shape*>& stack);
};

}
