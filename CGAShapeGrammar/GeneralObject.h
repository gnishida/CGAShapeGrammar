#pragma once

#include "CGA.h"
#include "Shape.h"

namespace cga {

class GeneralObject : public Shape {
private:
	std::vector<glm::vec3> _points;
	std::vector<glm::vec3> _normals;

public:
	GeneralObject(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec3>& points, const std::vector<glm::vec3>& normals, const glm::vec3& color);
	Shape* clone(const std::string& name);
	void generate(RenderManager* renderManager, bool showAxes);
};

}
