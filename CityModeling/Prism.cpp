#include "Prism.h"
#include "Polygon.h"
#include "Rectangle.h"
#include "GeometryUtil.h"
#include "BoundingBox.h"

namespace procedural_generation {

	Prism::Prism(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, float height, const glm::vec4& color) {
		this->_name = name;
		this->_modelMat = modelMat;
		this->_points = points;
		this->_color = color;

		BoundingBox bbox{ points };
		this->_scope = glm::vec3(bbox.maxX(), bbox.maxY(), height);
	}

	/*ShapePtr Prism::clone(const std::string& name) const {
	std::shared_ptr<Prism> copy = std::make_shared<Prism>(*this);
	copy->_name = name;
	return copy;
	}*/

	ShapePtr Prism::extrude(const std::string& name, float height) const {
		throw std::runtime_error("Prism::extrude is not implemented.");
	}

	void Prism::componentSplit(const std::unordered_map<std::string, std::string>& nameMap, std::vector<ShapePtr>& shapes) const {
		// top face
		if (nameMap.find("top") != nameMap.end()) {
			shapes.emplace_back(std::make_shared<Polygon>(nameMap.at("top"), glm::translate(_modelMat, glm::vec3(0, 0, _scope.z)), _points, _color));
		}

		// bottom face
		if (nameMap.find("bottom") != nameMap.end()) {
			std::vector<glm::vec2> flipped_points;
			for (const auto& p : _points) {
				flipped_points.push_back(glm::vec2(glm::rotate(glm::mat4(), glm::pi<float>(), glm::vec3(1, 0, 0)) * glm::vec4(p, 0, 1)));
			}
			shapes.emplace_back(std::make_shared<Polygon>(nameMap.at("bottom"), glm::rotate(_modelMat, glm::pi<float>(), glm::vec3(1, 0, 0)), flipped_points, _color));
		}

		// front face
		if (nameMap.find("front") != nameMap.end()) {
			shapes.emplace_back(std::make_shared<Rectangle>(nameMap.at("front"), glm::rotate(_modelMat, glm::pi<float>() * 0.5f, glm::vec3(1, 0, 0)), glm::length(_points[1] - _points[0]), _scope.z, _color));
		}

		// side faces
		if (_points.size() >= 2 && nameMap.find("side") != nameMap.end()) {
			glm::mat4 mat = glm::translate(_modelMat, glm::vec3(_points[0].x, _points[0].y, 0));

			glm::vec2 a(1, 0);
			glm::vec2 b = _points[1] - _points[0];

			float theta = acos(glm::clamp(glm::dot(a, b) / glm::length(a) / glm::length(b), -1.0f, 1.0f));
			if (a.x * b.y - a.y * b.x < 0) theta = -theta;
			mat = glm::rotate(mat, theta, glm::vec3(0, 0, 1));

			// front face
			if (nameMap.find("front") == nameMap.end()) {
				shapes.emplace_back(std::make_shared<Rectangle>(nameMap.at("side"), glm::rotate(mat, glm::pi<float>() * 0.5f, glm::vec3(1, 0, 0)), glm::length(_points[1] - _points[0]), _scope.z, _color));
			}

			for (size_t i = 1; i < _points.size(); i++) {
				glm::vec2 a = _points[i] - _points[i - 1];
				glm::vec2 b = _points[(i + 1) % _points.size()] - _points[i];

				mat = glm::translate(mat, glm::vec3(glm::length(a), 0, 0));
				float theta = acos(glm::clamp(glm::dot(a, b) / glm::length(a) / glm::length(b), -1.0f, 1.0f));
				if (a.x * b.y - a.y * b.x < 0) theta = -theta;
				mat = glm::rotate(mat, theta, glm::vec3(0, 0, 1));
				glm::mat4 mat2 = glm::rotate(mat, glm::pi<float>() * 0.5f, glm::vec3(1, 0, 0));

				shapes.emplace_back(std::make_shared<Rectangle>(nameMap.at("side"), mat2, glm::length(_points[(i + 1) % _points.size()] - _points[i]), _scope.z, _color));
			}
		}
	}

	void Prism::split(SplitDirection splitAxis, const StringValuePairVec& sizeAndNames, std::vector<ShapePtr>& shapes) const {
		throw std::runtime_error("Prims::split is not implemented.");
	}

	void Prism::offset(const std::string& name, float offsetDistance, const std::string& inside, const std::string& border, std::vector<ShapePtr>& shapes) const {
		throw std::runtime_error("Prism::offset is not implemented.");
	}

	void Prism::size(float xSize, float ySize, float zSize, bool centered) {
		throw std::runtime_error("Prism::size is not implemented.");
	}

	void Prism::setupProjection(AxisDirection axesSelector, float texWidth, float texHeight) {
		throw std::runtime_error("Prism::setupProjection is not implemented.");
	}

	void Prism::generateGeometry(Faces& faces) const {
		// top
		if (_scope.z >= 0) {
			std::vector<Vertex> vertices;
			const auto mat = glm::translate(_modelMat, glm::vec3(0, 0, _scope.z));
			GeneratePolygon(_points, _color, mat, faces);
		}

		// bottom
	{
		std::vector<glm::vec2> flipped_points;
		for (const auto& p : _points) {
			flipped_points.push_back(glm::vec2(glm::rotate(glm::mat4(), glm::pi<float>(), glm::vec3(1, 0, 0)) * glm::vec4(p, 0, 1)));
		}
		GeneratePolygon(flipped_points, _color, glm::rotate(_modelMat, glm::pi<float>(), glm::vec3(1, 0, 0)), faces);
	}

	// side
	{
		glm::vec3 p1(_modelMat * glm::vec4(_points.back(), 0, 1));
		glm::vec3 p2(_modelMat * glm::vec4(_points.back(), _scope.z, 1));

		for (size_t i = 0; i < _points.size(); ++i) {
			glm::vec3 p3(_modelMat * glm::vec4(_points[i], 0, 1));
			glm::vec3 p4(_modelMat * glm::vec4(_points[i], _scope.z, 1));
			glm::vec3 normal = glm::normalize(glm::cross(p3 - p1, p2 - p1));

			Face face;
			face.vertices.emplace_back(p1, normal, _color);
			face.vertices.emplace_back(p3, normal, _color);
			face.vertices.emplace_back(p4, normal, _color);
			face.vertices.emplace_back(p2, normal, _color);
			faces.push_back(face);

			p1 = p3;
			p2 = p4;
		}
	}
	}

}
