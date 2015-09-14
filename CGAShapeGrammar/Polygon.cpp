#include "Polygon.h"
#include "Pyramid.h"
#include "GableRoof.h"
#include "HipRoof.h"
#include "OffsetPolygon.h"
#include "Prism.h"
#include "GeneralObject.h"
#include "GLUtils.h"

namespace cga {

Polygon::Polygon(const std::string& name, const glm::mat4& pivot, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, const glm::vec3& color, const std::string& texture) {
	this->_name = name;
	this->_removed = false;
	this->_pivot = pivot;
	this->_modelMat = modelMat;
	this->_points = points;
	this->_color = color;
	this->_texture = texture;

	BoundingBox bbox(points);
	this->_scope = glm::vec3(bbox.maxPt.x, bbox.maxPt.y, 0);

	this->_center = glm::vec2(0, 0);
	for (int i = 0; i < points.size(); ++i) {
		_center += points[i];
	}
	_center /= points.size();
}

Shape* Polygon::clone(const std::string& name) {
	Shape* copy = new Polygon(*this);
	copy->_name = name;
	return copy;
}

Shape* Polygon::extrude(const std::string& name, float height) {
	return new Prism(name, _pivot, _modelMat, _points, height, _color);
}

Shape* Polygon::inscribeCircle(const std::string& name) {
	return NULL;
}

Shape* Polygon::offset(const std::string& name, float offsetDistance, int offsetSelector) {
	if (offsetSelector == SELECTOR_ALL) {
		return new OffsetPolygon(name, _pivot, _modelMat, _points, offsetDistance, _color, _texture);
	} else if (offsetSelector == SELECTOR_INSIDE) {
		std::vector<glm::vec2> offset_points;
		glutils::offsetPolygon(_points, offsetDistance, offset_points);
		return new Polygon(name, _pivot, _modelMat, offset_points, _color, _texture);
	} else {
		std::vector<glm::vec2> offset_points;
		glutils::offsetPolygon(_points, offsetDistance, offset_points);

		std::vector<glm::vec3> pts;
		std::vector<glm::vec3> normals;
		for (int i = 0; i < _points.size(); ++i) {
			pts.push_back(glm::vec3(offset_points[i], 0));
			pts.push_back(glm::vec3(_points[i], 0));
			pts.push_back(glm::vec3(_points[(i+1) % _points.size()], 0));

			pts.push_back(glm::vec3(offset_points[i], 0));
			pts.push_back(glm::vec3(_points[(i+1) % _points.size()], 0));
			pts.push_back(glm::vec3(offset_points[(i+1) % offset_points.size()], 0));

			normals.push_back(glm::vec3(0, 0, 1));
			normals.push_back(glm::vec3(0, 0, 1));
			normals.push_back(glm::vec3(0, 0, 1));
			normals.push_back(glm::vec3(0, 0, 1));
			normals.push_back(glm::vec3(0, 0, 1));
			normals.push_back(glm::vec3(0, 0, 1));
		}
		
		return new GeneralObject(name, _pivot, _modelMat, pts, normals, _color);
	}
}

Shape* Polygon::roofHip(const std::string& name, float angle) {
	return new HipRoof(name, _pivot, _modelMat, _points, angle, _color);
}

Shape* Polygon::roofGable(const std::string& name, float angle) {
	return new GableRoof(name, _pivot, _modelMat, _points, angle, _color);
}

void Polygon::setupProjection(float texWidth, float texHeight) {
	_texCoords.resize(_points.size());
	for (int i = 0; i < _points.size(); ++i) {
		_texCoords[i] = glm::vec2(_points[i].x / texWidth, _points[i].y / texHeight);
	}
}

void Polygon::size(float xSize, float ySize, float zSize) {
	_prev_scope = _scope;

	float scaleX = xSize / _scope.x;
	float scaleY = ySize / _scope.y;
	for (int i = 0; i < _points.size(); ++i) {
		_points[i].x *= scaleX;
		_points[i].y *= scaleY;
	}
	_scope.x = xSize;
	_scope.y = ySize;
	_scope.z = 0.0f;
}

Shape* Polygon::taper(const std::string& name, float height, float top_ratio) {
	return new Pyramid(name, _pivot, _modelMat, _points, _center, height, top_ratio, _color, _texture);
}

void Polygon::generate(RenderManager* renderManager, bool showScopeCoordinateSystem) {
	if (_removed) return;

	std::vector<Vertex> vertices;
	glutils::drawConcavePolygon(_points, _color, _pivot * _modelMat, vertices);

	renderManager->addObject(_name.c_str(), _texture.c_str(), vertices);

	if (showScopeCoordinateSystem) {
		drawAxes(renderManager, _pivot * _modelMat);
	}
}

}
