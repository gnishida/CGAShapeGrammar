#include "CGA.h"
#include "GLUtils.h"
#include "OBJLoader.h"
#include <map>

#include "Rectangle.h"
#include "Polygon.h"
#include "GeneralObject.h"

namespace cga {

const bool showAxes = false;

BoundingBox::BoundingBox(const std::vector<glm::vec2>& points) {
	minPt.x = (std::numeric_limits<float>::max)();
	minPt.y = (std::numeric_limits<float>::max)();
	minPt.z = 0.0f;
	maxPt.x = -(std::numeric_limits<float>::max)();
	maxPt.y = -(std::numeric_limits<float>::max)();
	maxPt.z = 0.0f;

	for (int i = 0; i < points.size(); ++i) {
		minPt.x = min(minPt.x, points[i].x);
		minPt.y = min(minPt.y, points[i].y);
		maxPt.x = max(maxPt.x, points[i].x);
		maxPt.y = max(maxPt.y, points[i].y);
	}
}

BoundingBox::BoundingBox(const std::vector<glm::vec3>& points) {
	minPt.x = (std::numeric_limits<float>::max)();
	minPt.y = (std::numeric_limits<float>::max)();
	minPt.z = (std::numeric_limits<float>::max)();
	maxPt.x = -(std::numeric_limits<float>::max)();
	maxPt.y = -(std::numeric_limits<float>::max)();
	maxPt.z = -(std::numeric_limits<float>::max)();

	for (int i = 0; i < points.size(); ++i) {
		minPt.x = min(minPt.x, points[i].x);
		minPt.y = min(minPt.y, points[i].y);
		minPt.z = min(minPt.y, points[i].z);
		maxPt.x = max(maxPt.x, points[i].x);
		maxPt.y = max(maxPt.y, points[i].y);
		maxPt.z = max(maxPt.y, points[i].z);
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

Object* Object::insert(const std::string& name, const std::string& geometryPath) {
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> texCoords;
	OBJLoader::load(geometryPath.c_str(), points, normals, texCoords);

	// compute scale
	float scaleX = 1.0f;
	float scaleY = 1.0f;
	float scaleZ = 1.0f;

	BoundingBox bbox(points);
	if (_scope.z == 0) {
		scaleX = _scope.x / bbox.sx();
		scaleY = _scope.y / bbox.sy();
		scaleZ = (scaleX + scaleY) * 0.5f;
	}

	// scale the points
	for (int i = 0; i < points.size(); ++i) {
		points[i].x = points[i].x * scaleX;
		points[i].y = points[i].y * scaleY;
		points[i].z = points[i].z * scaleZ;
	}

	return new GeneralObject(name, _modelMat, points, normals, _color);
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

Object* Object::shapeL(const std::string& name, float frontWidth, float leftWidth) {
	throw "shapeL() is not supported.";
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

void Object::generate(RenderManager* renderManager, bool showAxes) {
	throw "generate() is not supported.";
}

void Object::drawAxes(RenderManager* renderManager, const glm::mat4& modelMat) {
	std::vector<Vertex> vertices;
	glutils::drawAxes(0.1, 3, modelMat, vertices);
	renderManager->addObject("axis", "", vertices);
}

CGA::CGA() {
}

void CGA::generate(RenderManager* renderManager, std::map<std::string, Rule>& rules, std::list<Object*> stack, bool showAxes) {
	while (!stack.empty()) {
		Object* obj = stack.front();
		stack.pop_front();

		if (rules.find(obj->_name) == rules.end()) {
			obj->generate(renderManager, showAxes);
			delete obj;
		} else {
			rules[obj->_name].apply(obj, stack);
		}
	}

}

}
