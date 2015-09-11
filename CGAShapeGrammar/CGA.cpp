#include "CGA.h"
#include "GLUtils.h"
#include "OBJLoader.h"
#include <map>
#include <iostream>

#include "Rectangle.h"
#include "Polygon.h"
#include "GeneralObject.h"

namespace cga {

const bool showAxes = false;

BoundingBox::BoundingBox(const std::vector<glm::vec2>& points) {
	minPt.x = (std::numeric_limits<float>::max)();
	minPt.y = (std::numeric_limits<float>::max)();
	minPt.z = 0.0f;
	maxPt.x = -(std::numeric_limits<float>::max)();
	maxPt.y = -(std::numeric_limits<float>::max)();
	maxPt.z = 0.0f;

	for (int i = 0; i < points.size(); ++i) {
		minPt.x = min(minPt.x, points[i].x);
		minPt.y = min(minPt.y, points[i].y);
		maxPt.x = max(maxPt.x, points[i].x);
		maxPt.y = max(maxPt.y, points[i].y);
	}
}

BoundingBox::BoundingBox(const std::vector<glm::vec3>& points) {
	minPt.x = (std::numeric_limits<float>::max)();
	minPt.y = (std::numeric_limits<float>::max)();
	minPt.z = (std::numeric_limits<float>::max)();
	maxPt.x = -(std::numeric_limits<float>::max)();
	maxPt.y = -(std::numeric_limits<float>::max)();
	maxPt.z = -(std::numeric_limits<float>::max)();

	for (int i = 0; i < points.size(); ++i) {
		minPt.x = min(minPt.x, points[i].x);
		minPt.y = min(minPt.y, points[i].y);
		minPt.z = min(minPt.y, points[i].z);
		maxPt.x = max(maxPt.x, points[i].x);
		maxPt.y = max(maxPt.y, points[i].y);
		maxPt.z = max(maxPt.y, points[i].z);
	}
}

CGA::CGA() {
}

void CGA::generate(RenderManager* renderManager, const RuleSet& ruleSet, std::list<Shape*> stack, bool showScopeCoordinateSystem) {
	while (!stack.empty()) {
		Shape* shape = stack.front();
		stack.pop_front();

		if (ruleSet.contain(shape->_name)) {
			ruleSet.getRule(shape->_name).apply(shape, ruleSet, stack);
		} else {
			if (shape->_name.back() != '!' && shape->_name.back() != '.') {
				std::cout << "Warning: " << "no rule is found for " << shape->_name << "." << std::endl;
			}
			shape->generate(renderManager, showScopeCoordinateSystem);
			delete shape;
		}
	}
}

}
