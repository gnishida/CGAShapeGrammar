#pragma once

#include "CGA.h"

namespace cga {

class Polygon : public Shape {
private:
	std::vector<glm::vec2> _points;
	glm::vec2 _center;

public:
	Polygon() {}
	Polygon(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, const glm::vec3& color, const std::string& texture);
	Shape* clone(const std::string& name);
	void setupProjection(float texWidth, float texHeight);
	Shape* extrude(const std::string& name, float height);
	Shape* taper(const std::string& name, float height, float top_ratio = 0.0f);
	Shape* offset(const std::string& name, float offsetRatio);
	Shape* inscribeCircle(const std::string& name);
	//void split(int direction, const std::vector<float> ratios, const std::vector<std::string> names, std::vector<Object*>& objects);
	Shape* roofHip(const std::string& name, float angle);
	void size(const glm::vec3& sz);
	void generate(RenderManager* renderManager, bool showAxes);
};

}
