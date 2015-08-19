#include "CGA.h"
#include "GLUtils.h"
#include <map>

namespace cga {

const float M_PI = 3.1415926f;
const bool showAxes = false;

BoundingBox::BoundingBox(const std::vector<glm::vec2>& points) {
	bottom_left = glm::vec2((std::numeric_limits<float>::max)(), (std::numeric_limits<float>::max)());
	upper_right = -bottom_left;

	for (int i = 0; i < points.size(); ++i) {
		bottom_left.x = min(bottom_left.x, points[i].x);
		bottom_left.y = min(bottom_left.y, points[i].y);
		upper_right.x = max(upper_right.x, points[i].x);
		upper_right.y = max(upper_right.y, points[i].y);
	}
}

void Object::translate(const glm::vec3& v) {
	modelMat = glm::translate(modelMat, v);
}

void Object::setTexture(const std::string& texture) {
	this->texture = texture;
}

PrismObject::PrismObject(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, float height, const glm::vec3& color) {
	this->name = name;
	this->modelMat = modelMat;
	this->points = points;
	this->height = height;
	this->color = color;

	BoundingBox bbox(points);
	this->scope = glm::vec3(bbox.upper_right.x, bbox.upper_right.y, height);
}

void PrismObject::setupProjection(float texWidth, float texHeight) {
}

PrismObject PrismObject::extrude(const std::string& name, float height) {
	throw "PrismObject does not support extrude operation.";
}

void PrismObject::componentSplit(const std::string& front_name, Rectangle& front, const std::string& sides_name, std::vector<Rectangle>& sides, const std::string& top_name, Polygon& top, const std::string& base_name, Polygon& base) {
	// front face
	{
		front = Rectangle(front_name, glm::rotate(modelMat, M_PI * 0.5f, glm::vec3(1, 0, 0)), glm::length(points[1] - points[0]), height, color, texture);
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

			sides[i - 1] = Rectangle(sides_name, modelMat * mat2, glm::length(points[(i + 1) % points.size()] - points[i]), height, color, texture);
		}
	}

	// top face
	{
		top = Polygon(top_name, glm::translate(modelMat, glm::vec3(0, 0, height)), points, color, texture);
	}

	// bottom face
	{
		std::vector<glm::vec2> basePoints = points;
		std::reverse(basePoints.begin(), basePoints.end());
		base = Polygon(base_name, modelMat, basePoints, color, texture);
	}
}

void PrismObject::generate(RenderManager* renderManager) {
	std::vector<Vertex> vertices((points.size() - 2) * 6 + points.size() * 6);

	int num = 0;

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

			vertices[i * 3] = Vertex(glm::vec3(p0), glm::vec3(normal), color);
			vertices[i * 3 + 1] = Vertex(glm::vec3(p1), glm::vec3(normal), color);
			vertices[i * 3 + 2] = Vertex(glm::vec3(p2), glm::vec3(normal), color);

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

	renderManager->addObject(name.c_str(), "", vertices);

	if (showAxes) {
		vertices.resize(0);
		glutils::drawAxes(0.1, 3, modelMat, vertices);
		renderManager->addObject("axis", "", vertices);
	}
}

Rectangle::Rectangle(const std::string& name, const glm::mat4& modelMat, float width, float height, const glm::vec3& color, const std::string& texture) {
	this->name = name;
	this->modelMat = modelMat;
	this->width = width;
	this->height = height;
	this->color = color;
	this->texture = texture;
	this->scope = glm::vec3(width, height, 0);
}

void Rectangle::setupProjection(float texWidth, float texHeight) {
	setupProjection(0, 0, width / texWidth, height / texHeight);
	/*
	texCoords.resize(4);
	texCoords[0] = glm::vec2(0, 0);
	texCoords[1] = glm::vec2(width / texWidth, 0);
	texCoords[2] = glm::vec2(width / texWidth, height / texHeight);
	texCoords[3] = glm::vec2(0, height / texHeight);
	*/
}

void Rectangle::setupProjection(float u1, float v1, float u2, float v2) {
	texCoords.resize(4);
	texCoords[0] = glm::vec2(u1, v1);
	texCoords[1] = glm::vec2(u2, v1);
	texCoords[2] = glm::vec2(u2, v2);
	texCoords[3] = glm::vec2(u1, v2);
}

PrismObject Rectangle::extrude(const std::string& name, float height) {
	std::vector<glm::vec2> points(4);
	points[0] = glm::vec2(0, 0);
	points[1] = glm::vec2(width, 0);
	points[2] = glm::vec2(width, height);
	points[3] = glm::vec2(0, height);
	return PrismObject(name, modelMat, points, height, color);
}

void Rectangle::componentSplit(const std::string& front_name, Rectangle& front, const std::string& sides_name, std::vector<Rectangle>& sides, const std::string& top_name, Polygon& top, const std::string& base_name, Polygon& base) {
	throw "Rectangle does not support componentSplit operation.";
}

