#pragma once

#include "RenderManager.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Vertex.h"

namespace cga {

enum { DIRECTION_X = 0, DIRECTION_Y, SCOPE_SX, SCOPE_SY };

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
	glm::mat4 modelMat;
	glm::vec3 color;
	bool textureEnabled;
	std::string texture;
	std::vector<glm::vec2> texCoords;
	glm::vec3 scope;

public:
	void translate(const glm::vec3& v);
	void setTexture(const std::string& texture);
	virtual Object* clone();
	virtual void setupProjection(float texWidth, float texHeight);
	virtual void setupProjection(float u1, float v1, float u2, float v2);
	virtual Object* extrude(const std::string& name, float height);
	virtual void split(int direction, const std::vector<float> sizes, const std::vector<std::string> names, std::vector<Rectangle*>& rectangles);
	virtual void componentSplit(const std::string& front_name, Rectangle** front, const std::string& sides_name, std::vector<Rectangle*>& sides, const std::string& top_name, Polygon** top, const std::string& base_name, Polygon** base);
	virtual Object* revolve(const std::string& name);
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
	void split(int direction, const std::vector<float> sizes, const std::vector<std::string> names, std::vector<Rectangle*>& rectangles);
	void componentSplit(const std::string& front_name, Rectangle** front, const std::string& sides_name, std::vector<Rectangle*>& sides, const std::string& top_name, Polygon** top, const std::string& base_name, Polygon** base);
	void generate(RenderManager* renderManager);
};

class Rectangle : public Object {
public:
	float width;
	float height;

public:
	Rectangle() {}
	Rectangle(const std::string& name, const glm::mat4& modelMat, float width, float height, const glm::vec3& color, const std::string& texture);
	Object* clone();
	void setupProjection(float texWidth, float texHeight);
	void setupProjection(float u1, float v1, float u2, float v2);
	Object* extrude(const std::string& name, float height);
	void split(int direction, const std::vector<float> sizes, const std::vector<std::string> names, std::vector<Rectangle*>& rectangles);
	void generate(RenderManager* renderManager);
};

class Polygon : public Object {
private:
	std::vector<glm::vec2> points;

public:
	Polygon() {}
	Polygon(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2> points, const glm::vec3& color, const std::string& texture);
	Object* clone();
	void setupProjection(float texWidth, float texHeight);
	Object* extrude(const std::string& name, float height);
	void split(int direction, const std::vector<float> sizes, const std::vector<std::string> names, std::vector<Rectangle*>& rectangles);
	void generate(RenderManager* renderManager);
};

class SineCurve : public Object {
private:
	float width;
	float height;

public:
	SineCurve(const std::string& name, float width, float height);
	Object* revolve(const std::string& name);
};

class SineCurveRevolved : public Object {
private:
	float width;
	float height;

public:
	SineCurveRevolved(const std::string& name, float width, float height);
	void generate(RenderManager* renderManager);
};

class Value  {
public:
	static enum { TYPE_ABSOLUTE = 0, TYPE_RELATIVE, TYPE_FLOATING, TYPE_SET };

public:
	int type;
	float value;
	std::vector<Value*> values;
	bool repeat;

public:
	virtual float getEstimateValue(float size) = 0;
};

class SingleValue : public Value {
public:
	SingleValue(int type, float value) { this->type = type; this->value = value; this->repeat = false; }

	float getEstimateValue(float size) { return value; }
};


class ValueSet : public Value {
public:
	ValueSet(Value* value, bool repeat = false);
	ValueSet(const std::vector<Value*>& values, bool repeat = false);

	float getEstimateValue(float size);
};

class Rule {
protected:
	std::string output_name;

public:
	Rule() {}

	virtual void apply(Object* obj, std::list<Object*>& stack) = 0;
	void decodeSplitSizes(float size, const std::vector<Value*>& sizes, const std::vector<std::string>& output_names, std::vector<float>& decoded_sizes, std::vector<std::string>& decoded_output_names);
};

class ExtrudeRule : public Rule {
private:
	float height;

public:
	ExtrudeRule(float height, const std::string& output_name);

	void apply(Object* obj, std::list<Object*>& stack);
};

class ComponentSplitRule : public Rule {
private:
	std::string front_name;
	std::string side_name;
	std::string top_name;
	std::string bottom_name;

public:
	ComponentSplitRule(const std::string& front_name, const std::string& side_name, const std::string& top_name, const std::string& bottom_name);
	void apply(Object* obj, std::list<Object*>& stack);
};

class SplitRule : public Rule {
private:
	int direction;
	std::vector<Value*> sizes;
	std::vector<std::string> output_names;

public:
	SplitRule(int direction, const std::vector<Value*>& sizes, const std::vector<std::string>& output_names);
	void apply(Object* obj, std::list<Object*>& stack);
};

class SetupProjectionRule : public Rule {
public:
	static enum { TYPE_ABSOLUTE = 0, TYPE_RELATIVE };

private:
	int type;
	float texWidth;
	float texHeight;

public:
	SetupProjectionRule(int type, float texWidth, float texHeight, const std::string& output_name);
	//SetupProjectionRule(float u1, float v1, float u2, float v2, const std::string& output_name);
	void apply(Object* obj, std::list<Object*>& stack);
};

class SetTextureRule : public Rule {
private:
	std::string texture;

public:
	SetTextureRule(const std::string& texture, const std::string& output_name);
	void apply(Object* obj, std::list<Object*>& stack);
};

class TranslateRule : public Rule {
private:
	glm::vec3 vec;

public:
	TranslateRule(const glm::vec3& vec, const std::string& output_name);
	void apply(Object* obj, std::list<Object*>& stack);
};

class RevolveRule : public Rule {
public:
	RevolveRule(const std::string& output_name);
	void apply(Object* obj, std::list<Object*>& stack);
};

class CGA {
private:
	std::map<std::string, Rule*> rules;

public:
	CGA();

	void generate(RenderManager* renderManager);
	std::map<std::string, Rule*> buildingRule();
	std::map<std::string, Rule*> vaseRule();
};

}
