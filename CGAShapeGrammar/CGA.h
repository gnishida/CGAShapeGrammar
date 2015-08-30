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

/*class Polygon;
class Rectangle;*/

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
	virtual Object* clone(const std::string& name);
	virtual void comp(const std::string& front_name, Object** front, const std::string& sides_name, std::vector<Object*>& sides, const std::string& top_name, Object** top, const std::string& bottom_name, Object** bottom);
	virtual Object* extrude(const std::string& name, float height);
	virtual Object* inscribeCircle(const std::string& name);
	void nil();
	virtual Object* offset(const std::string& name, float offsetRatio);
	virtual Object* roofHip(const std::string& name, float angle);
	void rotate(const std::string& name, float xAngle, float yAngle, float zAngle);
	virtual void setupProjection(float texWidth, float texHeight);
	virtual Object* shapeL(const std::string& name, float frontWidth, float leftWidth);
	virtual void split(int direction, const std::vector<float> ratios, const std::vector<std::string> names, std::vector<Object*>& objects);
	virtual Object* taper(const std::string& name, float height, float top_ratio = 0.0f);
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

	void generate(RenderManager* renderManager, std::map<std::string, Rule>& rules, std::list<Object*> stack, bool showAxes = false);
};

}
