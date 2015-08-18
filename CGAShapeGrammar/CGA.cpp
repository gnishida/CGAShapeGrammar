#include "CGA.h"
#include "GLUtils.h"
#include <map>

namespace cga {

const float M_PI = 3.1415926f;
const bool showAxes = false;

void Object::translate(const glm::vec3& v) {
	modelMat = glm::translate(modelMat, v);
}

void Object::setTexture(const std::string& texture) {
	this->texture = texture;
}

PrismObject::PrismObject(const glm::mat4& modelMat, const std::vector<glm::vec2>& points, float height, const glm::vec3& color) {
	this->modelMat = modelMat;
	this->points = points;
	this->height = height;
	this->color = color;
}

PrismObject PrismObject::extrude(float height) {
	return *this;
}

void PrismObject::componentSplit(Rectangle& front, std::vector<Rectangle>& sides, Polygon& top, Polygon& base) {
	// front face
	{
		front = Rectangle(glm::rotate(modelMat, M_PI * 0.5f, glm::vec3(1, 0, 0)), glm::length(points[1] - points[0]), height, color);
	}

	// side faces
	{
		glm::mat4 mat;
		sides.resize(points.size() - 1);
		for (int i = 1; i < points.size(); ++i) {
			glm::vec2 a = points[i] - points[i - 1];
			glm::vec2 b = points[(i + 1) % points.size()] - points[i];

			mat = glm::translate(mat, glm::vec3(glm::length(a), 0, 0));
			float theta = acos(glm::dot(a, b) / glm::length(a) / glm::length(b));
			mat = glm::rotate(mat, theta, glm::vec3(0, 0, 1));
			glm::mat4 mat2 = glm::rotate(mat, M_PI * 0.5f, glm::vec3(1, 0, 0));
			glm::mat4 invMat = glm::inverse(mat2);

			std::vector<glm::vec2> sidePoints(4);
			sidePoints[0] = glm::vec2(invMat * glm::vec4(points[i], 0, 1));
			sidePoints[1] = glm::vec2(invMat * glm::vec4(points[(i + 1) % points.size()], 0, 1));
			sidePoints[2] = glm::vec2(invMat * glm::vec4(points[(i + 1) % points.size()], height, 1));
			sidePoints[3] = glm::vec2(invMat * glm::vec4(points[i], height, 1));

			sides[i - 1] = Rectangle(modelMat * mat2, glm::length(points[(i + 1) % points.size()] - points[i]), height, color);
		}
	}

	// top face
	{
		top = Polygon(glm::translate(modelMat, glm::vec3(0, 0, height)), points, color);
	}

	// bottom face
	{
		std::vector<glm::vec2> basePoints = points;
		std::reverse(basePoints.begin(), basePoints.end());
		base = Polygon(modelMat, basePoints, color);
	}
}

void PrismObject::generate(std::vector<Vertex>& vertices) {
	int num = vertices.size();
	vertices.resize(num + (points.size() - 2) * 6 + points.size() * 6);

	// top
	{
		glm::vec4 p0(points.back(), height, 1);
		p0 = modelMat * p0;
		glm::vec4 normal(0, 0, 1, 0);
		normal = modelMat * normal;

		glm::vec4 p1(points[0], height, 1);
		p1 = modelMat * p1;

		for (int i = 0; i < points.size() - 2; ++i) {
			glm::vec4 p2(points[i + 1], height, 1);
			p2 = modelMat * p2;

			vertices[num + i * 3] = Vertex(glm::vec3(p0), glm::vec3(normal), color);
			vertices[num + i * 3 + 1] = Vertex(glm::vec3(p1), glm::vec3(normal), color);
			vertices[num + i * 3 + 2] = Vertex(glm::vec3(p2), glm::vec3(normal), color);

			p1 = p2;
		}

		num += (points.size() - 2) * 3;
	}

	// bottom
	{
		glm::vec4 p0(points.back(), 0, 1);
		p0 = modelMat * p0;
		glm::vec4 normal(0, 0, -1, 0);
		normal = modelMat * normal;

		glm::vec4 p1(points[0], 0, 1);
		p1 = modelMat * p1;

		for (int i = 0; i < points.size() - 2; ++i) {
			glm::vec4 p2(points[i + 1], 0, 1);
			p2 = modelMat * p2;

			vertices[num + i * 3] = Vertex(glm::vec3(p0), glm::vec3(normal), color);
			vertices[num + i * 3 + 1] = Vertex(glm::vec3(p2), glm::vec3(normal), color);
			vertices[num + i * 3 + 2] = Vertex(glm::vec3(p1), glm::vec3(normal), color);

			p1 = p2;
		}

		num += (points.size() - 2) * 3;
	}

	// side
	{
		glm::vec4 p1(points.back(), 0, 1);
		glm::vec4 p2(points.back(), height, 1);
		p1 = modelMat * p1;
		p2 = modelMat * p2;

		for (int i = 0; i < points.size(); ++i) {
			glm::vec4 p3(points[i], 0, 1);
			glm::vec4 p4(points[i], height, 1);
			p3 = modelMat * p3;
			p4 = modelMat * p4;

			glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(p3) - glm::vec3(p1), glm::vec3(p2) - glm::vec3(p1)));
			
			vertices[num + i * 6 + 0] = Vertex(glm::vec3(p1), normal, color);
			vertices[num + i * 6 + 1] = Vertex(glm::vec3(p3), normal, color);
			vertices[num + i * 6 + 2] = Vertex(glm::vec3(p4), normal, color);

			vertices[num + i * 6 + 3] = Vertex(glm::vec3(p1), normal, color);
			vertices[num + i * 6 + 4] = Vertex(glm::vec3(p4), normal, color);
			vertices[num + i * 6 + 5] = Vertex(glm::vec3(p2), normal, color);

			p1 = p3;
			p2 = p4;
		}
	}

	if (showAxes) glutils::drawAxes(0.1, 3, modelMat, vertices);
}

