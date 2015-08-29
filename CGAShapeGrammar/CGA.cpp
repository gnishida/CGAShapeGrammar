#include "CGA.h"
#include "GLUtils.h"
#include <map>

#include "Rectangle.h"
#include "Polygon.h"


namespace cga {

const bool showAxes = false;

BoundingBox::BoundingBox(const std::vector<glm::vec2>& points) {
	bottom_left = glm::vec2((std::numeric_limits<float>::max)(), (std::numeric_limits<float>::max)());
	upper_right = -bottom_left;

	for (int i = 0; i < points.size(); ++i) {
		bottom_left.x = min(bottom_left.x, points[i].x);
		bottom_left.y = min(bottom_left.y, points[i].y);
		upper_right.x = max(upper_right.x, points[i].x);
		upper_right.y = max(upper_right.y, points[i].y);
	}
}

Object* Object::clone(const std::string& name) {
	throw "clone() is not supported.";
}

void Object::comp(const std::string& front_name, Object** front, const std::string& sides_name, std::vector<Object*>& sides, const std::string& top_name, Object** top, const std::string& bottom_name, Object** bottom) {
	throw "componentSplit() is not supported.";
}

Object* Object::extrude(const std::string& name, float height) {
	throw "extrude() is not supported.";
}

Object* Object::inscribeCircle(const std::string& name) {
	throw "inscribeCircle() is not supported.";
}

void Object::nil() {
	_removed = true;
}

Object* Object::offset(const std::string& name, float offsetRatio) {
	throw "offset() is not supported.";
}

Object* Object::roofHip(const std::string& name, float angle) {
	throw "roofHip() is not supported.";
}

void Object::rotate(const std::string& name, float xAngle, float yAngle, float zAngle) {
	_modelMat = glm::rotate(_modelMat, xAngle * M_PI / 180.0f, glm::vec3(1, 0, 0));
	_modelMat = glm::rotate(_modelMat, yAngle * M_PI / 180.0f, glm::vec3(0, 1, 0));
	_modelMat = glm::rotate(_modelMat, zAngle * M_PI / 180.0f, glm::vec3(0, 0, 1));
}

void Object::setupProjection(float texWidth, float texHeight) {
	throw "setupProjection() is not supported.";
}

void Object::split(int direction, const std::vector<float> ratios, const std::vector<std::string> names, std::vector<Object*>& objects) {
	throw "split() is not supported.";
}

Object* Object::taper(const std::string& name, float height, float top_ratio) {
	throw "taper() is not supported.";
}

void Object::texture(const std::string& tex) {
	this->_texture = tex;
	_textureEnabled = true;
}

void Object::translate(const glm::vec3& v) {
	_modelMat = glm::translate(_modelMat, v);
}

void Object::generate(RenderManager* renderManager) {
	throw "generate() is not supported.";
}

CGA::CGA() {
}

void CGA::generate(RenderManager* renderManager, std::map<std::string, Rule*>& rules, std::list<Object*> stack) {
	while (!stack.empty()) {
		Object* obj = stack.front();
		stack.pop_front();

		if (rules.find(obj->_name) == rules.end()) {
			obj->generate(renderManager);

		} else {
			rules[obj->_name]->apply(obj, stack);
		}
		delete obj;
	}

}

}
