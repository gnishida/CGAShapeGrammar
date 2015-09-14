#pragma once

#include "CGA.h"
#include "Shape.h"

namespace cga {

class GeneralObject : public Shape {
private:
	std::vector<glm::vec3> _points;
	std::vector<glm::vec3> _normals;
	std::vector<glm::vec2> _texCoords;

public:
	GeneralObject(const std::string& name, const glm::mat4& pivot, const glm::mat4& modelMat, const std::vector<glm::vec3>& points, const std::vector<glm::vec3>& normals, const glm::vec3& color);
	GeneralObject(const std::string& name, const glm::mat4& pivot, const glm::mat4& modelMat, const std::vector<glm::vec3>& points, const std::vector<glm::vec3>& normals, const glm::vec3& color, const std::vector<glm::vec2>& texCoords, const std::string& texture);
	Shape* clone(const std::string& name);
	void size(float xSize, float ySize, float zSize);
	void generate(RenderManager* renderManager, bool showScopeCoordinateSystem);
};

}
