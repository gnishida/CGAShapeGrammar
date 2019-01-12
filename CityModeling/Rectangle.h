#pragma once

#include "Shape.h"

namespace procedural_generation {

	class Rectangle : public Shape {
	public:
		Rectangle(const std::string& name, const glm::mat4& modelMat, float width, float height, const glm::vec4& color);
		Rectangle(const std::string& name, const glm::mat4& modelMat, float width, float height, const glm::vec4& color, const std::string& texturePath, float u1, float v1, float u2, float v2);
		~Rectangle() {}

		//virtual ShapePtr clone(const std::string& name) const override;
		virtual ShapePtr extrude(const std::string& name, float height) const override;
		virtual void componentSplit(const std::unordered_map<std::string, std::string>& nameMap, std::vector<ShapePtr>& shapes) const override;
		virtual void split(SplitDirection splitAxis, const StringValuePairVec& sizeAndNames, std::vector<ShapePtr>& shapes) const override;
		virtual void offset(const std::string& name, float offsetDistance, const std::string& inside, const std::string& border, std::vector<ShapePtr>& shapes) const override;
		virtual void size(float xSize, float ySize, float zSize, bool centered) override;
		virtual void setupProjection(AxisDirection axesSelector, float texWidth, float texHeight) override;
		virtual void generateGeometry(Faces& faces) const override;
	};

}
