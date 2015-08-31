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

class BoundingBox {
public:
	glm::vec3 minPt;
	glm::vec3 maxPt;

public:
	BoundingBox(const std::vector<glm::vec2>& points);
	BoundingBox(const std::vector<glm::vec3>& points);
	float sx() { return maxPt.x - minPt.x; }
	float sy() { return maxPt.y - minPt.y; }
	float sz() { return maxPt.z - minPt.z; }
};

class Shape {
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
	virtual Shape* clone(const std::string& name);
	virtual void comp(const std::string& front_name, Shape** front, const std::string& sides_name, std::vector<Shape*>& sides, const std::string& top_name, Shape** top, const std::string& bottom_name, Shape** bottom);
	virtual Shape* extrude(const std::string& name, float height);
	virtual Shape* inscribeCircle(const std::string& name);
	Shape* insert(const std::string& name, const std::string& geometryPath);
	void nil();
	virtual Shape* offset(const std::string& name, float offsetRatio);
	virtual Shape* roofHip(const std::string& name, float angle);
	void rotate(const std::string& name, float xAngle, float yAngle, float zAngle);
	virtual void setupProjection(float texWidth, float texHeight);
	virtual Shape* shapeL(const std::string& name, float frontWidth, float leftWidth);
	virtual void size(const glm::vec3& sz);
	virtual void split(int direction, const std::vector<float>& sizes, const std::vector<std::string>& names, std::vector<Shape*>& objects);
	virtual Shape* taper(const std::string& name, float height, float top_ratio = 0.0f);
	void texture(const std::string& tex);
	void translate(const glm::vec3& v);
	virtual void generate(RenderManager* renderManager, bool showAxes);

protected:
	void drawAxes(RenderManager* renderManager, const glm::mat4& modelMat);
};

class CGA {
public:
	glm::mat4 modelMat;

public:
	CGA();

	void generate(RenderManager* renderManager, std::map<std::string, Rule>& rules, std::list<Shape*> stack, bool showAxes = false);
};

}
