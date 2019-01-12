#pragma once

#include "Shape.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

namespace procedural_generation {

	class Polygon : public Shape {
	public:
		Polygon(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, const glm::vec4& color);
		//virtual std::shared_ptr<Shape> clone(const std::string& name) const override;
		virtual std::shared_ptr<Shape> extrude(const std::string& name, float height) const override;
		virtual void componentSplit(const std::unordered_map<std::string, std::string>& nameMap, std::vector<ShapePtr>& shapes) const override;
		virtual void split(SplitDirection splitAxis, const StringValuePairVec& sizeAndNames, std::vector<ShapePtr>& shapes) const override;
		virtual void offset(const std::string& name, float offsetDistance, const std::string& inside, const std::string& border, std::vector<std::shared_ptr<Shape>>& shapes) const override;
		virtual void size(float xSize, float ySize, float zSize, bool centered) override;
		virtual void setupProjection(AxisDirection axesSelector, float texWidth, float texHeight) override;
		virtual void generateGeometry(Faces& faces) const override;

	private:
		std::vector<glm::vec2> _points;
	};

}
