#include "Rectangle.h"
#include "Cuboid.h"
#include "GeometryUtil.h"

namespace procedural_generation {

	Rectangle::Rectangle(const std::string& name, const glm::mat4& modelMat, float width, float height, const glm::vec4& color) {
		_name = name;
		_modelMat = modelMat;
		_scope = glm::vec3(width, height, 0);
		_color = color;
	}

	Rectangle::Rectangle(const std::string& name, const glm::mat4& modelMat, float width, float height, const glm::vec4& color, const std::string& texturePath, float u1, float v1, float u2, float v2) {
		_name = name;
		_modelMat = modelMat;
		_scope = glm::vec3(width, height, 0);
		_color = color;
		_texturePath = texturePath;

		_texCoords.resize(4);
		_texCoords[0] = glm::vec2(u1, v1);
		_texCoords[1] = glm::vec2(u2, v1);
		_texCoords[2] = glm::vec2(u2, v2);
		_texCoords[3] = glm::vec2(u1, v2);
	}

	/*ShapePtr Rectangle::clone(const std::string& name) const {
	std::shared_ptr<Rectangle> copy = std::make_shared<Rectangle>(*this);
	copy->_name = name;
	return copy;
	}*/

	ShapePtr Rectangle::extrude(const std::string& name, float height) const {
		return std::make_shared<Cuboid>(name, _modelMat, _scope.x, _scope.y, height, _color);
	}

	void Rectangle::componentSplit(const std::unordered_map<std::string, std::string>& nameMap, std::vector<ShapePtr>& shapes) const {
		throw std::runtime_error("Rectangle::ComponentSplit is not implemented.");
	}

	void Rectangle::split(SplitDirection splitAxis, const StringValuePairVec& sizeAndNames, std::vector<ShapePtr>& shapes) const {
		std::vector<std::pair<std::string, float>> decodedNameAndSizes;
		if (splitAxis == SplitDirection::X) {
			DecodeSplitSizes(_scope.x, sizeAndNames, decodedNameAndSizes);
		}
		else {
			DecodeSplitSizes(_scope.y, sizeAndNames, decodedNameAndSizes);
		}

		shapes.clear();
		float offset = 0.0f;

		for (int i = 0; i < decodedNameAndSizes.size(); ++i) {
			if (splitAxis == SplitDirection::X) {
				if (decodedNameAndSizes[i].second > 0) {
					glm::mat4 mat = glm::translate(_modelMat, glm::vec3(offset, 0, 0));
					if (_texCoords.size() == 4) {
						shapes.emplace_back(std::make_shared<Rectangle>(decodedNameAndSizes[i].first, mat, decodedNameAndSizes[i].second, _scope.y, _color, _texturePath,
							_texCoords[0].x + (_texCoords[1].x - _texCoords[0].x) * offset / _scope.x, _texCoords[0].y,
							_texCoords[0].x + (_texCoords[1].x - _texCoords[0].x) * (offset + decodedNameAndSizes[i].second) / _scope.x, _texCoords[2].y));
					}
					else {
						shapes.emplace_back(std::make_shared<Rectangle>(decodedNameAndSizes[i].first, mat, decodedNameAndSizes[i].second, _scope.y, _color));
					}
				}
				offset += decodedNameAndSizes[i].second;
			}
			else if (splitAxis == SplitDirection::Y) {
				if (decodedNameAndSizes[i].second > 0) {
					glm::mat4 mat = glm::translate(_modelMat, glm::vec3(0, offset, 0));
					if (_texCoords.size() == 4) {
						shapes.emplace_back(std::make_shared<Rectangle>(decodedNameAndSizes[i].first, mat, _scope.x, decodedNameAndSizes[i].second, _color, _texturePath,
							_texCoords[0].x, _texCoords[0].y + (_texCoords[2].y - _texCoords[0].y) * offset / _scope.y,
							_texCoords[1].x, _texCoords[0].y + (_texCoords[2].y - _texCoords[0].y) * (offset + decodedNameAndSizes[i].second) / _scope.y));
					}
					else {
						shapes.emplace_back(std::make_shared<Rectangle>(decodedNameAndSizes[i].first, mat, _scope.x, decodedNameAndSizes[i].second, _color));
					}
				}
				offset += decodedNameAndSizes[i].second;
			}
			else if (splitAxis == SplitDirection::Z) {
				throw std::runtime_error("Rectangle::split does not support splitting in Z direction.");
			}
		}
	}

	void Rectangle::offset(const std::string& name, float offsetDistance, const std::string& inside, const std::string& border, std::vector<ShapePtr>& shapes) const {
		// inner shape
		if (!inside.empty()) {
			float offset_width = _scope.x + offsetDistance * 2.0f;
			float offset_height = _scope.y + offsetDistance * 2.0f;
			glm::mat4 mat = glm::translate(_modelMat, glm::vec3(-offsetDistance, -offsetDistance, 0));
			shapes.emplace_back(std::make_shared<Rectangle>(inside, mat, offset_width, offset_height, _color));
		}

		// border shape
		if (!border.empty() && offsetDistance < 0) {
			shapes.emplace_back(new Rectangle(border, _modelMat, _scope.x, -offsetDistance, _color));
			shapes.emplace_back(new Rectangle(border, glm::rotate(glm::translate(_modelMat, glm::vec3(_scope.x, -offsetDistance, 0)), glm::pi<float>() * 0.5f, glm::vec3(0, 0, 1)), _scope.y + offsetDistance * 2, -offsetDistance, _color));
			shapes.emplace_back(new Rectangle(border, glm::rotate(glm::translate(_modelMat, glm::vec3(_scope.x, _scope.y, 0)), glm::pi<float>(), glm::vec3(0, 0, 1)), _scope.x, -offsetDistance, _color));
			shapes.emplace_back(new Rectangle(border, glm::rotate(glm::translate(_modelMat, glm::vec3(0, _scope.y + offsetDistance, 0)), -glm::pi<float>() * 0.5f, glm::vec3(0, 0, 1)), _scope.y + offsetDistance * 2, -offsetDistance, _color));
		}
	}

	void Rectangle::size(float xSize, float ySize, float zSize, bool centered) {
		if (centered) {
			_modelMat = glm::translate(_modelMat, glm::vec3((_scope.x - xSize) * 0.5, (_scope.y - ySize) * 0.5, (_scope.z - zSize) * 0.5));
		}

		_scope.x = xSize;
		_scope.y = ySize;
		_scope.z = zSize;
	}

	void Rectangle::setupProjection(AxisDirection axesSelector, float texWidth, float texHeight) {
		if (axesSelector == AxisDirection::SCOPE_XY) {
			_texCoords.resize(4);
			_texCoords[0] = glm::vec2(0, 0);
			_texCoords[1] = glm::vec2(_scope.x / texWidth, 0);
			_texCoords[2] = glm::vec2(_scope.x / texWidth, _scope.y / texHeight);
			_texCoords[3] = glm::vec2(0, _scope.y / texHeight);
		}
		else {
			throw std::runtime_error("Rectangle supports only scope.xy for setupProjection.");
		}
	}

	void Rectangle::generateGeometry(Faces& faces) const {
		glm::mat4 mat = glm::translate(_modelMat, glm::vec3(_scope.x * 0.5, _scope.y * 0.5, 0));

		if (!_texturePath.empty() && _texCoords.size() == 4) {
			GenerateRectangle(_scope.x, _scope.y, _color, _texCoords, _texturePath, mat, faces);
		}
		else {
			GenerateRectangle(_scope.x, _scope.y, _color, mat, faces);
		}
	}

}
