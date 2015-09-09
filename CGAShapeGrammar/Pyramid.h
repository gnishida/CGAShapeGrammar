#pragma once

#include "CGA.h"
#include "Shape.h"

namespace cga {

class Pyramid : public Shape {
private:
	std::vector<glm::vec2> _points;
	glm::vec2 _center;
	float _height;
	float _top_ratio;

public:
	Pyramid(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, const glm::vec2& center, float height, float top_ratio, const glm::vec3& color, const std::string& texture);
	Shape* clone(const std::string& name);
	void comp(const std::string& front_name, Shape** front, const std::string& sides_name, std::vector<Shape*>& sides, const std::string& top_name, Shape** top, const std::string& base_name, Shape** base);
	void generate(RenderManager* renderManager, bool showAxes);
};

}
