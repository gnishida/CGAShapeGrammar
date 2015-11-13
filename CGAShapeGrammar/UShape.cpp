#include "UShape.h"
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

UShape::UShape(const std::string& name, const glm::mat4& pivot, const glm::mat4& modelMat, float width, float height, float front_width, float back_height, const glm::vec3& color) {
	this->_name = name;
	this->_removed = false;
	this->_pivot = pivot;
	this->_modelMat = modelMat;
	this->_scope = glm::vec3(width, height, 0);
	this->_front_width = front_width;
	this->_back_height = back_height;
	this->_color = color;
	this->_textureEnabled = false;
}

boost::shared_ptr<Shape> UShape::clone(const std::string& name) const {
	boost::shared_ptr<Shape> copy = boost::shared_ptr<Shape>(new UShape(*this));
	copy->_name = name;
	return copy;
}

boost::shared_ptr<Shape> UShape::extrude(const std::string& name, float height) {
	return boost::shared_ptr<Shape>(new UShapePrism(name, _pivot, _modelMat, _scope.x, _scope.y, height, _front_width, _back_height, _color));
}

void UShape::generateGeometry(RenderManager* renderManager, float opacity) const {
	if (_removed) return;

	std::vector<glm::vec2> pts(8);
	pts[0] = glm::vec2(0, 0);
	pts[1] = glm::vec2(_front_width, 0);
	pts[2] = glm::vec2(_front_width, _scope.y - _back_height);
	pts[3] = glm::vec2(_scope.x - _front_width, _scope.y - _back_height);
	pts[4] = glm::vec2(_scope.x - _front_width, 0);
	pts[5] = glm::vec2(_scope.x, 0);
	pts[6] = glm::vec2(_scope.x, _scope.y);
	pts[7] = glm::vec2(0, _scope.y);

	std::vector<Vertex> vertices;
	glutils::drawConcavePolygon(pts, glm::vec4(_color, opacity), _pivot * _modelMat, vertices);

	renderManager->addObject(_name.c_str(), "", vertices);
}

}