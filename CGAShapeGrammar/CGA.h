#pragma once

#include "RenderManager.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Vertex.h"
#include "Rule.h"

namespace cga {

enum { DIRECTION_X = 0, DIRECTION_Y, SCOPE_SX, SCOPE_SY };
enum { REVOLVE_X = 0, REVOLVE_Y };

const float M_PI = 3.1415926f;

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
	virtual void split(int direction, const std::vector<float> ratios, const std::vector<std::string> names, std::vector<Object*>& objects);
	virtual void componentSplit(const std::string& front_name, Rectangle** front, const std::string& sides_name, std::vector<Rectangle*>& sides, const std::string& top_name, Polygon** top, const std::string& base_name, Polygon** base);
	virtual Object* roofHip(const std::string& name, float angle);
	virtual void generate(RenderManager* renderManager);
};

class CGA {
public:
	glm::mat4 modelMat;

public:
	CGA();

	void generate(RenderManager* renderManager, std::map<std::string, Rule*>& rules, std::list<Object*> stack);

	/*void generatePyramid(RenderManager* renderManager);*/
	void generateSimpleBuilding(RenderManager* renderManager);
	/*
	void generateHouse(RenderManager* renderManager);
	void generateBuilding(RenderManager* renderManager);
	void generateVase1(RenderManager* renderManager);
	void generateVase2(RenderManager* renderManager);
	void generateVase3(RenderManager* renderManager);
	void generateSaltShaker1(RenderManager* renderManager);
	void generateSaltShaker2(RenderManager* renderManager);
	*/
};

}
