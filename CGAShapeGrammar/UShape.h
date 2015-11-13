#pragma once

#include <boost/shared_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include "Shape.h"

namespace cga {

class UShape : public Shape {
private:
	float _front_width;
	float _back_height;

public:
	UShape() {}
	UShape(const std::string& name, const glm::mat4& pivot, const glm::mat4& modelMat, float width, float height, float front_width, float back_height, const glm::vec3& color);
	boost::shared_ptr<Shape> clone(const std::string& name) const;
	boost::shared_ptr<Shape> extrude(const std::string& name, float height);
	void generateGeometry(RenderManager* renderManager, float opacity) const;
};

}
