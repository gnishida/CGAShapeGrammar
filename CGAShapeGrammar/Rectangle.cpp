#include "Rectangle.h"
#include "GLUtils.h"
#include "Pyramid.h"
#include "HipRoof.h"
#include "Prism.h"
#include "Polygon.h"

namespace cga {

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

Object* Rectangle::clone(const std::string& name) {
	Object* copy = new Rectangle(*this);
	copy->_name = name;
	return copy;
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
	return new Prism(name, _modelMat, points, height, _color);
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
	return NULL;
}

Object* Rectangle::shapeL(const std::string& name, float frontWidth, float leftWidth) {
	std::vector<glm::vec2> points(6);
	points[0] = glm::vec2(0, 0);
	points[1] = glm::vec2(_width, 0);
	points[2] = glm::vec2(_width, frontWidth);
	points[3] = glm::vec2(leftWidth, frontWidth);
	points[4] = glm::vec2(leftWidth, _height);
	points[5] = glm::vec2(0, _height);

	return new Polygon(name, _modelMat, points, _color, _texture);
}

void Rectangle::split(int direction, const std::vector<float> sizes, const std::vector<std::string> names, std::vector<Object*>& objects) {
	objects.resize(sizes.size());

	float offset = 0.0f;
	
	for (int i = 0; i < sizes.size(); ++i) {
		glm::mat4 mat;
		if (direction == DIRECTION_X) {
			mat = glm::translate(glm::mat4(), glm::vec3(offset, 0, 0));
			if (_textureEnabled) {
				objects[i] = new Rectangle(names[i], _modelMat * mat, sizes[i], _height, _texture,
					_texCoords[0].x + (_texCoords[1].x - _texCoords[0].x) * offset / _width, _texCoords[0].y,
					_texCoords[0].x + (_texCoords[1].x - _texCoords[0].x) * (offset + sizes[i]) / _width, _texCoords[2].y);
			} else {
				objects[i] = new Rectangle(names[i], _modelMat * mat, sizes[i], _height, _color);
			}
			offset += sizes[i];
		} else {
			mat = glm::translate(glm::mat4(), glm::vec3(0, offset, 0));
			if (_textureEnabled) {
				objects[i] = new Rectangle(names[i], _modelMat * mat, _width, sizes[i], _texture,
					_texCoords[0].x, _texCoords[0].y + (_texCoords[2].y - _texCoords[0].y) * offset / _height,
					_texCoords[1].x, _texCoords[0].y + (_texCoords[2].y - _texCoords[0].y) * (offset + sizes[i]) / _height);
			} else {
				objects[i] = new Rectangle(names[i], _modelMat * mat, _width, sizes[i], _color);
			}
			offset += sizes[i];
		}
	}
}

Object* Rectangle::roofHip(const std::string& name, float angle) {
	std::vector<glm::vec2> points(4);
	points[0] = glm::vec2(0, 0);
	points[1] = glm::vec2(_width, 0);
	points[2] = glm::vec2(_width, _height);
	points[3] = glm::vec2(0, _height);
	return new HipRoof(name, _modelMat, points, angle, _color);
}

void Rectangle::generate(RenderManager* renderManager, bool showAxes) {
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
		vertices[0] = Vertex(glm::vec3(p1), glm::vec3(normal), _color, _texCoords[0]);
		vertices[1] = Vertex(glm::vec3(p2), glm::vec3(normal), _color, _texCoords[1]);
		vertices[2] = Vertex(glm::vec3(p3), glm::vec3(normal), _color, _texCoords[2]);

		vertices[3] = Vertex(glm::vec3(p1), glm::vec3(normal), _color, _texCoords[0]);
		vertices[4] = Vertex(glm::vec3(p3), glm::vec3(normal), _color, _texCoords[2]);
		vertices[5] = Vertex(glm::vec3(p4), glm::vec3(normal), _color, _texCoords[3]);

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
	
	if (showAxes) {
		vertices.resize(0);
		glutils::drawAxes(0.1, 3, _modelMat, vertices);
		renderManager->addObject("axis", "", vertices);
	}
}

}
