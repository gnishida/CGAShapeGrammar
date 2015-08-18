#pragma once

#include "RenderManager.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Vertex.h"

namespace cga {

enum { DIRECTION_X = 0, DIRECTION_Y };

class PrismObject;
class Polygon;
class Rectangle;

class Object {
protected:
	std::string name;
	glm::mat4 modelMat;
	glm::vec3 color;
	std::string texture;

public:
	void translate(const glm::vec3& v);
	void setTexture(const std::string& texture);
	virtual PrismObject extrude(const std::string& name, float height) = 0;
	virtual void componentSplit(const std::string& front_name, Rectangle& front, const std::string& sides_name, std::vector<Rectangle>& sides, const std::string& top_name, Polygon& top, const std::string& base_name, Polygon& base) = 0;
	virtual void generate(RenderManager* renderManager) = 0;
};

class PrismObject : public Object {
private:
	std::vector<glm::vec2> points;
	float height;

public:
	PrismObject() {}
	PrismObject(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, float height, const glm::vec3& color);
	PrismObject extrude(const std::string& name, float height);
	void componentSplit(const std::string& front_name, Rectangle& front, const std::string& sides_name, std::vector<Rectangle>& sides, const std::string& top_name, Polygon& top, const std::string& base_name, Polygon& base);
	void generate(RenderManager* renderManager);
};

class Rectangle : public Object {
public:
	float width;
	float height;

public:
	Rectangle() {}
	Rectangle(const std::string& name, const glm::mat4& modelMat, float width, float height, const glm::vec3& color);
	PrismObject extrude(const std::string& name, float height);
	void componentSplit(const std::string& front_name, Rectangle& front, const std::string& sides_name, std::vector<Rectangle>& sides, const std::string& top_name, Polygon& top, const std::string& base_name, Polygon& base);
	void split(int direction, const std::vector<float> ratios, const std::vector<std::string> names, std::vector<Rectangle>& rectangles);
	void generate(RenderManager* renderManager);
};

class Polygon : public Object {
private:
	std::vector<glm::vec2> points;

public:
	Polygon() {}
	Polygon(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2> points, const glm::vec3& color);
	PrismObject extrude(const std::string& name, float height);
	void componentSplit(const std::string& front_name, Rectangle& front, const std::string& sides_name, std::vector<Rectangle>& sides, const std::string& top_name, Polygon& top, const std::string& base_name, Polygon& base);
	void split(int direction, const std::vector<float> ratios, const std::vector<std::string> names, std::vector<Rectangle>& rectangles);
	void generate(RenderManager* renderManager);
};

class CGA {
public:
	CGA();

	void generate(RenderManager* renderManager);
};

}
