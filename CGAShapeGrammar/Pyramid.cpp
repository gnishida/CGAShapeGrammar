#include "Pyramid.h"
#include "Rectangle.h"
#include "CGA.h"
#include "GLUtils.h"
#include "Polygon.h"

namespace cga {

Pyramid::Pyramid(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, const glm::vec2& center, float height, float top_ratio, const glm::vec3& color, const std::string& texture) {
	this->_name = name;
	this->_removed = false;
	this->_modelMat = modelMat;
	this->_points = points;
	this->_center = center;
	this->_height = height;
	this->_top_ratio = top_ratio;
	this->_color = color;
	this->_texture = texture;

	BoundingBox bbox(points);
	this->_scope = glm::vec3(bbox.upper_right.x, bbox.upper_right.y, height);
}

Object* Pyramid::clone(const std::string& name) {
	Object* copy = new Pyramid(*this);
	copy->_name = name;
	return copy;
}

void Pyramid::componentSplit(const std::string& front_name, Rectangle** front, const std::string& sides_name, std::vector<Rectangle*>& sides, const std::string& top_name, Polygon** top, const std::string& base_name, Polygon** base) {
	// front face (To be fixed)
	{
		*front = new Rectangle(front_name, glm::rotate(_modelMat, M_PI * 0.5f, glm::vec3(1, 0, 0)), glm::length(_points[1] - _points[0]), _height, _color);
	}

	// side faces (To be fixed);
	{
		glm::mat4 mat;
		sides.resize(_points.size() - 1);
		for (int i = 1; i < _points.size(); ++i) {
			glm::vec2 a = _points[i] - _points[i - 1];
			glm::vec2 b = _points[(i + 1) % _points.size()] - _points[i];

			mat = glm::translate(mat, glm::vec3(glm::length(a), 0, 0));
			float theta = acos(glm::dot(a, b) / glm::length(a) / glm::length(b));
			mat = glm::rotate(mat, theta, glm::vec3(0, 0, 1));
			glm::mat4 mat2 = glm::rotate(mat, M_PI * 0.5f, glm::vec3(1, 0, 0));
			glm::mat4 invMat = glm::inverse(mat2);

			std::vector<glm::vec2> sidePoints(4);
			sidePoints[0] = glm::vec2(invMat * glm::vec4(_points[i], 0, 1));
			sidePoints[1] = glm::vec2(invMat * glm::vec4(_points[(i + 1) % _points.size()], 0, 1));
			sidePoints[2] = glm::vec2(invMat * glm::vec4(_points[(i + 1) % _points.size()], _height, 1));
			sidePoints[3] = glm::vec2(invMat * glm::vec4(_points[i], _height, 1));

			sides[i - 1] = new Rectangle(sides_name, _modelMat * mat2, glm::length(_points[(i + 1) % _points.size()] - _points[i]), _height, _color);
		}
	}

	// top face
	{
		std::vector<glm::vec2> points(_points.size());
		for (int i = 0; i < _points.size(); ++i) {
			points[i] = glm::vec2(_points[i] * _top_ratio + _center * (1.0f - _top_ratio));
		}
		*top = new Polygon(top_name, glm::translate(_modelMat, glm::vec3(0, 0, _height)), points, _color, _texture);
	}

	// bottom face
	{
		std::vector<glm::vec2> basePoints = _points;
		std::reverse(basePoints.begin(), basePoints.end());
		*base = new Polygon(base_name, _modelMat, basePoints, _color, _texture);
	}
}

void Pyramid::generate(RenderManager* renderManager) {
	if (_removed) return;

	if (_top_ratio == 0.0f) {
		std::vector<Vertex> vertices(_points.size() * 3);

		glm::vec4 p0(_center, _height, 1);
		p0 = _modelMat * p0;

		glm::vec4 p1(_points.back(), 0, 1);
		p1 = _modelMat * p1;

		for (int i = 0; i < _points.size(); ++i) {
			glm::vec4 p2(_points[i], 0, 1);
			p2 = _modelMat * p2;

			glm::vec3 normal = glm::cross(glm::vec3(p1 - p0), glm::vec3(p2 - p0));

			vertices[i * 3] = Vertex(glm::vec3(p0), normal, _color, glm::vec3(0, 0, 0));
			vertices[i * 3 + 1] = Vertex(glm::vec3(p1), normal, _color, glm::vec3(0, 0, 0));
			vertices[i * 3 + 2] = Vertex(glm::vec3(p2), normal, _color, glm::vec3(0, 0, 0));

			p1 = p2;
		}

		renderManager->addObject(_name.c_str(), _texture.c_str(), vertices);
	} else {
		std::vector<Vertex> vertices(_points.size() * 6);

		glm::vec4 p0(_points.back(), 0, 1);
		p0 = _modelMat * p0;

		glm::vec4 p1(_points.back() * _top_ratio + _center * (1.0f - _top_ratio), _height, 1);
		p1 = _modelMat * p1;

		std::vector<glm::vec3> pts3(_points.size());
		for (int i = 0; i < _points.size(); ++i) {
			glm::vec4 p2(_points[i], 0, 1);
			p2 = _modelMat * p2;

			glm::vec4 p3(_points[i] * _top_ratio + _center * (1.0f - _top_ratio), _height, 1);
			pts3[i] = glm::vec3(p3);
			p3 = _modelMat * p3;

			glm::vec3 normal = glm::cross(glm::vec3(p2 - p0), glm::vec3(p3 - p0));

			vertices[i * 6 + 0] = Vertex(glm::vec3(p0), normal, _color, glm::vec3(0, 0, 0));
			vertices[i * 6 + 1] = Vertex(glm::vec3(p2), normal, _color, glm::vec3(0, 0, 0));
			vertices[i * 6 + 2] = Vertex(glm::vec3(p3), normal, _color, glm::vec3(0, 0, 0));

			vertices[i * 6 + 3] = Vertex(glm::vec3(p0), normal, _color, glm::vec3(0, 0, 0));
			vertices[i * 6 + 4] = Vertex(glm::vec3(p3), normal, _color, glm::vec3(0, 0, 0));
			vertices[i * 6 + 5] = Vertex(glm::vec3(p1), normal, _color, glm::vec3(0, 0, 0));

			p0 = p2;
			p1 = p3;
		}

		glutils::drawPolygon(pts3, _color, _modelMat, vertices);

		renderManager->addObject(_name.c_str(), _texture.c_str(), vertices);
	}

	/*if (showAxes) {
		std::vector<Vertex> vertices;
		glutils::drawAxes(0.1, 3, _modelMat, vertices);
		renderManager->addObject("axis", "", vertices);
	}*/
}


}
