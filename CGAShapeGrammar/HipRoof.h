#pragma once

#include "CGA.h"
#include "Shape.h"

namespace cga {

class HipRoof : public Shape {
private:
	std::vector<glm::vec2> _points;
	float _angle;

public:
	HipRoof(const std::string& name, const glm::mat4& pivot, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, float angle, const glm::vec3& color);
	Shape* clone(const std::string& name);
	void generate(RenderManager* renderManager, bool showScopeCoordinateSystem);
};

}
