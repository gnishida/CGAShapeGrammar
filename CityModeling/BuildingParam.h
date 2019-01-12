#pragma once

#include <vector>
#include <glm/glm.hpp>

class BuildingParam {
public:
	std::vector<glm::vec2> footprint;
	float height;

public:
	BuildingParam(const std::vector<glm::vec2>& footprint, float height);
	~BuildingParam();
};

