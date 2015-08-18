#pragma once

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
	glm::mat4 modelMat;
	glm::vec3 color;
	std::string texture;

public:
	void translate(const glm::vec3& v);
	void setTexture(const std::string& texture);
	virtual PrismObject extrude(float height) = 0;
	virtual void componentSplit(Rectangle& front, std::vector<Rectangle>& sides, Polygon& top, Polygon& base) = 0;
	virtual void generate(std::vector<Vertex>& vertices) = 0;
};

class PrismObject : public Object {
private:
	std::vector<glm::vec2> points;
	float height;

public:
	PrismObject(const glm::mat4& modelMat, const std::vector<glm::vec2>& points, float height, const glm::vec3& color);
	PrismObject extrude(float height);
	void componentSplit(Rectangle& front, std::vector<Rectangle>& sides, Polygon& top, Polygon& base);
	void generate(std::vector<Vertex>& vertices);
};

class Rectangle : public Object {
public:
	float width;
	float height;

public:
	Rectangle() {}
	Rectangle(const glm::mat4& modelMat, float width, float height, const glm::vec3& color);
	PrismObject extrude(float height);
	void componentSplit(Rectangle& front, std::vector<Rectangle>& sides, Polygon& top, Polygon& base);
	void split(int direction, const std::vector<float> ratios, std::vector<Rectangle>& rectangles);
	void generate(std::vector<Vertex>& vertices);
};

class Polygon : public Object {
private:
	std::vector<glm::vec2> points;

public:
	Polygon() {}
	Polygon(const glm::mat4& modelMat, const std::vector<glm::vec2> points, const glm::vec3& color);
	PrismObject extrude(float height);
	void componentSplit(Rectangle& front, std::vector<Rectangle>& sides, Polygon& top, Polygon& base);
	void split(int direction, const std::vector<float> ratios, std::vector<Rectangle>& rectangles);
	void generate(std::vector<Vertex>& vertices);
};

class CGA {
public:
	CGA();

	void generate(std::vector<Vertex>& vertices);
};

}
