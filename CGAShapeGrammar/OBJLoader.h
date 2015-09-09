#pragma once

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "Vertex.h"

class OBJLoader {
protected:
	OBJLoader() {}

public:
	static void load(const char* filename, std::vector<Vertex>& vertices);
	static void load(const char* filename, std::vector<glm::vec3>& points, std::vector<glm::vec3>& normals, std::vector<glm::vec3>& texCoords);
};

