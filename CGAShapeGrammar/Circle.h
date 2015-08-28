#pragma once

#include "CGA.h"

namespace cga {

class Circle : public Object {
private:
	glm::vec2 _center;
	float _radius;

public:
	Circle(const std::string& name, const glm::mat4& modelMat, const glm::vec2& center, float radius, const glm::vec3& color);
};

}
