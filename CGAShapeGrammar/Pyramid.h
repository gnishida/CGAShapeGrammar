#pragma once

#include "CGA.h"

namespace cga {

class Pyramid : public Object {
private:
	std::vector<glm::vec2> _points;
	glm::vec2 _center;
	float _height;
	float _top_ratio;

public:
	Pyramid(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, const glm::vec2& center, float height, float top_ratio, const glm::vec3& color, const std::string& texture);
	Object* clone(const std::string& name);
	void comp(const std::string& front_name, Object** front, const std::string& sides_name, std::vector<Object*>& sides, const std::string& top_name, Object** top, const std::string& base_name, Object** base);
	void generate(RenderManager* renderManager, bool showAxes);
};

}
