#include "RoadGraph.h"

RoadVertex::RoadVertex(const glm::vec2& pos) : pos(pos) {
}

RoadEdge::RoadEdge(const std::vector<glm::vec2>& polyline) : polyline(polyline) {
}

RoadGraph::RoadGraph() {
}

RoadGraph::~RoadGraph() {
}

void RoadGraph::clear() {
	vertices.clear();
	edges.clear();
}