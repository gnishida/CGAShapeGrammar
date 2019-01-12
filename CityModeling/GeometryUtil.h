#pragma once

#include "Vertex.h"
#include "BoundingBox.h"
#include <vector>
#include <glm/glm.hpp>
#include <iostream>

namespace procedural_generation {

	std::vector<Polygon2D> Triangulate(const Polygon2D& poly);
	std::vector<Polygon2D> Triangulate(const Polygon2D& poly, const std::vector<Polygon2D>& holes);

	std::vector<Polygon2DWithHoles> UnionPolygons(const Polygon2D& polygon1, const Polygon2D& polygon2);
	std::vector<Polygon2DWithHoles> UnionPolygons(const std::vector<Polygon2D>& polygons);
	std::vector<Polygon2DWithHoles> UnionPolygons(const std::vector<Polygon2DWithHoles>& polygons);
	std::vector<Polygon2DWithHoles> DifferencePolygons(const Polygon2D& polygon1, const Polygon2D& polygon2);
	std::vector<Polygon2DWithHoles> DifferencePolygons(const Polygon2DWithHoles& polygon1, const Polygon2D& polygon2);
	std::vector<Polygon2DWithHoles> DifferencePolygons(const std::vector<Polygon2D>& polygons1, const std::vector<Polygon2D>& polygons2);
	std::vector<Polygon2DWithHoles> DifferencePolygons(const std::vector<Polygon2DWithHoles>& polygons1, const std::vector<Polygon2D>& polygons2);
	std::vector<Polygon2DWithHoles> IntersectPolygons(const Polygon2D& polygon1, const Polygon2D& polygon2);
	std::vector<Polygon2DWithHoles> IntersectPolygons(const Polygon2DWithHoles& polygon1, const Polygon2D& polygon2);
	bool IsIntersect(const Polygon2D& polygon1, const Polygon2D& polygon2);

	float DistanceFromPointToSegment(const glm::vec2& pt, const glm::vec2& a, const glm::vec2& b, glm::vec2& closestPt);
	float DistanceFromPointToPolygon(const glm::vec2& pt, const Polygon2D& polygon, glm::vec2& closestPt);
	bool IsClockwise(const Polygon2D& points);
	void OrientPolygon(Polygon2D& points);
	void RemoveRedundantPoints(Polygon2D& points);
	void OrientedBoundingBox(const Polygon2D& polygon, glm::mat4& xformMat, BoundingBox& bbox);
	bool WithinPolygon(const glm::vec2& pt, const Polygon2D& polygon);

	enum class OffsetPolygonType { CGAL, SQUARE, ROUND, MITER };
	void OffsetPolygon(const Polygon2D& points, float offsetDistance, OffsetPolygonType offsetPolygonType, std::vector<Polygon2D>& offsetPoints);
	bool IsSimple(const Polygon2D& polygon);

	enum class TextureCoordinateCalculationType { LOCAL_COORDINATE, GLOBAL_COORDINATE };

	void GenerateRectangle(float width, float height, const glm::vec4& color, const glm::mat4& xformMat, Faces& faces);
	void GenerateRectangle(float width, float height, const glm::vec4& color, const std::vector<glm::vec2>& texCoords, const std::string& texturePath, const glm::mat4& xformMat, Faces& faces);
	void GeneratePolygon(const Polygon2D& points, const glm::vec4& color, const glm::mat4& xformMat, Faces& faces);
	void GeneratePolygon(const Polygon2D& points, const glm::vec4& color, const std::vector<glm::vec2>& texCoords, const std::string& texturePath, const glm::mat4& xformMat, Faces& faces);
	void GeneratePolygon(const Polygon2D& points, const glm::vec4& color, float textureWidth, float textureHeight, TextureCoordinateCalculationType textureCoordinateCalculationType, const std::string& texturePath, const glm::mat4& xformMat, Faces& faces);
	void GeneratePolygon(const Polygon2DWithHoles& polygonWithHoles, const glm::vec4& color, const glm::mat4& xformMat, Faces& faces);
	void GeneratePolygon(const Polygon2DWithHoles& polygonWithHoles, const glm::vec4& color, float textureWidth, float textureHeight, TextureCoordinateCalculationType textureCoordinateCalculationType, const std::string& texturePath, const glm::mat4& xformMat, Faces& faces);
	void GeneratePolygon(const Polygon3D& points, const glm::vec4& color, const glm::mat4& xformMat, Faces& faces);
	void GeneratePolygon(const Polygon3D& points, const glm::vec4& color, float textureWidth, float textureHeight, TextureCoordinateCalculationType textureCoordinateCalculationType, const std::string& texturePath, const glm::mat4& xformMat, Faces& faces);
	void GeneratePrism(const Polygon2D& points, const glm::vec4& color, const glm::mat4& xformMat, float height, Faces& faces);
	void GeneratePrism(const Polygon2DWithHoles& polygonWithHoles, const glm::vec4& color, const glm::mat4& xformMat, float height, Faces& faces);
	void GeneratePrism(const Polygon2D& points, const glm::vec4& color, float textureWidth, float textureHeight, TextureCoordinateCalculationType textureCoordinateCalculationType, const std::string& texturePath, const glm::mat4& xformMat, float height, Faces& faces);
	void GeneratePrism(const Polygon2DWithHoles& polygonWithHoles, const glm::vec4& color, float textureWidth, float textureHeight, TextureCoordinateCalculationType textureCoordinateCalculationType, const std::string& texturePath, const glm::mat4& xformMat, float height, Faces& faces);

}
