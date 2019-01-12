#pragma once

#include "Shape.h"

namespace procedural_generation {

	class Cuboid : public Shape {
	public:
		Cuboid(const std::string& name, const glm::mat4& modelMat, float width, float depth, float height, const glm::vec4& color);
		~Cuboid() {}

		//virtual ShapePtr clone(const std::string& name) const override;
		virtual ShapePtr extrude(const std::string& name, float height) const override;
		virtual void componentSplit(const std::unordered_map<std::string, std::string>& nameMap, std::vector<std::shared_ptr<Shape>>& shapes) const override;
		virtual void split(SplitDirection splitAxis, const StringValuePairVec& sizeAndNames, std::vector<std::shared_ptr<Shape> >& shapes) const override;
		virtual void offset(const std::string& name, float offsetDistance, const std::string& inside, const std::string& border, std::vector<ShapePtr>& shapes) const override;
		virtual void size(float xSize, float ySize, float zSize, bool centered) override;
		virtual void setupProjection(AxisDirection axesSelector, float texWidth, float texHeight) override;
		virtual void generateGeometry(Faces& faces) const override;
	};

}

