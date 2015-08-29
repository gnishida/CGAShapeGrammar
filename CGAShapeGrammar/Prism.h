#pragma once

#include "CGA.h"

namespace cga {

class Prism : public Object {
private:
	std::vector<glm::vec2> _points;
	float _height;

public:
	Prism() {}
	Prism(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, float height, const glm::vec3& color);
	Object* clone(const std::string& name);
	void setupProjection(float texWidth, float texHeight);
	void split(int direction, const std::vector<float> ratios, const std::vector<std::string> names, std::vector<Object*>& objects);
	void comp(const std::string& front_name, Object** front, const std::string& sides_name, std::vector<Object*>& sides, const std::string& top_name, Object** top, const std::string& bottom_name, Object** bottom);
	void generate(RenderManager* renderManager);
};

}
