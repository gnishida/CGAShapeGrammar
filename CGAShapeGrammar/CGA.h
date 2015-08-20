#pragma once

#include "RenderManager.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Vertex.h"

namespace cga {

enum { DIRECTION_X = 0, DIRECTION_Y };
enum { REVOLVE_X = 0, REVOLVE_Y };

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
	std::string _name;
	bool _removed;
	glm::mat4 _modelMat;
	glm::vec3 _color;
	bool _textureEnabled;
	std::string _texture;
	std::vector<glm::vec2> _texCoords;
	glm::vec3 _scope;

public:
	void translate(const glm::vec3& v);
	void rotate(float xAngle, float yAngle, float zAngle);
	void nil();
	void setTexture(const std::string& texture);
	virtual Object* clone();
	virtual void setupProjection(float texWidth, float texHeight);
	virtual Object* extrude(const std::string& name, float height);
	virtual Object* taper(const std::string& name, float height, float top_ratio = 0.0f);
	virtual Object* offset(const std::string& name, float offsetRatio);
	virtual Object* inscribeCircle(const std::string& name);
	virtual Object* revolve(const std::string& name, int direction);
	virtual void split(int direction, const std::vector<float> ratios, const std::vector<std::string> names, std::vector<Object*>& objects);
	virtual void componentSplit(const std::string& front_name, Rectangle** front, const std::string& sides_name, std::vector<Rectangle*>& sides, const std::string& top_name, Polygon** top, const std::string& base_name, Polygon** base);
	virtual void generate(RenderManager* renderManager);
};

class Line : public Object {
private:
	std::vector<glm::vec2> _points;

public:
	Line(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, const glm::vec3& color);
	Object* clone();
	Object* revolve(const std::string& name, int direction);
	void generate(RenderManager* renderManager);
};

class RevolvedLine : public Object {
private:
	std::vector<glm::vec2> _points;
	int _direction;

public:
	RevolvedLine(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, int direction, const glm::vec3& color);
	Object* clone();
	void generate(RenderManager* renderManager);
};

class Circle : public Object {
private:
	glm::vec2 _center;
	float _radius;

public:
	Circle(const std::string& name, const glm::mat4& modelMat, const glm::vec2& center, float radius, const glm::vec3& color);
};

class PrismObject : public Object {
private:
	std::vector<glm::vec2> _points;
	float _height;

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
	float _width;
	float _height;

public:
	Rectangle() {}
	Rectangle(const std::string& name, const glm::mat4& modelMat, float width, float height, const glm::vec3& color);
	Rectangle(const std::string& name, const glm::mat4& modelMat, float width, float height, const std::string& texture, float u1, float v1, float u2, float v2);
	Object* clone();
	void setupProjection(float texWidth, float texHeight);
	Object* extrude(const std::string& name, float height);
	Object* taper(const std::string& name, float height, float top_ratio = 0.0f);
	Object* offset(const std::string& name, float offsetRatio);
	Object* inscribeCircle(const std::string& name);
	void split(int direction, const std::vector<float> ratios, const std::vector<std::string> names, std::vector<Object*>& objects);
	void generate(RenderManager* renderManager);
};

class Polygon : public Object {
private:
	std::vector<glm::vec2> _points;
	glm::vec2 _center;

public:
	Polygon() {}
	Polygon(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, const glm::vec3& color, const std::string& texture);
	Object* clone();
	void setupProjection(float texWidth, float texHeight);
	Object* extrude(const std::string& name, float height);
	Object* taper(const std::string& name, float height, float top_ratio = 0.0f);
	Object* offset(const std::string& name, float offsetRatio);
	Object* inscribeCircle(const std::string& name);
	//void split(int direction, const std::vector<float> ratios, const std::vector<std::string> names, std::vector<Object*>& objects);
	void generate(RenderManager* renderManager);
};

class Pyramid : public Object {
private:
	std::vector<glm::vec2> _points;
	glm::vec2 _center;
	float _height;
	float _top_ratio;

public:
	Pyramid(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, const glm::vec2& center, float height, float top_ratio, const glm::vec3& color, const std::string& texture);
	void componentSplit(const std::string& front_name, Rectangle** front, const std::string& sides_name, std::vector<Rectangle*>& sides, const std::string& top_name, Polygon** top, const std::string& base_name, Polygon** base);
	void generate(RenderManager* renderManager);
};

class CGA {
public:
	glm::mat4 modelMat;

public:
	CGA();

	void generatePyramid(RenderManager* renderManager);
	void generateSimpleBuilding(RenderManager* renderManager);
	void generateBuilding(RenderManager* renderManager);
	void generateVase1(RenderManager* renderManager);
	void generateVase2(RenderManager* renderManager);
	void generateVase3(RenderManager* renderManager);
	void generateSaltShaker1(RenderManager* renderManager);
	void generateSaltShaker2(RenderManager* renderManager);
};

}
