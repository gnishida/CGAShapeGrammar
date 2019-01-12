#pragma once

#include "Vertex.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <exception>
#include <glm/glm.hpp>

namespace procedural_generation {

	enum class SplitDirection {
		X = 0,
		Y,
		Z
	};

	enum class AxisDirection {
		SCOPE_XY = 0,
		SCOPE_XZ,
		SCOPE_YX,
		SCOPE_YZ,
		SCOPE_ZX,
		SCOPE_ZY,
		WORLD_XY,
		WORLD_XZ,
		WORLD_YX,
		WORLD_YZ,
		WORLD_ZX,
		WORLD_ZY
	};

	enum class TranslateMode {
		ABSOL = 0,
		RELAT
	};

	enum class CoordinateSystem {
		WORLD = 0,
		OBJECT
	};

	class Value {
	public:
		enum {
			TYPE_ABSOLUTE = 0,
			TYPE_RELATIVE,
			TYPE_FLOATING
		};

	public:
		int type;
		float value;
		bool repeat;

	public:
		Value() : type(TYPE_ABSOLUTE), value(0), repeat(false) {}
		Value(int type, float value, bool repeat = false) : type(type), value(value), repeat(repeat) {}
	};

	using StringValuePair = std::pair<std::string, Value>;
	using StringValuePairVec = std::vector<StringValuePair>;

	void DecodeSplitSizes(float size, const StringValuePairVec& nameAndSizes, std::vector<std::pair<std::string, float>>& decodedNameAndSizes);

	class Shape;
	using ShapePtr = std::shared_ptr<Shape>;

	/**
	* Abstract class to represent any shape in the CGA Shape Grammar.
	*/
	class Shape {
	public:
		Shape() = default;
		virtual ~Shape() {}

		std::string name() { return _name; }
		//virtual ShapePtr clone(const std::string& name) const = 0;
		virtual ShapePtr extrude(const std::string& name, float height) const = 0;
		virtual void componentSplit(const std::unordered_map<std::string, std::string>& nameMap, std::vector<std::shared_ptr<Shape>>& shapes) const = 0;
		virtual void split(SplitDirection splitAxis, const StringValuePairVec& sizeAndNames, std::vector<std::shared_ptr<Shape>>& shapes) const = 0;
		virtual void offset(const std::string& name, float offsetDistance, const std::string& inside, const std::string& border, std::vector<std::shared_ptr<Shape>>& shapes) const = 0;
		virtual void size(float xSize, float ySize, float zSize, bool centered) = 0;
		void rotate(float xAngle, float yAngle, float zAngle);
		void translate(TranslateMode translateMode, CoordinateSystem coordSystem, float x, float y, float z);
		virtual void setupProjection(AxisDirection axesSelector, float texWidth, float texHeight) = 0;
		virtual void generateGeometry(Faces& faces) const = 0;

		friend class Operator;
		friend class ColorOperator;
		friend class TextureOperator;
		friend class SetupProjectionOperator;
		friend class RotateOperator;
		friend class TranslateOperator;
		friend class SizeOperator;

	protected:
		std::string _name;
		glm::mat4 _modelMat;
		glm::vec4 _color;
		std::string _texturePath;
		std::vector<glm::vec2> _texCoords;
		glm::vec3 _scope;

	};


}