Rectangle::Rectangle(const glm::mat4& modelMat, float width, float height, const glm::vec3& color) {
	this->modelMat = modelMat;
	this->width = width;
	this->height = height;
	this->color = color;
}

PrismObject Rectangle::extrude(float height) {
	std::vector<glm::vec2> points(4);
	points[0] = glm::vec2(0, 0);
	points[1] = glm::vec2(width, 0);
	points[2] = glm::vec2(width, height);
	points[3] = glm::vec2(0, height);
	return PrismObject(modelMat, points, height, color);
}

void Rectangle::componentSplit(Rectangle& front, std::vector<Rectangle>& sides, Polygon& top, Polygon& base) {
	front = *this;
	sides.resize(0);
}

void Rectangle::split(int direction, const std::vector<float> ratios, std::vector<Rectangle>& rectangles) {
	rectangles.resize(ratios.size());

	float offset = 0.0f;
	
	for (int i = 0; i < ratios.size(); ++i) {
		glm::mat4 mat;
		if (direction == DIRECTION_X) {
			mat = glm::translate(glm::mat4(), glm::vec3(offset, 0, 0));
			rectangles[i] = Rectangle(modelMat * mat, width * ratios[i], height, color);
			offset += width * ratios[i];
		} else {
			mat = glm::translate(glm::mat4(), glm::vec3(0, offset, 0));
			rectangles[i] = Rectangle(modelMat * mat, width, height * ratios[i], color);
			offset += height * ratios[i];
		}
	}
}

void Rectangle::generate(std::vector<Vertex>& vertices) {
	int num = vertices.size();
	vertices.resize(num + 6);

	glm::vec4 p1(0, 0, 0, 1);
	p1 = modelMat * p1;
	glm::vec4 p2(width, 0, 0, 1);
	p2 = modelMat * p2;
	glm::vec4 p3(width, height, 0, 1);
	p3 = modelMat * p3;
	glm::vec4 p4(0, height, 0, 1);
	p4 = modelMat * p4;

	glm::vec4 normal(0, 0, 1, 0);
	normal = modelMat * normal;

	vertices[num + 0] = Vertex(glm::vec3(p1), glm::vec3(normal), color);
	vertices[num + 1] = Vertex(glm::vec3(p2), glm::vec3(normal), color);
	vertices[num + 2] = Vertex(glm::vec3(p3), glm::vec3(normal), color);

	vertices[num + 3] = Vertex(glm::vec3(p1), glm::vec3(normal), color);
	vertices[num + 4] = Vertex(glm::vec3(p3), glm::vec3(normal), color);
	vertices[num + 5] = Vertex(glm::vec3(p4), glm::vec3(normal), color);

	if (showAxes) glutils::drawAxes(0.1, 3, modelMat, vertices);
}

Polygon::Polygon(const glm::mat4& modelMat, const std::vector<glm::vec2> points, const glm::vec3& color) {
	this->modelMat = modelMat;
	this->points = points;
	this->color = color;
}

PrismObject Polygon::extrude(float height) {
	return PrismObject(modelMat, points, height, color);
}

void Polygon::componentSplit(Rectangle& front, std::vector<Rectangle>& sides, Polygon& top, Polygon& base) {
}

