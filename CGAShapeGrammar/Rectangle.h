#pragma once

#include "CGA.h"
#include "Shape.h"

namespace cga {

class Rectangle : public Shape {
public:
	Rectangle() {}
	Rectangle(const std::string& name, const glm::mat4& pivot, const glm::mat4& modelMat, float width, float height, const glm::vec3& color);
	Rectangle(const std::string& name, const glm::mat4& pivot, const glm::mat4& modelMat, float width, float height, const glm::vec3& color, const std::string& texture, float u1, float v1, float u2, float v2);
	Shape* clone(const std::string& name);
	Shape* extrude(const std::string& name, float height);
	Shape* inscribeCircle(const std::string& name);
	Shape* offset(const std::string& name, float offsetDistance, int offsetSelector);
	Shape* roofGable(const std::string& name, float angle);
	Shape* roofHip(const std::string& name, float angle);
	void setupProjection(int axesSelector, float texWidth, float texHeight);
	Shape* shapeL(const std::string& name, float frontWidth, float leftWidth);
	void size(float xSize, float ySize, float zSize);
	void split(int splitAxis, const std::vector<float>& ratios, const std::vector<std::string>& names, std::vector<Shape*>& objects);
	Shape* taper(const std::string& name, float height, float top_ratio = 0.0f);
	void generate(RenderManager* renderManager, bool showScopeCoordinateSystem);
};

}

