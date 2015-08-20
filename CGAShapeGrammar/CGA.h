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

class BoundingBox {
public:
	glm::vec2 bottom_left;
	glm::vec2 upper_right;

public:
	BoundingBox(const std::vector<glm::vec2>& points);
};

class Object {
public:
	std::string name;
	bool removed;
	glm::mat4 modelMat;
	glm::vec3 color;
	bool textureEnabled;
	std::string texture;
	std::vector<glm::vec2> texCoords;
	glm::vec3 scope;

public:
	void translate(const glm::vec3& v);
	void rotate(float xAngle, float yAngle, float zAngle);
	void nil();
	void setTexture(const std::string& texture);
	virtual Object* clone();
	virtual void setupProjection(float texWidth, float texHeight);
	virtual Object* extrude(const std::string& name, float height);
	virtual Object* taper(const std::string& name, float height);
	virtual void split(int direction, const std::vector<float> ratios, const std::vector<std::string> names, std::vector<Object*>& objects);
	virtual void componentSplit(const std::string& front_name, Rectangle** front, const std::string& sides_name, std::vector<Rectangle*>& sides, const std::string& top_name, Polygon** top, const std::string& base_name, Polygon** base);
	virtual void generate(RenderManager* renderManager);
};

class PrismObject : public Object {
private:
	std::vector<glm::vec2> points;
	float height;

public:
	PrismObject() {}
	PrismObject(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, float height, const glm::vec3& color);
	Object* clone();
	void setupProjection(float texWidth, float texHeight);
	void split(int direction, const std::vector<float> ratios, const std::vector<std::string> names, std::vector<Object*>& objects);
	void componentSplit(const std::string& front_name, Rectangle** front, const std::string& sides_name, std::vector<Rectangle*>& sides, const std::string& top_name, Polygon** top, const std::string& base_name, Polygon** base);
	void generate(RenderManager* renderManager);
};

class Rectangle : public Object {
public:
	float width;
	float height;

public:
	Rectangle() {}
	Rectangle(const std::string& name, const glm::mat4& modelMat, float width, float height, const glm::vec3& color);
	Rectangle(const std::string& name, const glm::mat4& modelMat, float width, float height, const std::string& texture, float u1, float v1, float u2, float v2);
	Object* clone();
	void setupProjection(float texWidth, float texHeight);
	Object* extrude(const std::string& name, float height);
	Object* taper(const std::string& name, float height);
	void split(int direction, const std::vector<float> ratios, const std::vector<std::string> names, std::vector<Object*>& objects);
	void generate(RenderManager* renderManager);
};

class Polygon : public Object {
private:
	std::vector<glm::vec2> points;

public:
	Polygon() {}
	Polygon(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, const glm::vec3& color, const std::string& texture);
	Object* clone();
	void setupProjection(float texWidth, float texHeight);
	Object* extrude(const std::string& name, float height);
	Object* taper(const std::string& name, float height);
	//void split(int direction, const std::vector<float> ratios, const std::vector<std::string> names, std::vector<Object*>& objects);
	void generate(RenderManager* renderManager);
};

class Pyramid : public Object {
private:
	std::vector<glm::vec2> points;
	glm::vec2 center;
	float height;

public:
	Pyramid(const std::string& name, const std::vector<glm::vec2>& points, const glm::vec2& center, float height, const glm::vec3& color, const std::string& texture);
	void generate(RenderManager* renderManager);
};

class CGA {
public:
	CGA();

	void generatePyramid(RenderManager* renderManager);
	void generateSimpleBuilding(RenderManager* renderManager);
	void generateBuilding(RenderManager* renderManager);
	void generateVase(RenderManager* renderManager);
};

}
