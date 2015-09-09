#pragma once

#include "RenderManager.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Vertex.h"
#include "Rule.h"

namespace cga {

enum { DIRECTION_X = 0, DIRECTION_Y, SCOPE_SX, SCOPE_SY };
enum { REVOLVE_X = 0, REVOLVE_Y };

const float M_PI = 3.1415926f;

class BoundingBox {
public:
	glm::vec3 minPt;
	glm::vec3 maxPt;

public:
	BoundingBox(const std::vector<glm::vec2>& points);
	BoundingBox(const std::vector<glm::vec3>& points);
	float sx() { return maxPt.x - minPt.x; }
	float sy() { return maxPt.y - minPt.y; }
	float sz() { return maxPt.z - minPt.z; }
};



class CGA {
public:
	glm::mat4 modelMat;

public:
	CGA();

	void generate(RenderManager* renderManager, std::map<std::string, Rule>& rules, std::list<Shape*> stack, bool showAxes = false);
};

}
