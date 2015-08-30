#include "Circle.h"

namespace cga {

Circle::Circle(const std::string& name, const glm::mat4& modelMat, const glm::vec2& center, float radius, const glm::vec3& color) {
	this->_name = name;
	this->_removed = false;
	this->_modelMat = modelMat;
	this->_center = center;
	this->_radius = _radius;
	this->_color = color;
}

Shape* Circle::clone(const std::string& name) {
	Shape* copy = new Circle(*this);
	copy->_name = name;
	return copy;
}

}
