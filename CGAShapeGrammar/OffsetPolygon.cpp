#include "OffsetPolygon.h"
#include "GLUtils.h"

namespace cga {

OffsetPolygon::OffsetPolygon(const std::string& name, const glm::mat4& pivot, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, float offsetDistance, const glm::vec3& color, const std::string& texture) {
	this->_name = name;
	this->_removed = false;
	this->_pivot = pivot;
	this->_modelMat = modelMat;
	this->_points = points;
	this->_offsetDistance = offsetDistance;
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

Shape* OffsetPolygon::clone(const std::string& name) {
	Shape* copy = new OffsetPolygon(*this);
	copy->_name = name;
	return copy;
}

void OffsetPolygon::comp(const std::string& front_name, Shape** front, const std::string& sides_name, std::vector<Shape*>& sides, const std::string& top_name, Shape** top, const std::string& bottom_name, Shape** bottom) {

}

void OffsetPolygon::generate(RenderManager* renderManager, bool showScopeCoordinateSystem) {
	if (_removed) return;

	std::vector<glm::vec2> offset_points;
	glutils::offsetPolygon(_points, _offsetDistance, offset_points);

	std::vector<Vertex> vertices;
	glutils::drawConcavePolygon(offset_points, _color, _pivot * _modelMat, vertices);

	for (int i = 0; i < _points.size(); ++i) {
		std::vector<glm::vec2> pts(4);
		pts[0] = offset_points[i];
		pts[1] = _points[i];
		pts[2] = _points[(i+1) % _points.size()];
		pts[3] = offset_points[(i+1) % offset_points.size()];
		glutils::drawPolygon(pts, _color, _pivot * _modelMat, vertices);
	}

	/*
	glm::vec2 t = glm::vec2(_points[0] * (1.0f - offsetRatio) + _center * offsetRatio) - _points[0];

	std::vector<glm::vec2> points(_points.size());
	for (int i = 0; i < _points.size(); ++i) {
		points[i] = glm::vec2(_points[i] * (1.0f - offsetRatio) + _center * offsetRatio) - t;
	}


	glutils::drawConcavePolygon(_points, _color, _pivot * _modelMat, vertices);
	*/

	renderManager->addObject(_name.c_str(), _texture.c_str(), vertices);

	if (showScopeCoordinateSystem) {
		drawAxes(renderManager, _pivot * _modelMat);
	}
}

}