void Rectangle::split(int direction, const std::vector<float> ratios, const std::vector<std::string> names, std::vector<Rectangle>& rectangles) {
	rectangles.resize(ratios.size());

	float offset = 0.0f;
	
	for (int i = 0; i < ratios.size(); ++i) {
		glm::mat4 mat;
		if (direction == DIRECTION_X) {
			mat = glm::translate(glm::mat4(), glm::vec3(offset, 0, 0));
			rectangles[i] = Rectangle(names[i], modelMat * mat, width * ratios[i], height, color, texture);
			rectangles[i].setupProjection(
				texCoords[0].x + (texCoords[1].x - texCoords[0].x) * offset / width, texCoords[0].y,
				texCoords[0].x + (texCoords[1].x - texCoords[0].x) * (offset / width + ratios[i]), texCoords[2].y);
			offset += width * ratios[i];
		} else {
			mat = glm::translate(glm::mat4(), glm::vec3(0, offset, 0));
			rectangles[i] = Rectangle(names[i], modelMat * mat, width, height * ratios[i], color, texture);
			rectangles[i].setupProjection(
				texCoords[0].x, texCoords[0].y + (texCoords[2].y - texCoords[0].y) * offset / height,
				texCoords[1].x, texCoords[0].y + (texCoords[2].y - texCoords[0].y) * (offset / height + ratios[i]));
			offset += height * ratios[i];
		}
	}
}

void Rectangle::generate(RenderManager* renderManager) {
	std::vector<Vertex> vertices;

	vertices.resize(6);

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

	vertices[0] = Vertex(glm::vec3(p1), glm::vec3(normal), color, glm::vec3(texCoords[0], 0));
	vertices[1] = Vertex(glm::vec3(p2), glm::vec3(normal), color, glm::vec3(texCoords[1], 0));
	vertices[2] = Vertex(glm::vec3(p3), glm::vec3(normal), color, glm::vec3(texCoords[2], 0));

	vertices[3] = Vertex(glm::vec3(p1), glm::vec3(normal), color, glm::vec3(texCoords[0], 0));
	vertices[4] = Vertex(glm::vec3(p3), glm::vec3(normal), color, glm::vec3(texCoords[2], 0));
	vertices[5] = Vertex(glm::vec3(p4), glm::vec3(normal), color, glm::vec3(texCoords[3], 0));

	renderManager->addObject(name.c_str(), texture.c_str(), vertices);

	vertices.resize(0);
	if (showAxes) {
		glutils::drawAxes(0.1, 3, modelMat, vertices);
		renderManager->addObject("axis", "", vertices);
	}
}

Polygon::Polygon(const std::string& name, const glm::mat4& modelMat, const std::vector<glm::vec2> points, const glm::vec3& color, const std::string& texture) {
	this->name = name;
	this->modelMat = modelMat;
	this->points = points;
	this->color = color;
	this->texture = texture;

	BoundingBox bbox(points);
	this->scope = glm::vec3(bbox.upper_right.x, bbox.upper_right.y, 0);
}

void Polygon::setupProjection(float texWidth, float texHeight) {
	texCoords.resize(points.size());
	for (int i = 0; i < points.size(); ++i) {
		texCoords[i] = glm::vec2(points[i].x / texWidth, points[i].y / texHeight);
	}
}

PrismObject Polygon::extrude(const std::string& name, float height) {
	return PrismObject(name, modelMat, points, height, color);
}

void Polygon::componentSplit(const std::string& front_name, Rectangle& front, const std::string& sides_name, std::vector<Rectangle>& sides, const std::string& top_name, Polygon& top, const std::string& base_name, Polygon& base) {
	throw "Polygon does not support componentSplit operation.";
}

void Polygon::split(int direction, const std::vector<float> ratios, const std::vector<std::string> names, std::vector<Rectangle>& rectangles) {
}


void Polygon::generate(RenderManager* renderManager) {
	std::vector<Vertex> vertices((points.size() - 2) * 3);

	glm::vec4 p0(points[0], 0, 1);
	p0 = modelMat * p0;
	glm::vec4 normal(0, 0, 1, 0);
	normal = modelMat * normal;

	glm::vec4 p1(points[1], 0, 1);
	p1 = modelMat * p1;

	glm::vec3 uv1(points[1].x / scope.x, points[1].y / scope.y, 0);

	for (int i = 1; i < points.size() - 1; ++i) {
		glm::vec4 p2(points[i + 1], 0, 1);
		p2 = modelMat * p2;

		glm::vec3 uv2(points[i + 1].x / scope.x, points[i + 1].y / scope.y, 0);

		vertices[(i - 1) * 3] = Vertex(glm::vec3(p0), glm::vec3(normal), color, glm::vec3(0, 0, 0));
		vertices[(i - 1) * 3 + 1] = Vertex(glm::vec3(p1), glm::vec3(normal), color, uv1);
		vertices[(i - 1) * 3 + 2] = Vertex(glm::vec3(p2), glm::vec3(normal), color, uv2);

		p1 = p2;
		uv1 = uv2;
	}

	renderManager->addObject(name.c_str(), texture.c_str(), vertices);

	if (showAxes) {
		vertices.resize(0);
		glutils::drawAxes(0.1, 3, modelMat, vertices);
		renderManager->addObject("axis", "", vertices);
	}
}

