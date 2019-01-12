#include "Polygon.h"
#include "Prism.h"
#include "BoundingBox.h"
#include "GeometryUtil.h"
#include <algorithm>

namespace procedural_generation {

	Polygon::Polygon(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, const glm::vec4& color) {
		this->_name = name;
		this->_modelMat = modelMat;
		this->_color = color;

		// remove redundant points
		_points = points;
		RemoveRedundantPoints(_points);

		// orient the polygon in CCW order
		OrientPolygon(_points);

		BoundingBox bbox{ _points };

		// offset the polygon to make the center of the bounding box be the origin of the local coordinate system
		for (auto& p : _points) {
			p.x -= bbox.centerX();
			p.y -= bbox.centerY();
		}
		_modelMat = glm::translate(_modelMat, glm::vec3(bbox.centerX(), bbox.centerY(), 0));

		this->_scope = glm::vec3(bbox.width(), bbox.height(), 0);
	}

	/*ShapePtr Polygon::clone(const std::string& name) const {
	std::shared_ptr<Polygon> copy(new Polygon(*this));
	copy->_name = name;
	return copy;
	}*/

	ShapePtr Polygon::extrude(const std::string& name, float height) const {
		return std::make_shared<Prism>(name, _modelMat, _points, height, _color);
	}

	void Polygon::componentSplit(const std::unordered_map<std::string, std::string>& nameMap, std::vector<ShapePtr>& shapes) const {
		throw std::runtime_error("Polygon::componentSplit is not implemented.");
	}

	void Polygon::split(SplitDirection splitAxis, const StringValuePairVec& sizeAndNames, std::vector<ShapePtr>& shapes) const {
		throw std::runtime_error("Polygon::split is not implemented.");
	}

	void Polygon::offset(const std::string& name, float offsetDistance, const std::string& inside, const std::string& border, std::vector<ShapePtr>& shapes) const {
		std::vector<Polygon2D> offsetPolygons;
		OffsetPolygon(_points, offsetDistance, OffsetPolygonType::SQUARE, offsetPolygons);

		// inner shape
		if (!inside.empty()) {
			for (const auto& offsetPolygon : offsetPolygons) {
				std::vector<glm::vec2> newPoints;
				glm::vec2 diff = offsetPolygon[0] - _points[0];
				glm::mat4 mat = glm::translate(_modelMat, glm::vec3(diff, 0));
				for (size_t i = 0; i < offsetPolygon.size(); ++i) {
					newPoints.push_back(offsetPolygon[i] - diff);
				}

				shapes.emplace_back(std::make_shared<Polygon>(inside, mat, newPoints, _color));
			}
		}

		// border shape
		if (!border.empty()) {
			// Note: We triangulate the polygon with holes, so the border shape gets split into multiple shapes.
			// This might be undesired for some cases...
			std::vector<Polygon2D> triangulatedPolygons = Triangulate(_points, offsetPolygons);
			for (const auto& triangulatedPolygon : triangulatedPolygons) {
				shapes.emplace_back(std::make_shared<Polygon>(border, _modelMat, triangulatedPolygon, _color));
			}
		}
	}

	void Polygon::size(float xSize, float ySize, float zSize, bool centered) {
		if (centered) {
			// Since the center of the bounding box is already the origin of the local coordinate system,
			// there is no need to adjust the coordinates.
		}

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

	void Polygon::setupProjection(AxisDirection axesSelector, float texWidth, float texHeight) {
		if (axesSelector == AxisDirection::SCOPE_XY) {
			BoundingBox bbox{ _points };

			_texCoords.resize(_points.size());
			for (size_t i = 0; i < _points.size(); i++) {
				_texCoords[i] = glm::vec2((_points[i].x - bbox.minX()) / texWidth, (_points[i].y - bbox.minY()) / texHeight);
			}
		}
		else {
			throw std::runtime_error("Polygon supports only scope.xy for setupProjection.");
		}
	}

	void Polygon::generateGeometry(Faces& faces) const {
		if (!_texturePath.empty() && _texCoords.size() == _points.size()) {
			GeneratePolygon(_points, _color, _texCoords, _texturePath, _modelMat, faces);
		}
		else {
			GeneratePolygon(_points, _color, _modelMat, faces);
		}
	}

}
