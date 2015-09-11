#pragma once

#include "CGA.h"
#include "Shape.h"

namespace cga {

class Prism : public Shape {
private:
	std::vector<glm::vec2> _points;
	float _height;

public:
	Prism() {}
	Prism(const std::string& name, const glm::mat4& pivot, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, float height, const glm::vec3& color);
	Shape* clone(const std::string& name);
	void comp(const std::map<std::string, std::string>& name_map, std::vector<Shape*>& shapes);
	void setupProjection(float texWidth, float texHeight);
	void size(float xSize, float ySize, float zSize);
	void split(int splitAxis, const std::vector<float>& sizes, const std::vector<std::string>& names, std::vector<Shape*>& objects);
	void generate(RenderManager* renderManager, bool showScopeCoordinateSystem);
};

}
