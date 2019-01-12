#include "Cuboid.h"
#include "Rectangle.h"
#include "GeometryUtil.h"

namespace procedural_generation {

	Cuboid::Cuboid(const std::string& name,  const glm::mat4& modelMat, float width, float depth, float height, const glm::vec4& color) {
		_name = name;
		_modelMat = modelMat;
		_scope = glm::vec3(width, depth, height);
		_color = color;
	}

	/*ShapePtr Cuboid::clone(const std::string& name) const {
	std::shared_ptr<Cuboid> copy = std::make_shared<Cuboid>(*this);
	copy->_name = name;
	return copy;
	}*/

	ShapePtr Cuboid::extrude(const std::string& name, float height) const {
		throw std::runtime_error("Cuboid::extrude() is not implemented.");
	}

	void Cuboid::componentSplit(const std::unordered_map<std::string, std::string>& nameMap, std::vector<std::shared_ptr<Shape>>& shapes) const {
		shapes.clear();

		// top face
		if (nameMap.find("top") != nameMap.end()) {
			const auto mat = glm::translate(_modelMat, glm::vec3(0, 0, _scope.z));
			shapes.emplace_back(std::make_shared<Rectangle>(nameMap.at("top"), mat, _scope.x, _scope.y, _color));
		}

		// bottom face
		if (nameMap.find("bottom") != nameMap.end() && _scope.z >= 0) {
			shapes.emplace_back(std::make_shared<Rectangle>(nameMap.at("bottom"), glm::translate(glm::rotate(_modelMat, glm::pi<float>(), glm::vec3(1, 0, 0)), glm::vec3(0, -_scope.y, 0)), _scope.x, _scope.y, _color));
		}

		// front face
		if (nameMap.find("front") != nameMap.end()) {
			float rot_angle = glm::pi<float>() * 0.5f;
			if (_scope.z < 0) {
				rot_angle = -rot_angle;
			}
			shapes.emplace_back(std::make_shared<Rectangle>(nameMap.at("front"), glm::rotate(_modelMat, rot_angle, glm::vec3(1, 0, 0)), _scope.x, fabs(_scope.z), _color));
		}

		// right face
		if (nameMap.find("right") != nameMap.end()) {
			float rot_angle = glm::pi<float>() * 0.5f;
			if (_scope.z < 0) {
				rot_angle = -rot_angle;
			}
			const auto mat = glm::rotate(glm::translate(_modelMat, glm::vec3(_scope.x, 0, 0)), glm::pi<float>() * 0.5f, glm::vec3(0, 0, 1));
			shapes.emplace_back(std::make_shared<Rectangle>(nameMap.at("right"), glm::rotate(mat, rot_angle, glm::vec3(1, 0, 0)), _scope.y, fabs(_scope.z), _color));
		}

		// left face
		if (nameMap.find("left") != nameMap.end()) {
			float rot_angle = glm::pi<float>() * 0.5f;
			if (_scope.z < 0) {
				rot_angle = -rot_angle;
			}
			const auto mat = glm::translate(glm::rotate(_modelMat, -glm::pi<float>() * 0.5f, glm::vec3(0, 0, 1)), glm::vec3(-_scope.y, 0, 0));
			shapes.emplace_back(std::make_shared<Rectangle>(nameMap.at("left"), glm::rotate(mat, rot_angle, glm::vec3(1, 0, 0)), _scope.y, fabs(_scope.z), _color));
		}

		// back face
		if (nameMap.find("back") != nameMap.end()) {
			float rot_angle = glm::pi<float>() * 0.5f;
			if (_scope.z < 0) {
				rot_angle = -rot_angle;
			}
			const auto mat = glm::translate(glm::rotate(glm::translate(_modelMat, glm::vec3(_scope.x, 0, 0)), glm::pi<float>(), glm::vec3(0, 0, 1)), glm::vec3(0, -_scope.y, 0));
			shapes.emplace_back(std::make_shared<Rectangle>(nameMap.at("back"), glm::rotate(mat, rot_angle, glm::vec3(1, 0, 0)), _scope.x, fabs(_scope.z), _color));
		}

		// side faces
		if (nameMap.find("side") != nameMap.end()) {
			float rot_angle = glm::pi<float>() * 0.5f;
			if (_scope.z < 0) {
				rot_angle = -rot_angle;
			}

			// front face
			if (nameMap.find("front") == nameMap.end()) {
				shapes.emplace_back(std::make_shared<Rectangle>(nameMap.at("side"), glm::rotate(_modelMat, rot_angle, glm::vec3(1, 0, 0)), _scope.x, fabs(_scope.z), _color));
			}

			// right face
			if (nameMap.find("right") == nameMap.end()) {
				const auto mat = glm::rotate(glm::translate(_modelMat, glm::vec3(_scope.x, 0, 0)), glm::pi<float>() * 0.5f, glm::vec3(0, 0, 1));
				shapes.emplace_back(std::make_shared<Rectangle>(nameMap.at("side"), glm::rotate(mat, rot_angle, glm::vec3(1, 0, 0)), _scope.y, fabs(_scope.z), _color));
			}

			// left face
			if (nameMap.find("left") == nameMap.end()) {
				const auto mat = glm::translate(glm::rotate(_modelMat, -glm::pi<float>() * 0.5f, glm::vec3(0, 0, 1)), glm::vec3(-_scope.y, 0, 0));
				shapes.emplace_back(std::make_shared<Rectangle>(nameMap.at("side"), glm::rotate(mat, rot_angle, glm::vec3(1, 0, 0)), _scope.y, fabs(_scope.z), _color));
			}

			// back face
			if (nameMap.find("back") == nameMap.end()) {
				const auto mat = glm::translate(glm::rotate(glm::translate(_modelMat, glm::vec3(_scope.x, 0, 0)), glm::pi<float>(), glm::vec3(0, 0, 1)), glm::vec3(0, -_scope.y, 0));
				shapes.emplace_back(std::make_shared<Rectangle>(nameMap.at("side"), glm::rotate(mat, rot_angle, glm::vec3(1, 0, 0)), _scope.x, fabs(_scope.z), _color));
			}
		}
	}

