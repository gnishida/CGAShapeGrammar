#pragma once

#include "CGA.h"

namespace cga {

class HipRoof : public Object {
private:
	std::vector<glm::vec2> _points;
	float _angle;

public:
	HipRoof(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, float angle, const glm::vec3& color);
	Object* clone();
	void generate(RenderManager* renderManager);
};

}