CGA::CGA() {
}

void CGA::generate(RenderManager* renderManager) {
	Rectangle lot = Rectangle("Lot", glm::rotate(glm::mat4(), -M_PI * 0.5f, glm::vec3(1, 0, 0)), 35, 10, glm::vec3(1, 1, 1), "");
	//lot.setTexture("textures/roof.jpg");
	//lot.generate(renderManager);

	PrismObject building = lot.extrude("Building", 11);
	//building.generate(vertices);

	Rectangle frontFacade;
	Polygon roof, base;
	std::vector<Rectangle> sideFacades;
	building.componentSplit("FrontFacade", frontFacade, "SideFacade", sideFacades, "Roof", roof, "Base", base);
	frontFacade.setupProjection(2.5f, 1.0f);
	frontFacade.setTexture("textures/brick.jpg");
	for (int i = 0; i < sideFacades.size(); ++i) {
		sideFacades[i].setupProjection(2.5f, 1.0f);
		sideFacades[i].setTexture("textures/brick.jpg");
	}

	std::vector<Rectangle> floors;

	{
		// 正面を、3階に分割
		std::vector<float> floor_ratios(3);
		floor_ratios[0] = 4.0f / frontFacade.height;
		floor_ratios[1] = 3.5f / frontFacade.height;
		floor_ratios[2] = 3.5f / frontFacade.height;
		std::vector<std::string> floor_names(3);
		floor_names[0] = "Floor";
		floor_names[1] = "Floor";
		floor_names[2] = "Floor";
		frontFacade.split(DIRECTION_Y, floor_ratios, floor_names, floors);

		// 側面を、3階に分割
		for (int i = 0; i < sideFacades.size(); ++i) {
			std::vector<Rectangle> sideFloors;
			sideFacades[i].split(DIRECTION_Y, floor_ratios, floor_names, sideFloors);

			floors.insert(floors.end(), sideFloors.begin(), sideFloors.end());
		}
	}

	/*
	for (int i = 0; i < floors.size(); ++i) {
		floors[i].generate(renderManager);
	}
	*/

	for (int i = 0; i < floors.size(); ++i) {
		std::vector<float> tile_ratios;
		std::vector<std::string> tile_names;
		{
			float margin_ratio = 1.0f / floors[i].width;
			int numTiles = (floors[i].width - 2.0f) / 3.0f;
			float tile_width_ratio = (floors[i].width - 2.0f) / numTiles / floors[i].width;

			tile_ratios.push_back(margin_ratio);
			tile_names.push_back("Wall");
			for (int i = 0; i < numTiles; ++i) {
				tile_ratios.push_back(tile_width_ratio);
				tile_names.push_back("Tile");
			}
			tile_ratios.push_back(margin_ratio);
			tile_names.push_back("Wall");
		}

		std::vector<Rectangle> tiles;
		floors[i].split(DIRECTION_X, tile_ratios, tile_names, tiles);

		for (int j = 0; j < tiles.size(); ++j) {
			std::vector<float> wall_ratios(3);
			wall_ratios[0] = 0.1f;
			wall_ratios[1] = 0.8f;
			wall_ratios[2] = 0.1f;
			std::vector<std::string> wall_names(3);
			wall_names[0] = "Wall";
			wall_names[1] = "WallWindowWall";
			wall_names[2] = "Wall";

			std::vector<Rectangle> walls;
			tiles[j].split(DIRECTION_X, wall_ratios, wall_names, walls);

			for (int k = 0; k < walls.size(); ++k) {
				if (k == 1) continue;
				walls[k].generate(renderManager);
			}

			{
				std::vector<float> subwall_ratios(3);
				subwall_ratios[0] = 0.2f;
				subwall_ratios[1] = 0.6f;
				subwall_ratios[2] = 0.2f;
				std::vector<std::string> subwall_names(3);
				subwall_names[0] = "Wall";
				subwall_names[1] = "Window";
				subwall_names[2] = "Wall";

				std::vector<Rectangle> sub_walls;
				walls[1].split(DIRECTION_Y, subwall_ratios, subwall_names, sub_walls);

				for (int l = 0; l < sub_walls.size(); ++l) {
					if (l == 1) {
						sub_walls[l].translate(glm::vec3(0, 0, -0.25f));
						sub_walls[l].setupProjection(sub_walls[l].scope.x, sub_walls[l].scope.y);
						sub_walls[l].setTexture("textures/window.jpg");
					}

					sub_walls[l].generate(renderManager);
				}
			}
		}
	}
	
	// 屋根
	roof.setupProjection(roof.scope.x, roof.scope.y);
	roof.setTexture("textures/roof.jpg");
	roof.generate(renderManager);
}

}
