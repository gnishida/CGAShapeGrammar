#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

class RoadVertex {
public:
	glm::vec2 pos;

public:
	RoadVertex(const glm::vec2& pos);
};
using RoadVertexPtr = std::shared_ptr<RoadVertex>;

class RoadEdge {
public:
	std::vector<glm::vec2> polyline;

public:
	RoadEdge(const std::vector<glm::vec2>& polyline);
};
using RoadEdgePtr = std::shared_ptr<RoadEdge>;

class RoadGraph {
public:
	std::vector<RoadVertexPtr> vertices;
	std::vector<std::vector<std::pair<int, RoadEdgePtr>>> edges;

public:
	RoadGraph();
	~RoadGraph();

	void clear();
};