	void Cuboid::split(SplitDirection splitAxis, const StringValuePairVec& sizeAndNames, std::vector<std::shared_ptr<Shape> >& shapes) const {
		throw std::runtime_error("Cuboid::split is not implemented.");
	}

	void Cuboid::offset(const std::string& name, float offsetDistance, const std::string& inside, const std::string& border, std::vector<ShapePtr>& shapes) const {
		throw std::runtime_error("Cuboid::offset is not implemented.");
	}

	void Cuboid::size(float xSize, float ySize, float zSize, bool centered) {
		if (centered) {
			_modelMat = glm::translate(_modelMat, glm::vec3((_scope.x - xSize) * 0.5, (_scope.y - ySize) * 0.5, (_scope.z - zSize) * 0.5));
		}

		_scope.x = xSize;
		_scope.y = ySize;
		_scope.z = zSize;
	}

	void Cuboid::setupProjection(AxisDirection axesSelector, float texWidth, float texHeight) {
		throw std::runtime_error("Cuboid::setupProjection is not implemented.");
	}

	void Cuboid::generateGeometry(Faces& faces) const {
		if (_scope.z == 0) return;
		// top
		if (_scope.x >= 0) {
			const auto mat = glm::translate(_modelMat, glm::vec3(_scope.x * 0.5, _scope.y * 0.5, _scope.z));
			GenerateRectangle(_scope.x, _scope.y, _color, mat, faces);
		}
		else {
			const auto mat = glm::rotate(glm::translate(_modelMat, glm::vec3(_scope.x * 0.5, _scope.y * 0.5, _scope.z)), glm::pi<float>(), glm::vec3(1, 0, 0));
			GenerateRectangle(_scope.x, _scope.y, _color, mat, faces);
		}

		// base
		if (_scope.z >= 0) {
			const auto mat = glm::translate(glm::rotate(_modelMat, glm::pi<float>(), glm::vec3(1, 0, 0)), glm::vec3(_scope.x * 0.5, -_scope.y * 0.5, 0));
			GenerateRectangle(_scope.x, _scope.y, _color, mat, faces);
		}

		// front
	{
		float rot_angle = glm::pi<float>() * 0.5f;
		if (_scope.z < 0) {
			rot_angle = -rot_angle;
		}
		const auto mat = glm::rotate(glm::translate(_modelMat, glm::vec3(_scope.x * 0.5, 0, _scope.z * 0.5)), rot_angle, glm::vec3(1, 0, 0));
		GenerateRectangle(_scope.x, _scope.z, _color, mat, faces);
	}

	// back
	{
		float rot_angle = glm::pi<float>() * 0.5f;
		if (_scope.z < 0) {
			rot_angle = -rot_angle;
		}
		const auto mat = glm::rotate(glm::translate(glm::rotate(glm::translate(_modelMat, glm::vec3(_scope.x * 0.5, 0, _scope.z * 0.5)), glm::pi<float>(), glm::vec3(0, 0, 1)), glm::vec3(0, -_scope.y, 0)), rot_angle, glm::vec3(1, 0, 0));
		GenerateRectangle(_scope.x, _scope.z, _color, mat, faces);
	}

	// right
	{
		float rot_angle = glm::pi<float>() * 0.5f;
		if (_scope.z < 0) {
			rot_angle = -rot_angle;
		}
		const auto mat = glm::rotate(glm::rotate(glm::translate(_modelMat, glm::vec3(_scope.x, _scope.y * 0.5, _scope.z * 0.5)), glm::pi<float>() * 0.5f, glm::vec3(0, 0, 1)), rot_angle, glm::vec3(1, 0, 0));
		GenerateRectangle(_scope.y, _scope.z, _color, mat, faces);
	}

	// left
	{
		float rot_angle = glm::pi<float>() * 0.5f;
		if (_scope.z < 0) {
			rot_angle = -rot_angle;
		}
		const auto mat = glm::rotate(glm::translate(glm::rotate(_modelMat, -glm::pi<float>() * 0.5f, glm::vec3(0, 0, 1)), glm::vec3(-_scope.y * 0.5, 0, _scope.z * 0.5)), rot_angle, glm::vec3(1, 0, 0));
		GenerateRectangle(_scope.y, _scope.z, _color, mat, faces);
	}
	}

}
