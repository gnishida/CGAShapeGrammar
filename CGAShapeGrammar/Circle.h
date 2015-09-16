#pragma once

#include <boost/shared_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Shape.h"

namespace cga {

class Circle : public Shape {
private:
	glm::vec2 _center;
	float _radius;

public:
	Circle(const std::string& name, const glm::mat4& modelMat, const glm::vec2& center, float radius, const glm::vec3& color);
	boost::shared_ptr<Shape> clone(const std::string& name);
};

}
