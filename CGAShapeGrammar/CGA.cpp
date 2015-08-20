#include "CGA.h"
#include "GLUtils.h"
#include <map>

namespace cga {

const float M_PI = 3.1415926f;
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

void Object::translate(const glm::vec3& v) {
	_modelMat = glm::translate(_modelMat, v);
}

void Object::rotate(float xAngle, float yAngle, float zAngle) {
	_modelMat = glm::rotate(_modelMat, xAngle * M_PI / 180.0f, glm::vec3(1, 0, 0));
	_modelMat = glm::rotate(_modelMat, yAngle * M_PI / 180.0f, glm::vec3(0, 1, 0));
	_modelMat = glm::rotate(_modelMat, zAngle * M_PI / 180.0f, glm::vec3(0, 0, 1));
}

void Object::nil() {
	_removed = true;
}

void Object::setTexture(const std::string& texture) {
	this->_texture = texture;
	_textureEnabled = true;
}

Object* Object::clone() {
	throw "clone() is not supported.";
}

void Object::setupProjection(float texWidth, float texHeight) {
	throw "setupProjection() is not supported.";
}

Object* Object::extrude(const std::string& name, float height) {
	throw "extrude() is not supported.";
}

Object* Object::taper(const std::string& name, float height, float top_ratio) {
	throw "taper() is not supported.";
}

Object* Object::offset(const std::string& name, float offsetRatio) {
	throw "offset() is not supported.";
}

Object* offset(const std::string& name, float offsetRatio) {
	throw "offset() is not supported.";
}

Object* Object::revolve(const std::string& name, int direction) {
	throw "revolve() is not supported.";
}

void Object::split(int direction, const std::vector<float> ratios, const std::vector<std::string> names, std::vector<Object*>& objects) {
	throw "split() is not supported.";
}

void Object::componentSplit(const std::string& front_name, Rectangle** front, const std::string& sides_name, std::vector<Rectangle*>& sides, const std::string& top_name, Polygon** top, const std::string& base_name, Polygon** base) {
	throw "componentSplit() is not supported.";
}

void Object::generate(RenderManager* renderManager) {
	throw "generate() is not supported.";
}

Line::Line(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, const glm::vec3& color) {
	this->_name = name;
	this->_removed = false;
	this->_modelMat = modelMat;
	this->_points = points;
	this->_color = color;
	this->_textureEnabled = false;

	BoundingBox bbox(points);
	this->_scope = glm::vec3(bbox.upper_right.x, bbox.upper_right.y, 0);
}

Object* Line::clone() {
	return new Line(*this);
}

Object* Line::revolve(const std::string& name, int direction) {
	return new RevolvedLine(name, _modelMat, _points, direction, _color);
}

void Line::generate(RenderManager* renderManager) {
	// render nothing
}

RevolvedLine::RevolvedLine(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, int direction, const glm::vec3& color) {
	this->_name = name;
	this->_removed = false;
	this->_modelMat = modelMat;
	this->_points = points;
	this->_direction = direction;
	this->_color = color;
	this->_textureEnabled = false;

	BoundingBox bbox(points);
	this->_scope = glm::vec3(bbox.upper_right.x, bbox.upper_right.y, bbox.upper_right.y);
}

Object* RevolvedLine::clone() {
	return new RevolvedLine(*this);
}

void RevolvedLine::generate(RenderManager* renderManager) {
	std::vector<Vertex> vertices;

	if (_direction == REVOLVE_X) {
		glm::mat4 mat = _modelMat;
		for (int i = 0; i < _points.size() - 1; ++i) {
			glutils::drawCylinderX(_points[i].y, _points[i + 1].y, _points[i + 1].x - _points[i].x, _color, mat, vertices);

			mat = glm::translate(mat, glm::vec3(_points[i + 1].x - _points[i].x, 0, 0));
		}
	} else {
		glm::mat4 mat = _modelMat;
		for (int i = 0; i < _points.size() - 1; ++i) {
			glutils::drawCylinderY(_points[i].x, _points[i + 1].x, _points[i + 1].y - _points[i].y, _color, mat, vertices);

			mat = glm::translate(mat, glm::vec3(0, _points[i + 1].y - _points[i].y, 0));
		}
	}

	renderManager->addObject(_name.c_str(), "", vertices);
}

Circle::Circle(const std::string& name, const glm::mat4& modelMat, const glm::vec2& center, float radius, const glm::vec3& color) {
	this->_name = name;
	this->_removed = false;
	this->_modelMat = modelMat;
	this->_center = center;
	this->_radius = _radius;
	this->_color = color;
}

PrismObject::PrismObject(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, float height, const glm::vec3& color) {
	this->_name = name;
	this->_removed = false;
	this->_modelMat = modelMat;
	this->_points = points;
	this->_height = height;
	this->_color = color;

	BoundingBox bbox(points);
	this->_scope = glm::vec3(bbox.upper_right.x, bbox.upper_right.y, height);
}

Object* PrismObject::clone() {
	return new PrismObject(*this);
}

void PrismObject::setupProjection(float texWidth, float texHeight) {
}

void PrismObject::split(int direction, const std::vector<float> ratios, const std::vector<std::string> names, std::vector<Object*>& objects) {
	throw "PrismObject does not support split operation.";
}


void PrismObject::componentSplit(const std::string& front_name, Rectangle** front, const std::string& sides_name, std::vector<Rectangle*>& sides, const std::string& top_name, Polygon** top, const std::string& base_name, Polygon** base) {
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

void PrismObject::generate(RenderManager* renderManager) {
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

	if (showAxes) {
		vertices.resize(0);
		glutils::drawAxes(0.1, 3, _modelMat, vertices);
		renderManager->addObject("axis", "", vertices);
	}
}

Rectangle::Rectangle(const std::string& name, const glm::mat4& modelMat, float width, float height, const glm::vec3& color) {
	this->_name = name;
	this->_removed = false;
	this->_modelMat = modelMat;
	this->_width = width;
	this->_height = height;
	this->_color = color;
	this->_scope = glm::vec3(width, height, 0);
	this->_textureEnabled = false;
}

Rectangle::Rectangle(const std::string& name, const glm::mat4& modelMat, float width, float height, const std::string& texture, float u1, float v1, float u2, float v2) {
	this->_name = name;
	this->_removed = false;
	this->_modelMat = modelMat;
	this->_width = width;
	this->_height = height;
	this->_texture = texture;
	this->_scope = glm::vec3(width, height, 0);

	_texCoords.resize(4);
	_texCoords[0] = glm::vec2(u1, v1);
	_texCoords[1] = glm::vec2(u2, v1);
	_texCoords[2] = glm::vec2(u2, v2);
	_texCoords[3] = glm::vec2(u1, v2);
	this->_textureEnabled = true;
}

Object* Rectangle::clone() {
	return new Rectangle(*this);
}

void Rectangle::setupProjection(float texWidth, float texHeight) {
	_texCoords.resize(4);
	_texCoords[0] = glm::vec2(0, 0);
	_texCoords[1] = glm::vec2(_width / texWidth, 0);
	_texCoords[2] = glm::vec2(_width / texWidth, _height / texHeight);
	_texCoords[3] = glm::vec2(0, _height / texHeight);
}

Object* Rectangle::extrude(const std::string& name, float height) {
	std::vector<glm::vec2> points(4);
	points[0] = glm::vec2(0, 0);
	points[1] = glm::vec2(_width, 0);
	points[2] = glm::vec2(_width, this->_height);
	points[3] = glm::vec2(0, _height);
	return new PrismObject(name, _modelMat, points, height, _color);
}

Object* Rectangle::taper(const std::string& name, float height, float top_ratio) {
	std::vector<glm::vec2> points(4);
	points[0] = glm::vec2(0, 0);
	points[1] = glm::vec2(_width, 0);
	points[2] = glm::vec2(_width, _height);
	points[3] = glm::vec2(0, _height);
	return new Pyramid(name, _modelMat, points, glm::vec2(_width * 0.5, _height * 0.5), height, top_ratio, _color, _texture);
}

Object* Rectangle::offset(const std::string& name, float offsetRatio) {
	glm::mat4 mat = glm::translate(_modelMat, glm::vec3(_scope.x * 0.5f * offsetRatio, _scope.y * 0.5f * offsetRatio, 0));
	if (_textureEnabled) {
		return new Rectangle(name, mat, _width * (1.0f - offsetRatio), _height * (1.0f - offsetRatio), _texture, _texCoords[0].x, _texCoords[0].y, _texCoords[2].x, _texCoords[2].y);
	} else {
		return new Rectangle(name, mat, _width * (1.0f - offsetRatio), _height * (1.0f - offsetRatio), _color);
	}
}

Object* Rectangle::inscribeCircle(const std::string& name) {
}

void Rectangle::split(int direction, const std::vector<float> ratios, const std::vector<std::string> names, std::vector<Object*>& objects) {
	objects.resize(ratios.size());

	float offset = 0.0f;
	
	for (int i = 0; i < ratios.size(); ++i) {
		glm::mat4 mat;
		if (direction == DIRECTION_X) {
			mat = glm::translate(glm::mat4(), glm::vec3(offset, 0, 0));
			if (_textureEnabled) {
				objects[i] = new Rectangle(names[i], _modelMat * mat, _width * ratios[i], _height, _texture, 
					_texCoords[0].x + (_texCoords[1].x - _texCoords[0].x) * offset / _width, _texCoords[0].y,
					_texCoords[0].x + (_texCoords[1].x - _texCoords[0].x) * (offset / _width + ratios[i]), _texCoords[2].y);
			} else {
				objects[i] = new Rectangle(names[i], _modelMat * mat, _width * ratios[i], _height, _color); 
			}
			offset += _width * ratios[i];
		} else {
			mat = glm::translate(glm::mat4(), glm::vec3(0, offset, 0));
			if (_textureEnabled) {
				objects[i] = new Rectangle(names[i], _modelMat * mat, _width, _height * ratios[i], _texture,
					_texCoords[0].x, _texCoords[0].y + (_texCoords[2].y - _texCoords[0].y) * offset / _height,
					_texCoords[1].x, _texCoords[0].y + (_texCoords[2].y - _texCoords[0].y) * (offset / _height + ratios[i]));
			} else {
				objects[i] = new Rectangle(names[i], _modelMat * mat, _width, _height * ratios[i], _color);
			}
			offset += _height * ratios[i];
		}
	}
}

void Rectangle::generate(RenderManager* renderManager) {
	if (_removed) return;

	std::vector<Vertex> vertices;

	vertices.resize(6);

	glm::vec4 p1(0, 0, 0, 1);
	p1 = _modelMat * p1;
	glm::vec4 p2(_width, 0, 0, 1);
	p2 = _modelMat * p2;
	glm::vec4 p3(_width, _height, 0, 1);
	p3 = _modelMat * p3;
	glm::vec4 p4(0, _height, 0, 1);
	p4 = _modelMat * p4;

	glm::vec4 normal(0, 0, 1, 0);
	normal = _modelMat * normal;

	if (_textureEnabled) {
		vertices[0] = Vertex(glm::vec3(p1), glm::vec3(normal), _color, glm::vec3(_texCoords[0], 0));
		vertices[1] = Vertex(glm::vec3(p2), glm::vec3(normal), _color, glm::vec3(_texCoords[1], 0));
		vertices[2] = Vertex(glm::vec3(p3), glm::vec3(normal), _color, glm::vec3(_texCoords[2], 0));

		vertices[3] = Vertex(glm::vec3(p1), glm::vec3(normal), _color, glm::vec3(_texCoords[0], 0));
		vertices[4] = Vertex(glm::vec3(p3), glm::vec3(normal), _color, glm::vec3(_texCoords[2], 0));
		vertices[5] = Vertex(glm::vec3(p4), glm::vec3(normal), _color, glm::vec3(_texCoords[3], 0));

		renderManager->addObject(_name.c_str(), _texture.c_str(), vertices);
	} else {
		vertices[0] = Vertex(glm::vec3(p1), glm::vec3(normal), _color);
		vertices[1] = Vertex(glm::vec3(p2), glm::vec3(normal), _color);
		vertices[2] = Vertex(glm::vec3(p3), glm::vec3(normal), _color);

		vertices[3] = Vertex(glm::vec3(p1), glm::vec3(normal), _color);
		vertices[4] = Vertex(glm::vec3(p3), glm::vec3(normal), _color);
		vertices[5] = Vertex(glm::vec3(p4), glm::vec3(normal), _color);

		renderManager->addObject(_name.c_str(), "", vertices);
	}
	
	vertices.resize(0);
	if (showAxes) {
		glutils::drawAxes(0.1, 3, _modelMat, vertices);
		renderManager->addObject("axis", "", vertices);
	}
}

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

Object* Polygon::clone() {
	return new Polygon(*this);
}


void Polygon::setupProjection(float texWidth, float texHeight) {
	_texCoords.resize(_points.size());
	for (int i = 0; i < _points.size(); ++i) {
		_texCoords[i] = glm::vec2(_points[i].x / texWidth, _points[i].y / texHeight);
	}
}

Object* Polygon::extrude(const std::string& name, float height) {
	return new PrismObject(name, _modelMat, _points, height, _color);
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

	if (showAxes) {
		vertices.resize(0);
		glutils::drawAxes(0.1, 3, _modelMat, vertices);
		renderManager->addObject("axis", "", vertices);
	}
}

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

	if (showAxes) {
		std::vector<Vertex> vertices;
		glutils::drawAxes(0.1, 3, _modelMat, vertices);
		renderManager->addObject("axis", "", vertices);
	}
}

CGA::CGA() {
}

void CGA::generatePyramid(RenderManager* renderManager) {
	std::list<Object*> stack;
	
	Rectangle* lot = new Rectangle("Lot", modelMat, 20, 20, glm::vec3(1, 1, 1));
	stack.push_back(lot);
	
	while (!stack.empty()) {
		Object* obj = stack.front();
		stack.pop_front();

		if (obj->_removed) {
			delete obj;
			continue;
		}
		
		if (obj->_name == "Lot") {
			stack.push_back(obj->taper("pyramid", 10));
		} else {
			obj->generate(renderManager);
			delete obj;
		}
	}
}

void CGA::generateSimpleBuilding(RenderManager* renderManager) {
	std::list<Object*> stack;
	
	Rectangle* lot = new Rectangle("Lot", modelMat, 35, 10, glm::vec3(1, 1, 1));
	stack.push_back(lot);
	
	while (!stack.empty()) {
		Object* obj = stack.front();
		stack.pop_front();

		if (obj->_removed) {
			delete obj;
			continue;
		}
		
		if (obj->_name == "Lot") {
			std::vector<float> ratios(5);
			for (int i = 0; i < ratios.size(); ++i) ratios[i] = 1.0f / ratios.size();
			std::vector<std::string> names(5);
			for (int i = 0; i < names.size(); ++i) names[i] = "SmallLot";
			std::vector<Object*> polygons;
			obj->split(DIRECTION_X, ratios, names, polygons);

			for (int i = 0; i < polygons.size(); ++i) {
				if (i % 2 == 1) {
					polygons[i]->nil();
				}
			}

			stack.insert(stack.end(), polygons.begin(), polygons.end());
		} else if (obj->_name == "SmallLot") {
			stack.push_back(obj->extrude("Building", 20));
		} else {
			obj->generate(renderManager);
			delete obj;
		}
	}
}

void CGA::generateBuilding(RenderManager* renderManager) {
	std::list<Object*> stack;
	
	Rectangle* lot = new Rectangle("Lot", modelMat, 35, 10, glm::vec3(1, 1, 1));
	stack.push_back(lot);
	
	while (!stack.empty()) {
		Object* obj = stack.front();
		stack.pop_front();

		if (obj->_removed) {
			delete obj;
			continue;
		}
		
		if (obj->_name == "Lot") {
			stack.push_back(obj->extrude("Building", 11));
		} else if (obj->_name == "Building") {
			Rectangle* frontFacade;
			Polygon* roof;
			Polygon* base;
			std::vector<Rectangle*> sideFacades;
			obj->componentSplit("FrontFacade", &frontFacade, "SideFacade", sideFacades, "Roof", &roof, "Base", &base);

			stack.push_back(frontFacade);
			for (int i = 0; i < sideFacades.size(); ++i) {
				stack.push_back(sideFacades[i]);
			}
			stack.push_back(roof);
			stack.push_back(base);
		} else if (obj->_name == "Roof") {
			obj->setupProjection(obj->_scope.x, obj->_scope.y);
			obj->setTexture("textures/roof.jpg");

			Object* obj2 = obj->clone();
			obj2->_name = "Roof_1";
			stack.push_back(obj2);
		} else if (obj->_name == "FrontFacade") {
			obj->setupProjection(2.5f, 1.0f);
			obj->setTexture("textures/brick.jpg");

			std::vector<float> floor_ratios(3);
			floor_ratios[0] = 4.0f / obj->_scope.y;
			floor_ratios[1] = 3.5f / obj->_scope.y;
			floor_ratios[2] = 3.5f / obj->_scope.y;
			std::vector<std::string> floor_names(3);
			floor_names[0] = "Floor";
			floor_names[1] = "Floor";
			floor_names[2] = "Floor";

			std::vector<Object*> floors;
			obj->split(DIRECTION_Y, floor_ratios, floor_names, floors);

			stack.insert(stack.end(), floors.begin(), floors.end());
		} else if (obj->_name == "SideFacade") {
			obj->setupProjection(2.5f, 1.0f);
			obj->setTexture("textures/brick.jpg");

			// 分割比率、名前をセット
			std::vector<float> floor_ratios(3);
			floor_ratios[0] = 4.0f / obj->_scope.y;
			floor_ratios[1] = 3.5f / obj->_scope.y;
			floor_ratios[2] = 3.5f / obj->_scope.y;
			std::vector<std::string> floor_names(3);
			floor_names[0] = "Floor";
			floor_names[1] = "Floor";
			floor_names[2] = "Floor";

			std::vector<Object*> floors;
			obj->split(DIRECTION_Y, floor_ratios, floor_names, floors);

			stack.insert(stack.end(), floors.begin(), floors.end());
		} else if (obj->_name == "Floor") {
			std::vector<float> tile_ratios;
			std::vector<std::string> tile_names;

			// 分割比率、名前をセット
			float margin_ratio = 1.0f / obj->_scope.x;
			int numTiles = (obj->_scope.x - 2.0f) / 3.0f;
			float tile_width_ratio = (obj->_scope.x - 2.0f) / numTiles / obj->_scope.x;

			tile_ratios.push_back(margin_ratio);
			tile_names.push_back("Wall");
			for (int i = 0; i < numTiles; ++i) {
				tile_ratios.push_back(tile_width_ratio);
				tile_names.push_back("Tile");
			}
			tile_ratios.push_back(margin_ratio);
			tile_names.push_back("Wall");

			std::vector<Object*> tiles;
			obj->split(DIRECTION_X, tile_ratios, tile_names, tiles);

			stack.insert(stack.end(), tiles.begin(), tiles.end());
		} else if (obj->_name == "Tile") {
			std::vector<float> wall_ratios(3);
			wall_ratios[0] = 0.1f;
			wall_ratios[1] = 0.8f;
			wall_ratios[2] = 0.1f;
			std::vector<std::string> wall_names(3);
			wall_names[0] = "Wall";
			wall_names[1] = "WallWindowWall";
			wall_names[2] = "Wall";

			std::vector<Object*> walls;
			obj->split(DIRECTION_X, wall_ratios, wall_names, walls);

			stack.insert(stack.end(), walls.begin(), walls.end());
		} else if (obj->_name == "WallWindowWall") {
			std::vector<float> wall_ratios(3);
			wall_ratios[0] = 0.2f;
			wall_ratios[1] = 0.6f;
			wall_ratios[2] = 0.2f;
			std::vector<std::string> wall_names(3);
			wall_names[0] = "Wall";
			wall_names[1] = "Window";
			wall_names[2] = "Wall";

			std::vector<Object*> walls;
			obj->split(DIRECTION_Y, wall_ratios, wall_names, walls);

			stack.insert(stack.end(), walls.begin(), walls.end());
		} else if (obj->_name == "Window") {
			obj->translate(glm::vec3(0, 0, -0.25f));
			obj->setupProjection(obj->_scope.x, obj->_scope.y);
			obj->setTexture("textures/window.jpg");

			Object* obj2 = obj->clone();
			obj2->_name = "Window_1";
			stack.push_back(obj2);
		} else {
			obj->generate(renderManager);
			delete obj;
		}
	}
}

void CGA::generateVase1(RenderManager* renderManager) {
	std::list<Object*> stack;
	
	std::vector<glm::vec2> points;
	points.push_back(glm::vec2(0, 6));
	points.push_back(glm::vec2(38, 12));
	Line* line = new Line("Line", modelMat, points, glm::vec3(1, 1, 1));
	stack.push_back(line);
	
	while (!stack.empty()) {
		Object* obj = stack.front();
		stack.pop_front();

		if (obj->_removed) {
			delete obj;
			continue;
		}
		
		if (obj->_name == "Line") {
			Object* obj2 = obj->clone();
			obj2->_name = "RotatedLine";
			obj2->rotate(0, -90, 0);
			stack.push_back(obj2);
		} else if (obj->_name == "RotatedLine") {
			stack.push_back(obj->revolve("Vase", REVOLVE_X));
		} else {
			obj->generate(renderManager);
			delete obj;
		}
	}
}

void CGA::generateVase2(RenderManager* renderManager) {
	std::list<Object*> stack;
	
	std::vector<glm::vec2> points;
	for (int i = 0; i <= 30; ++i) {
		float y = 6.0f + 6.0f * sinf((float)i / 30 * M_PI * 0.5);
		points.push_back(glm::vec2(i, y));
	}
	points.push_back(glm::vec2(34, 8));
	points.push_back(glm::vec2(38, 12));
	Line* line = new Line("Line", modelMat, points, glm::vec3(1, 1, 1));
	stack.push_back(line);
	
	while (!stack.empty()) {
		Object* obj = stack.front();
		stack.pop_front();

		if (obj->_removed) {
			delete obj;
			continue;
		}
		
		if (obj->_name == "Line") {
			Object* obj2 = obj->clone();
			obj2->_name = "RotatedLine";
			obj2->rotate(0, -90, 0);
			stack.push_back(obj2);
		} else if (obj->_name == "RotatedLine") {
			stack.push_back(obj->revolve("Vase", REVOLVE_X));
		} else {
			obj->generate(renderManager);
			delete obj;
		}
	}
}

void CGA::generateVase3(RenderManager* renderManager) {
	std::list<Object*> stack;
	
	std::vector<glm::vec2> points;
	points.push_back(glm::vec2(0, 8));
	points.push_back(glm::vec2(0.5, 8));
	points.push_back(glm::vec2(0.6, 6));
	points.push_back(glm::vec2(2, 5));
	points.push_back(glm::vec2(2.1, 6));
	points.push_back(glm::vec2(3.0, 6));
	points.push_back(glm::vec2(4.5, 2));
	for (int i = 0; i < 5; ++i) {
		float x = 5.0f + (float)i / 5 * 3;
		float y = 2.0f + sinf((float)i / 5 * M_PI);
		points.push_back(glm::vec2(x, y));
	}

	//points.push_back(glm::vec2(5, 2));
	//points.push_back(glm::vec2(6, 4));
	for (int i = 0; i <= 22; ++i) {
		float y = 1.5f + 10.5f * sinf((float)i / 22 * M_PI * 0.5);
		points.push_back(glm::vec2(8 + i, y));
	}
	points.push_back(glm::vec2(34, 8));
	points.push_back(glm::vec2(38, 12));
	Line* line = new Line("Line", modelMat, points, glm::vec3(1, 1, 1));
	stack.push_back(line);
	
	while (!stack.empty()) {
		Object* obj = stack.front();
		stack.pop_front();

		if (obj->_removed) {
			delete obj;
			continue;
		}
		
		if (obj->_name == "Line") {
			Object* obj2 = obj->clone();
			obj2->_name = "RotatedLine";
			obj2->rotate(0, -90, 0);
			stack.push_back(obj2);
		} else if (obj->_name == "RotatedLine") {
			stack.push_back(obj->revolve("Vase", REVOLVE_X));
		} else {
			obj->generate(renderManager);
			delete obj;
		}
	}
}

void CGA::generateSaltShaker1(RenderManager* renderManager) {
	std::list<Object*> stack;
	
	Rectangle* base = new Rectangle("Base", modelMat, 10, 10, glm::vec3(1, 1, 1));
	stack.push_back(base);
	
	while (!stack.empty()) {
		Object* obj = stack.front();
		stack.pop_front();

		if (obj->_removed) {
			delete obj;
			continue;
		}
		
		if (obj->_name == "Base") {
			stack.push_back(obj->extrude("Body", 20));
		} else {
			obj->generate(renderManager);
			delete obj;
		}
	}
}

void CGA::generateSaltShaker2(RenderManager* renderManager) {
	std::list<Object*> stack;

	Rectangle* base = new Rectangle("Base", modelMat, 10, 10, glm::vec3(1, 1, 1));
	stack.push_back(base);
	
	while (!stack.empty()) {
		Object* obj = stack.front();
		stack.pop_front();

		if (obj->_removed) {
			delete obj;
			continue;
		}
		
		if (obj->_name == "Base") {
			Object* obj2 = obj->taper("Body", 20, 0.7f);
			Rectangle* front;
			std::vector<Rectangle*> sides;
			Polygon* top;
			Polygon* base;
			obj2->componentSplit("front", &front, "side", sides, "top", &top, "base", &base);
			stack.push_back(obj2);
			stack.push_back(top);
		} else if (obj->_name == "top") {
			stack.push_back(obj->offset("CapBase", 0.2f));
		} else if (obj->_name == "CapBase") {
			stack.push_back(obj->extrude("Cap", 6));
		} else {
			obj->generate(renderManager);
			delete obj;
		}
	}
}

}
