#include "GeneralObject.h"

namespace cga {

GeneralObject::GeneralObject(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec3>& points, const std::vector<glm::vec3>& normals, const glm::vec3& color) {
	this->_name = name;
	this->_removed = false;
	this->_modelMat = modelMat;
	this->_points = points;
	this->_normals = normals;
	this->_color = color;
}

Shape* GeneralObject::clone(const std::string& name) {
	Shape* copy = new GeneralObject(*this);
	copy->_name = name;
	return copy;
}

void GeneralObject::generate(RenderManager* renderManager, bool showAxes) {
	if (_removed) return;

	std::vector<Vertex> vertices(_points.size());
	for (int i = 0; i < _points.size(); ++i) {
		vertices[i].position = glm::vec3(_modelMat * glm::vec4(_points[i], 1));
		vertices[i].normal = glm::vec3(_modelMat * glm::vec4(_normals[i], 0));
		vertices[i].color = _color;
		//vertices[i].texCoord = _texCoords[i];
	}

	renderManager->addObject(_name.c_str(), _texture.c_str(), vertices);

	if (showAxes) {
		drawAxes(renderManager, _modelMat);
	}
}

}
