#include "Polygon.h"
#include "Pyramid.h"
#include "HipRoof.h"
#include "Prism.h"

namespace cga {


Polygon::Polygon(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, const glm::vec3& color, const std::string& texture) {
	this->_name = name;
	this->_removed = false;
	this->_modelMat = modelMat;
	this->_points = points;
	this->_color = color;
	this->_texture = texture;

	BoundingBox bbox(points);
	this->_scope = glm::vec3(bbox.upper_right.x, bbox.upper_right.y, 0);

	this->_center = glm::vec2(0, 0);
	for (int i = 0; i < points.size(); ++i) {
		_center += points[i];
	}
	_center /= points.size();
}

Object* Polygon::clone(const std::string& name) {
	Object* copy = new Polygon(*this);
	copy->_name = name;
	return copy;
}


void Polygon::setupProjection(float texWidth, float texHeight) {
	_texCoords.resize(_points.size());
	for (int i = 0; i < _points.size(); ++i) {
		_texCoords[i] = glm::vec2(_points[i].x / texWidth, _points[i].y / texHeight);
	}
}

Object* Polygon::extrude(const std::string& name, float height) {
	return new Prism(name, _modelMat, _points, height, _color);
}

Object* Polygon::taper(const std::string& name, float height, float top_ratio) {
	return new Pyramid(name, _modelMat, _points, _center, height, top_ratio, _color, _texture);
}

Object* Polygon::offset(const std::string& name, float offsetRatio) {
	glm::vec2 t = glm::vec2(_points[0] * (1.0f - offsetRatio) + _center * offsetRatio) - _points[0];

	std::vector<glm::vec2> points(_points.size());
	for (int i = 0; i < _points.size(); ++i) {
		points[i] = glm::vec2(_points[i] * (1.0f - offsetRatio) + _center * offsetRatio) - t;
	}

	glm::mat4 mat = glm::translate(_modelMat, glm::vec3(t, 0));
	return new Polygon(name, mat, points, _color, _texture);
}

Object* Polygon::inscribeCircle(const std::string& name) {
	return NULL;
}

Object* Polygon::roofHip(const std::string& name, float angle) {
	return new HipRoof(name, _modelMat, _points, angle, _color);
}

void Polygon::generate(RenderManager* renderManager) {
	if (_removed) return;

	std::vector<Vertex> vertices((_points.size() - 2) * 3);

	glm::vec4 p0(_points[0], 0, 1);
	p0 = _modelMat * p0;
	glm::vec4 normal(0, 0, 1, 0);
	normal = _modelMat * normal;

	glm::vec4 p1(_points[1], 0, 1);
	p1 = _modelMat * p1;

	glm::vec3 uv1(_points[1].x / _scope.x, _points[1].y / _scope.y, 0);

	for (int i = 1; i < _points.size() - 1; ++i) {
		glm::vec4 p2(_points[i + 1], 0, 1);
		p2 = _modelMat * p2;

		glm::vec3 uv2(_points[i + 1].x / _scope.x, _points[i + 1].y / _scope.y, 0);

		vertices[(i - 1) * 3] = Vertex(glm::vec3(p0), glm::vec3(normal), _color, glm::vec3(0, 0, 0));
		vertices[(i - 1) * 3 + 1] = Vertex(glm::vec3(p1), glm::vec3(normal), _color, uv1);
		vertices[(i - 1) * 3 + 2] = Vertex(glm::vec3(p2), glm::vec3(normal), _color, uv2);

		p1 = p2;
		uv1 = uv2;
	}

	renderManager->addObject(_name.c_str(), _texture.c_str(), vertices);

	/*if (showAxes) {
		vertices.resize(0);
		glutils::drawAxes(0.1, 3, _modelMat, vertices);
		renderManager->addObject("axis", "", vertices);
	}*/
}

}
