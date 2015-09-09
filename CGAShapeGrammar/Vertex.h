#pragma once

#include <glm/glm.hpp>

/**
 * This structure defines a vertex data.
 */
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texCoord;

	Vertex() {}

	Vertex(const glm::vec3& pos, const glm::vec3& n) {
		position = pos;
		normal = n;
	}

	Vertex(const glm::vec3& pos, const glm::vec3& n, const glm::vec3& c) {
		position = pos;
		normal = n;
		color = c;
	}

	Vertex(const glm::vec3& pos, const glm::vec3& n, const glm::vec3& c, const glm::vec2& tex) {
		position = pos;
		normal = n;
		color = c;
		texCoord = tex;
	}
};
