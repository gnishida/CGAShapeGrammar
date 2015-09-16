#include "Circle.h"
#include "CGA.h"

namespace cga {

Circle::Circle(const std::string& name, const glm::mat4& modelMat, const glm::vec2& center, float radius, const glm::vec3& color) {
	this->_name = name;
	this->_removed = false;
	this->_modelMat = modelMat;
	this->_center = center;
	this->_radius = _radius;
	this->_color = color;
}

boost::shared_ptr<Shape> Circle::clone(const std::string& name) {
	boost::shared_ptr<Shape> copy = boost::shared_ptr<Shape>(new Circle(*this));
	copy->_name = name;
	return copy;
}

}
