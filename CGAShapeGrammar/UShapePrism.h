#pragma once

#include <boost/shared_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include "Shape.h"

namespace cga {

class UShapePrism : public Shape {
private:
	float _front_width;
	float _back_depth;

public:
	UShapePrism() {}
	UShapePrism(const std::string& name, const glm::mat4& pivot, const glm::mat4& modelMat, float width, float depth, float height, float front_width, float back_depth, const glm::vec3& color);
	boost::shared_ptr<Shape> clone(const std::string& name) const;
	void generateGeometry(RenderManager* renderManager, float opacity) const;
};

}
