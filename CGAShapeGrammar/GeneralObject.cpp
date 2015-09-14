#include "GeneralObject.h"

namespace cga {

GeneralObject::GeneralObject(const std::string& name, const glm::mat4& pivot, const glm::mat4& modelMat, const std::vector<glm::vec3>& points, const std::vector<glm::vec3>& normals, const glm::vec3& color) {
	this->_name = name;
	this->_removed = false;
	this->_pivot = pivot;
	this->_modelMat = modelMat;
	this->_points = points;
	this->_normals = normals;
	this->_color = color;
	this->_textureEnabled = false;
}

GeneralObject::GeneralObject(const std::string& name, const glm::mat4& pivot, const glm::mat4& modelMat, const std::vector<glm::vec3>& points, const std::vector<glm::vec3>& normals, const glm::vec3& color, const std::vector<glm::vec2>& texCoords, const std::string& texture) {
	this->_name = name;
	this->_removed = false;
	this->_pivot = pivot;
	this->_modelMat = modelMat;
	this->_points = points;
	this->_normals = normals;
	this->_color = color;
	this->_texCoords = texCoords;
	this->_texture = texture;
	this->_textureEnabled = true;
}

Shape* GeneralObject::clone(const std::string& name) {
	Shape* copy = new GeneralObject(*this);
	copy->_name = name;
	return copy;
}

void GeneralObject::size(float xSize, float ySize, float zSize) {
	_prev_scope = _scope;

	BoundingBox bbox(_points);
	float scale_x = xSize / bbox.sx();
	float scale_y = xSize / bbox.sy();
	float scale_z = xSize / bbox.sz();

	for (int i =  0; i < _points.size(); ++i) {
		_points[i].x *= scale_x;
		_points[i].y *= scale_y;
		_points[i].z *= scale_z;
	}
}

void GeneralObject::generate(RenderManager* renderManager, bool showScopeCoordinateSystem) {
	if (_removed) return;

	std::vector<Vertex> vertices(_points.size());
	for (int i = 0; i < _points.size(); ++i) {
		vertices[i].position = glm::vec3(_pivot * _modelMat * glm::vec4(_points[i], 1));
		vertices[i].normal = glm::vec3(_pivot * _modelMat * glm::vec4(_normals[i], 0));
		vertices[i].color = _color;
		if (_textureEnabled) {
			vertices[i].texCoord = _texCoords[i];
		}
	}

	renderManager->addObject(_name.c_str(), _texture.c_str(), vertices);

	if (showScopeCoordinateSystem) {
		drawAxes(renderManager, _pivot * _modelMat);
	}
}

}
