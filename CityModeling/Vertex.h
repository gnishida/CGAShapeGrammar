#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>

namespace procedural_generation {

	class Vertex {
	public:
		Vertex() {}
		Vertex(const glm::vec3& pos, const glm::vec3& normal, const glm::vec4& color) : pos(pos), normal(normal), color(color) {}
		Vertex(const glm::vec3& pos, const glm::vec3& normal, const glm::vec4& color, const glm::vec2& texCoord) : pos(pos), normal(normal), color(color), texCoord(texCoord) {}

	public:
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec4 color;
		glm::vec2 texCoord;
	};

	using Polygon2D = std::vector<glm::vec2>;
	using Polygon3D = std::vector<glm::vec3>;

	struct Polygon2DWithHoles {
		Polygon2D outerContour;
		std::vector<Polygon2D> holes;

		Polygon2DWithHoles() = default;
		Polygon2DWithHoles(const Polygon2D& outerContour) : outerContour(outerContour) {}
		Polygon2DWithHoles(const Polygon2D& outerContour, const std::vector<Polygon2D>& holes) : outerContour(outerContour), holes(holes) {}
	};

	struct Face {
		std::vector<Vertex> vertices;
		std::string texturePath;
	};

	using Faces = std::vector<Face>;

}