void Polygon::split(int direction, const std::vector<float> ratios, std::vector<Rectangle>& rectangles) {
}


void Polygon::generate(std::vector<Vertex>& vertices) {
	int num = vertices.size();
	vertices.resize(num + (points.size() - 2) * 3);

	glm::vec4 p0(points[0], 0, 1);
	p0 = modelMat * p0;
	glm::vec4 normal(0, 0, 1, 0);
	normal = modelMat * normal;

	glm::vec4 p1(points[1], 0, 1);
	p1 = modelMat * p1;

	for (int i = 1; i < points.size() - 1; ++i) {
		glm::vec4 p2(points[i + 1], 0, 1);
		p2 = modelMat * p2;

		vertices[num + (i - 1) * 3] = Vertex(glm::vec3(p0), glm::vec3(normal), color);
		vertices[num + (i - 1) * 3 + 1] = Vertex(glm::vec3(p1), glm::vec3(normal), color);
		vertices[num + (i - 1) * 3 + 2] = Vertex(glm::vec3(p2), glm::vec3(normal), color);

		p1 = p2;
	}

	if (showAxes) glutils::drawAxes(0.1, 3, modelMat, vertices);
}

CGA::CGA() {
}

void CGA::generate(std::vector<Vertex>& vertices) {
	Rectangle lot = Rectangle(glm::rotate(glm::mat4(), -M_PI * 0.5f, glm::vec3(1, 0, 0)), 35, 10, glm::vec3(1, 1, 1));
	//lot.generate(vertices);

	PrismObject building = lot.extrude(11);
	//building.generate(vertices);

	Rectangle frontFacade;
	Polygon roof, base;
	std::vector<Rectangle> sideFacades;
	building.componentSplit(frontFacade, sideFacades, roof, base);

	/*front.generate(vertices);
	top.generate(vertices);
	base.generate(vertices);
	sides[0].generate(vertices);
	sides[1].generate(vertices);
	sides[2].generate(vertices);*/

	std::vector<Rectangle> floors;

	{
		// 正面を、3階に分割
		std::vector<float> floor_ratios(3);
		floor_ratios[0] = 4.0f / frontFacade.height;
		floor_ratios[1] = 3.5f / frontFacade.height;
		floor_ratios[2] = 3.5f / frontFacade.height;
		frontFacade.split(DIRECTION_Y, floor_ratios, floors);

		// 側面を、3階に分割
		for (int i = 0; i < sideFacades.size(); ++i) {
			std::vector<Rectangle> sideFloors;
			sideFacades[i].split(DIRECTION_Y, floor_ratios, sideFloors);

			floors.insert(floors.end(), sideFloors.begin(), sideFloors.end());
		}
	}

	for (int i = 0; i < floors.size(); ++i) {
		std::vector<float> tile_ratios;
		{
			float margin_ratio = 1.0f / floors[i].width;
			int numTiles = (floors[i].width - 2.0f) / 3.0f;
			float tile_width_ratio = (floors[i].width - 2.0f) / numTiles / floors[i].width;

			tile_ratios.push_back(margin_ratio);
			for (int i = 0; i < numTiles; ++i) {
				tile_ratios.push_back(tile_width_ratio);
			}
			tile_ratios.push_back(margin_ratio);
		}

		std::vector<Rectangle> tiles;
		floors[i].split(DIRECTION_X, tile_ratios, tiles);

		for (int j = 0; j < tiles.size(); ++j) {
			std::vector<float> wall_ratios(3);
			wall_ratios[0] = 0.1f;
			wall_ratios[1] = 0.8f;
			wall_ratios[2] = 0.1f;

			std::vector<Rectangle> walls;
			tiles[j].split(DIRECTION_X, wall_ratios, walls);

			for (int k = 0; k < walls.size(); ++k) {
				if (k == 1) continue;
				walls[k].generate(vertices);
			}

			{
				std::vector<float> subwall_ratios(3);
				subwall_ratios[0] = 0.2f;
				subwall_ratios[1] = 0.6f;
				subwall_ratios[2] = 0.2f;

				std::vector<Rectangle> sub_walls;
				walls[1].split(DIRECTION_Y, subwall_ratios, sub_walls);

				for (int l = 0; l < sub_walls.size(); ++l) {
					if (l == 1) {
						sub_walls[l].translate(glm::vec3(0, 0, -0.25f));
					}

					sub_walls[l].generate(vertices);
				}
			}
		}
	}
	
	// 屋根
	roof.generate(vertices);
}

}
