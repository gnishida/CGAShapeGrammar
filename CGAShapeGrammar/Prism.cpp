#include "Prism.h"
#include "Rectangle.h"
#include "Polygon.h"

namespace cga {

Prism::Prism(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, float height, const glm::vec3& color) {
	this->_name = name;
	this->_removed = false;
	this->_modelMat = modelMat;
	this->_points = points;
	this->_height = height;
	this->_color = color;

	BoundingBox bbox(points);
	this->_scope = glm::vec3(bbox.upper_right.x, bbox.upper_right.y, height);
}

Object* Prism::clone(const std::string& name) {
	Object* copy = new Prism(*this);
	copy->_name = name;
	return copy;
}

void Prism::setupProjection(float texWidth, float texHeight) {
}

void Prism::split(int direction, const std::vector<float> ratios, const std::vector<std::string> names, std::vector<Object*>& objects) {
	glm::mat4 modelMat = this->_modelMat;

	for (int i = 0; i < ratios.size(); ++i) {
		Prism* obj = new Prism(*this);
		obj->_name = names[i];
		obj->_modelMat = modelMat;
		obj->_height = this->_height * ratios[i];
		objects.push_back(obj);

		modelMat = glm::translate(modelMat, glm::vec3(0, 0, obj->_height));
	}
}


void Prism::comp(const std::string& front_name, Rectangle** front, const std::string& sides_name, std::vector<Rectangle*>& sides, const std::string& top_name, Polygon** top, const std::string& base_name, Polygon** base) {
	// front face
	{
		*front = new Rectangle(front_name, glm::rotate(_modelMat, M_PI * 0.5f, glm::vec3(1, 0, 0)), glm::length(_points[1] - _points[0]), _height, _color);
	}

	// side faces
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
		*top = new Polygon(top_name, glm::translate(_modelMat, glm::vec3(0, 0, _height)), _points, _color, _texture);
	}

	// bottom face
	{
		std::vector<glm::vec2> basePoints = _points;
		std::reverse(basePoints.begin(), basePoints.end());
		*base = new Polygon(base_name, _modelMat, basePoints, _color, _texture);
	}
}

void Prism::generate(RenderManager* renderManager) {
	if (_removed) return;

	std::vector<Vertex> vertices((_points.size() - 2) * 6 + _points.size() * 6);

	int num = 0;

	// top
	{
		glm::vec4 p0(_points.back(), _height, 1);
		p0 = _modelMat * p0;
		glm::vec4 normal(0, 0, 1, 0);
		normal = _modelMat * normal;

		glm::vec4 p1(_points[0], _height, 1);
		p1 = _modelMat * p1;

		for (int i = 0; i < _points.size() - 2; ++i) {
			glm::vec4 p2(_points[i + 1], _height, 1);
			p2 = _modelMat * p2;

			vertices[i * 3] = Vertex(glm::vec3(p0), glm::vec3(normal), _color);
			vertices[i * 3 + 1] = Vertex(glm::vec3(p1), glm::vec3(normal), _color);
			vertices[i * 3 + 2] = Vertex(glm::vec3(p2), glm::vec3(normal), _color);

			p1 = p2;
		}

		num += (_points.size() - 2) * 3;
	}

	// bottom
	{
		glm::vec4 p0(_points.back(), 0, 1);
		p0 = _modelMat * p0;
		glm::vec4 normal(0, 0, -1, 0);
		normal = _modelMat * normal;

		glm::vec4 p1(_points[0], 0, 1);
		p1 = _modelMat * p1;

		for (int i = 0; i < _points.size() - 2; ++i) {
			glm::vec4 p2(_points[i + 1], 0, 1);
			p2 = _modelMat * p2;

			vertices[num + i * 3] = Vertex(glm::vec3(p0), glm::vec3(normal), _color);
			vertices[num + i * 3 + 1] = Vertex(glm::vec3(p2), glm::vec3(normal), _color);
			vertices[num + i * 3 + 2] = Vertex(glm::vec3(p1), glm::vec3(normal), _color);

			p1 = p2;
		}

		num += (_points.size() - 2) * 3;
	}

	// side
	{
		glm::vec4 p1(_points.back(), 0, 1);
		glm::vec4 p2(_points.back(), _height, 1);
		p1 = _modelMat * p1;
		p2 = _modelMat * p2;

		for (int i = 0; i < _points.size(); ++i) {
			glm::vec4 p3(_points[i], 0, 1);
			glm::vec4 p4(_points[i], _height, 1);
			p3 = _modelMat * p3;
			p4 = _modelMat * p4;

			glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(p3) - glm::vec3(p1), glm::vec3(p2) - glm::vec3(p1)));
			
			vertices[num + i * 6 + 0] = Vertex(glm::vec3(p1), normal, _color);
			vertices[num + i * 6 + 1] = Vertex(glm::vec3(p3), normal, _color);
			vertices[num + i * 6 + 2] = Vertex(glm::vec3(p4), normal, _color);

			vertices[num + i * 6 + 3] = Vertex(glm::vec3(p1), normal, _color);
			vertices[num + i * 6 + 4] = Vertex(glm::vec3(p4), normal, _color);
			vertices[num + i * 6 + 5] = Vertex(glm::vec3(p2), normal, _color);

			p1 = p3;
			p2 = p4;
		}
	}

	renderManager->addObject(_name.c_str(), "", vertices);

	/*if (showAxes) {
		vertices.resize(0);
		glutils::drawAxes(0.1, 3, _modelMat, vertices);
		renderManager->addObject("axis", "", vertices);
	}*/
}

}
