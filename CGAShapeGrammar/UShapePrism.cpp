#include "UShapePrism.h"
#include "GLUtils.h"
#include "Circle.h"
#include "Pyramid.h"
#include "HipRoof.h"
#include "GableRoof.h"
#include "UShapePrism.h"
#include "Polygon.h"
#include "Cuboid.h"
#include "SemiCircle.h"
#include "OffsetRectangle.h"
#include "CGA.h"

namespace cga {

UShapePrism::UShapePrism(const std::string& name, const glm::mat4& pivot, const glm::mat4& modelMat, float width, float depth, float height, float front_width, float back_depth, const glm::vec3& color) {
	this->_name = name;
	this->_removed = false;
	this->_pivot = pivot;
	this->_modelMat = modelMat;
	this->_scope = glm::vec3(width, depth, height);
	this->_front_width = front_width;
	this->_back_depth = back_depth;
	this->_color = color;
	this->_textureEnabled = false;
}

boost::shared_ptr<Shape> UShapePrism::clone(const std::string& name) const {
	boost::shared_ptr<Shape> copy = boost::shared_ptr<Shape>(new UShapePrism(*this));
	copy->_name = name;
	return copy;
}

void UShapePrism::generateGeometry(RenderManager* renderManager, float opacity) const {
	if (_removed) return;

	int num = 0;

	std::vector<Vertex> vertices;

	// top
	{
		glm::mat4 mat = glm::translate(_pivot * _modelMat, glm::vec3(0, 0, _scope.z));
		std::vector<glm::vec2> pts(8);
		pts[0] = glm::vec2(0, 0);
		pts[1] = glm::vec2(_front_width, 0);
		pts[2] = glm::vec2(_front_width, _scope.y - _back_depth);
		pts[3] = glm::vec2(_scope.x - _front_width, _scope.y - _back_depth);
		pts[4] = glm::vec2(_scope.x - _front_width, 0);
		pts[5] = glm::vec2(_scope.x, 0);
		pts[6] = glm::vec2(_scope.x, _scope.y);
		pts[7] = glm::vec2(0, _scope.y);

		glutils::drawConcavePolygon(pts, glm::vec4(_color, opacity), mat, vertices);
	}

	// base
	if (_scope.z >= 0) {
		glm::mat4 mat;
		std::vector<glm::vec2> pts(8);

		if (_scope.z > 0) {
			mat = glm::translate(glm::rotate(_pivot * _modelMat, M_PI, glm::vec3(1, 0, 0)), glm::vec3(0, -_scope.y, 0));
			pts[0] = glm::vec2(0, 0);
			pts[1] = glm::vec2(_scope.x, 0);
			pts[2] = glm::vec2(_scope.x, _scope.y);
			pts[3] = glm::vec2(_scope.x - _front_width, _scope.y);
			pts[4] = glm::vec2(_scope.x - _front_width, _back_depth);
			pts[5] = glm::vec2(_front_width, _back_depth);
			pts[6] = glm::vec2(_front_width, _scope.y);
			pts[7] = glm::vec2(0, _scope.y);
		}
		else {
			mat = _pivot * _modelMat;
			pts[0] = glm::vec2(0, 0);
			pts[1] = glm::vec2(_front_width, 0);
			pts[2] = glm::vec2(_front_width, _scope.y - _back_depth);
			pts[3] = glm::vec2(_scope.x - _front_width, _scope.y - _back_depth);
			pts[4] = glm::vec2(_scope.x - _front_width, 0);
			pts[5] = glm::vec2(_scope.x, 0);
			pts[6] = glm::vec2(_scope.x, _scope.y);
			pts[7] = glm::vec2(0, _scope.y);
		}

		glutils::drawConcavePolygon(pts, glm::vec4(_color, opacity), mat, vertices);
	}

	// front
	{
		float rot_angle = M_PI * 0.5f;
		if (_scope.z < 0) {
			rot_angle = -rot_angle;
		}
		glm::mat4 mat = _pivot * glm::rotate(glm::translate(_modelMat, glm::vec3(_front_width * 0.5, 0, _scope.z * 0.5)), rot_angle, glm::vec3(1, 0, 0));
		glutils::drawQuad(_front_width, _scope.z, glm::vec4(_color, opacity), mat, vertices);

		mat = _pivot * glm::rotate(glm::translate(_modelMat, glm::vec3(_scope.x * 0.5, _scope.y - _back_depth, _scope.z * 0.5)), rot_angle, glm::vec3(1, 0, 0));
		glutils::drawQuad(_scope.x - _front_width * 2, _scope.z, glm::vec4(_color, opacity), mat, vertices);

		mat = _pivot * glm::rotate(glm::translate(_modelMat, glm::vec3(_scope.x - _front_width * 0.5, 0, _scope.z * 0.5)), rot_angle, glm::vec3(1, 0, 0));
		glutils::drawQuad(_front_width, _scope.z, glm::vec4(_color, opacity), mat, vertices);

		mat = _pivot * glm::rotate(glm::rotate(glm::translate(_modelMat, glm::vec3(_front_width, (_scope.y - _back_depth) * 0.5, _scope.z * 0.5)), M_PI * 0.5f, glm::vec3(0, 0, 1)), rot_angle, glm::vec3(1, 0, 0));
		glutils::drawQuad(_scope.y - _back_depth, _scope.z, glm::vec4(_color, opacity), mat, vertices);

		mat = _pivot * glm::rotate(glm::rotate(glm::translate(_modelMat, glm::vec3(_scope.x - _front_width, (_scope.y - _back_depth) * 0.5, _scope.z * 0.5)), -M_PI * 0.5f, glm::vec3(0, 0, 1)), rot_angle, glm::vec3(1, 0, 0));
		glutils::drawQuad(_scope.y - _back_depth, _scope.z, glm::vec4(_color, opacity), mat, vertices);
	}

	// back
	{
		float rot_angle = M_PI * 0.5f;
		if (_scope.z < 0) {
			rot_angle = -rot_angle;
		}
		glm::mat4 mat = _pivot * glm::rotate(glm::translate(glm::rotate(glm::translate(_modelMat, glm::vec3(_scope.x * 0.5, 0, _scope.z * 0.5)), M_PI, glm::vec3(0, 0, 1)), glm::vec3(0, -_scope.y, 0)), rot_angle, glm::vec3(1, 0, 0));
		glutils::drawQuad(_scope.x, _scope.z, glm::vec4(_color, opacity), mat, vertices);
	}

	// right
	{
		float rot_angle = M_PI * 0.5f;
		if (_scope.z < 0) {
			rot_angle = -rot_angle;
		}
		glm::mat4 mat = _pivot * glm::rotate(glm::rotate(glm::translate(_modelMat, glm::vec3(_scope.x, _scope.y * 0.5, _scope.z * 0.5)), M_PI * 0.5f, glm::vec3(0, 0, 1)), rot_angle, glm::vec3(1, 0, 0));
		glutils::drawQuad(_scope.y, _scope.z, glm::vec4(_color, opacity), mat, vertices);
	}

	// left
	{
		float rot_angle = M_PI * 0.5f;
		if (_scope.z < 0) {
			rot_angle = -rot_angle;
		}
		glm::mat4 mat = _pivot * glm::rotate(glm::translate(glm::rotate(_modelMat, -M_PI * 0.5f, glm::vec3(0, 0, 1)), glm::vec3(-_scope.y * 0.5, 0, _scope.z * 0.5)), rot_angle, glm::vec3(1, 0, 0));
		glutils::drawQuad(_scope.y, _scope.z, glm::vec4(_color, opacity), mat, vertices);
	}

	renderManager->addObject(_name.c_str(), "", vertices);
}

}