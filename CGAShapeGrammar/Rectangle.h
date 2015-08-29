#pragma once

#include "CGA.h"

namespace cga {

class Rectangle : public Object {
public:
	float _width;
	float _height;

public:
	Rectangle() {}
	Rectangle(const std::string& name, const glm::mat4& modelMat, float width, float height, const glm::vec3& color);
	Rectangle(const std::string& name, const glm::mat4& modelMat, float width, float height, const std::string& texture, float u1, float v1, float u2, float v2);
	Object* clone(const std::string& name);
	void setupProjection(float texWidth, float texHeight);
	Object* extrude(const std::string& name, float height);
	Object* taper(const std::string& name, float height, float top_ratio = 0.0f);
	Object* offset(const std::string& name, float offsetRatio);
	Object* inscribeCircle(const std::string& name);
	void split(int direction, const std::vector<float> ratios, const std::vector<std::string> names, std::vector<Object*>& objects);
	Object* roofHip(const std::string& name, float angle);
	void generate(RenderManager* renderManager);
};

}

