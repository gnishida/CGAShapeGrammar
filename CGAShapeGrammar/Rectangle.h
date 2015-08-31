#pragma once

#include "CGA.h"

namespace cga {

class Rectangle : public Shape {
public:
	float _width;
	float _height;

public:
	Rectangle() {}
	Rectangle(const std::string& name, const glm::mat4& modelMat, float width, float height, const glm::vec3& color);
	Rectangle(const std::string& name, const glm::mat4& modelMat, float width, float height, const std::string& texture, float u1, float v1, float u2, float v2);
	Shape* clone(const std::string& name);
	void setupProjection(float texWidth, float texHeight);
	Shape* extrude(const std::string& name, float height);
	Shape* taper(const std::string& name, float height, float top_ratio = 0.0f);
	Shape* offset(const std::string& name, float offsetRatio);
	Shape* inscribeCircle(const std::string& name);
	Shape* roofHip(const std::string& name, float angle);
	Shape* shapeL(const std::string& name, float frontWidth, float leftWidth);
	void size(const glm::vec3& sz);
	void split(int direction, const std::vector<float>& ratios, const std::vector<std::string>& names, std::vector<Shape*>& objects);
	void generate(RenderManager* renderManager, bool showAxes);
};

}

