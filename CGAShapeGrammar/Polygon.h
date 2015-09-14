#pragma once

#include "CGA.h"
#include "Shape.h"

namespace cga {

class Polygon : public Shape {
private:
	std::vector<glm::vec2> _points;
	glm::vec2 _center;

public:
	Polygon() {}
	Polygon(const std::string& name, const glm::mat4& pivot, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, const glm::vec3& color, const std::string& texture);
	Shape* clone(const std::string& name);
	Shape* extrude(const std::string& name, float height);
	Shape* inscribeCircle(const std::string& name);
	Shape* offset(const std::string& name, float offsetDistance, int offsetSelector);
	Shape* roofGable(const std::string& name, float angle);
	Shape* roofHip(const std::string& name, float angle);
	void setupProjection(float texWidth, float texHeight);
	void size(float xSize, float ySize, float zSize);
	//void split(int direction, const std::vector<float> ratios, const std::vector<std::string> names, std::vector<Object*>& objects);
	Shape* taper(const std::string& name, float height, float top_ratio = 0.0f);
	void generate(RenderManager* renderManager, bool showScopeCoordinateSystem);
};

}
