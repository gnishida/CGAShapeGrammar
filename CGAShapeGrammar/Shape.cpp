#include "Shape.h"
#include "OBJLoader.h"
#include "GeneralObject.h"
#include "GLUtils.h"

namespace cga {

Shape* Shape::clone(const std::string& name) {
	throw "clone() is not supported.";
}

void Shape::comp(const std::map<std::string, std::string>& name_map, std::vector<Shape*>& shapes) {
	throw "comp() is not supported.";
}

Shape* Shape::extrude(const std::string& name, float height) {
	throw "extrude() is not supported.";
}

Shape* Shape::inscribeCircle(const std::string& name) {
	throw "inscribeCircle() is not supported.";
}

Shape* Shape::insert(const std::string& name, const std::string& geometryPath) {
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> texCoords;
	OBJLoader::load(geometryPath.c_str(), points, normals, texCoords);

	// compute scale
	float scaleX = 1.0f;
	float scaleY = 1.0f;
	float scaleZ = 1.0f;

	BoundingBox bbox(points);
	if (_scope.z == 0) {
		scaleX = _scope.x / bbox.sx();
		scaleY = _scope.y / bbox.sy();
		scaleZ = (scaleX + scaleY) * 0.5f;
	}

	// scale the points
	for (int i = 0; i < points.size(); ++i) {
		points[i].x = points[i].x * scaleX;
		points[i].y = points[i].y * scaleY;
		points[i].z = points[i].z * scaleZ;
	}

	return new GeneralObject(name, _pivot, _modelMat, points, normals, _color);
}

void Shape::nil() {
	_removed = true;
}

Shape* Shape::offset(const std::string& name, float offsetDistance) {
	throw "offset() is not supported.";
}

Shape* Shape::roofHip(const std::string& name, float angle) {
	throw "roofHip() is not supported.";
}

void Shape::rotate(const std::string& name, float xAngle, float yAngle, float zAngle) {
	_modelMat = glm::rotate(_modelMat, xAngle * M_PI / 180.0f, glm::vec3(1, 0, 0));
	_modelMat = glm::rotate(_modelMat, yAngle * M_PI / 180.0f, glm::vec3(0, 1, 0));
	_modelMat = glm::rotate(_modelMat, zAngle * M_PI / 180.0f, glm::vec3(0, 0, 1));
}

void Shape::setupProjection(float texWidth, float texHeight) {
	throw "setupProjection() is not supported.";
}

Shape* Shape::shapeL(const std::string& name, float frontWidth, float leftWidth) {
	throw "shapeL() is not supported.";
}

void Shape::size(const SingleValue& xSize, const SingleValue& ySize, const SingleValue& zSize) {
	throw "size() is not supported.";
}

void Shape::split(int splitAxis, const std::vector<float>& sizes, const std::vector<std::string>& names, std::vector<Shape*>& objects) {
	throw "split() is not supported.";
}

Shape* Shape::taper(const std::string& name, float height, float top_ratio) {
	throw "taper() is not supported.";
}

void Shape::texture(const std::string& tex) {
	this->_texture = tex;
	_textureEnabled = true;
}

void Shape::translate(int mode, int coordSystem, const glm::vec3& v) {
	if (mode == MODE_ABSOLUTE) {
		if (coordSystem == COORD_SYSTEM_WORLD) {
			_modelMat[3].x = v.x;
			_modelMat[3].y = v.y;
			_modelMat[3].z = v.z;
		} else if (coordSystem == COORD_SYSTEM_OBJECT) {
			_modelMat = glm::translate(_modelMat, v);
		}
	} else if (mode == MODE_RELATIVE) {
		if (coordSystem == COORD_SYSTEM_WORLD) {
			_modelMat[3].x += v.x;
			_modelMat[3].y += v.y;
			_modelMat[3].z += v.z;
		} else if (coordSystem == COORD_SYSTEM_OBJECT) {
			_modelMat = glm::translate(_modelMat, v);
		}
	}
}

void Shape::generate(RenderManager* renderManager, bool showScopeCoordinateSystem) {
	throw "generate() is not supported.";
}

void Shape::drawAxes(RenderManager* renderManager, const glm::mat4& modelMat) {
	std::vector<Vertex> vertices;
	glutils::drawAxes(0.1, 3, modelMat, vertices);
	renderManager->addObject("axis", "", vertices);
}

}