#pragma once

#include "CGA.h"

namespace cga {

class Polygon : public Object {
private:
	std::vector<glm::vec2> _points;
	glm::vec2 _center;

public:
	Polygon() {}
	Polygon(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, const glm::vec3& color, const std::string& texture);
	Object* clone(const std::string& name);
	void setupProjection(float texWidth, float texHeight);
	Object* extrude(const std::string& name, float height);
	Object* taper(const std::string& name, float height, float top_ratio = 0.0f);
	Object* offset(const std::string& name, float offsetRatio);
	Object* inscribeCircle(const std::string& name);
	//void split(int direction, const std::vector<float> ratios, const std::vector<std::string> names, std::vector<Object*>& objects);
	Object* roofHip(const std::string& name, float angle);
	void generate(RenderManager* renderManager);
};

}
