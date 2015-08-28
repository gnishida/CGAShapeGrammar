#include "CGA.h"
#include "GLUtils.h"
#include <map>

#include "Rectangle.h"
#include "Polygon.h"


namespace cga {

//const float M_PI = 3.1415926f;
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
	_modelMat = glm::translate(_modelMat, v);
}

void Object::rotate(float xAngle, float yAngle, float zAngle) {
	_modelMat = glm::rotate(_modelMat, xAngle * M_PI / 180.0f, glm::vec3(1, 0, 0));
	_modelMat = glm::rotate(_modelMat, yAngle * M_PI / 180.0f, glm::vec3(0, 1, 0));
	_modelMat = glm::rotate(_modelMat, zAngle * M_PI / 180.0f, glm::vec3(0, 0, 1));
}

void Object::nil() {
	_removed = true;
}

void Object::setTexture(const std::string& texture) {
	this->_texture = texture;
	_textureEnabled = true;
}

Object* Object::clone() {
	throw "clone() is not supported.";
}

void Object::setupProjection(float texWidth, float texHeight) {
	throw "setupProjection() is not supported.";
}

Object* Object::extrude(const std::string& name, float height) {
	throw "extrude() is not supported.";
}

Object* Object::taper(const std::string& name, float height, float top_ratio) {
	throw "taper() is not supported.";
}

Object* Object::offset(const std::string& name, float offsetRatio) {
	throw "offset() is not supported.";
}

Object* Object::inscribeCircle(const std::string& name) {
	throw "inscribeCircle() is not supported.";
}

void Object::split(int direction, const std::vector<float> ratios, const std::vector<std::string> names, std::vector<Object*>& objects) {
	throw "split() is not supported.";
}

void Object::componentSplit(const std::string& front_name, Rectangle** front, const std::string& sides_name, std::vector<Rectangle*>& sides, const std::string& top_name, Polygon** top, const std::string& base_name, Polygon** base) {
	throw "componentSplit() is not supported.";
}

Object* Object::roofHip(const std::string& name, float angle) {
	throw "roofHip() is not supported.";
}

void Object::generate(RenderManager* renderManager) {
	throw "generate() is not supported.";
}

CGA::CGA() {
}

void CGA::generate(RenderManager* renderManager, std::map<std::string, Rule*>& rules, std::list<Object*> stack) {
	while (!stack.empty()) {
		Object* obj = stack.front();
		stack.pop_front();

		if (rules.find(obj->_name) == rules.end()) {
			obj->generate(renderManager);

		} else {
			rules[obj->_name]->apply(obj, stack);
		}
		delete obj;
	}

}

/*void CGA::generatePyramid(RenderManager* renderManager) {
	std::list<Object*> stack;
	
	Rectangle* lot = new Rectangle("Lot", modelMat, 20, 20, glm::vec3(1, 1, 1));
	stack.push_back(lot);
	
	while (!stack.empty()) {
		Object* obj = stack.front();
		stack.pop_front();

		if (obj->_removed) {
			delete obj;
			continue;
		}
		
		if (obj->_name == "Lot") {
			stack.push_back(obj->taper("pyramid", 10));
		} else {
			obj->generate(renderManager);
			delete obj;
		}
	}
}*/

void CGA::generateSimpleBuilding(RenderManager* renderManager) {
	/*
	std::list<Object*> stack;
	
	Rectangle* lot = new Rectangle("Lot", modelMat, 35, 10, glm::vec3(1, 1, 1));
	stack.push_back(lot);
	
	while (!stack.empty()) {
		Object* obj = stack.front();
		stack.pop_front();

		if (obj->_removed) {
			delete obj;
			continue;
		}
		
		if (obj->_name == "Lot") {
			stack.push_back(obj->extrude("Building", 20));
		} else {
			obj->generate(renderManager);
			delete obj;
		}
	}
	*/
}

/*void CGA::generateHouse(RenderManager* renderManager) {
	std::list<Object*> stack;

	Rectangle* lot = new Rectangle("Lot", modelMat, 10, 6, glm::vec3(1, 1, 1));
	stack.push_back(lot);
	
	while (!stack.empty()) {
		Object* obj = stack.front();
		stack.pop_front();

		if (obj->_removed) {
			delete obj;
			continue;
		}
		
		if (obj->_name == "Lot") {
			stack.push_back(obj->extrude("House", 6));
		} else if (obj->_name == "House") {
			Rectangle* frontFacade;
			Polygon* roof;
			Polygon* base;
			std::vector<Rectangle*> sideFacades;
			obj->componentSplit("FrontFacade", &frontFacade, "SideFacade", sideFacades, "Roof", &roof, "Base", &base);

			stack.push_back(frontFacade);
			for (int i = 0; i < sideFacades.size(); ++i) {
				stack.push_back(sideFacades[i]);
			}
			stack.push_back(roof);
			stack.push_back(base);
		} else if (obj->_name == "Roof") {
			stack.push_back(obj->roofHip("HipRoof", 30));
		} else if (obj->_name == "FrontFacade") {
			std::vector<float> floor_ratios(2);
			floor_ratios[0] = 0.5f;
			floor_ratios[1] = 0.5f;
			std::vector<std::string> floor_names(2);
			floor_names[0] = "Entrance";
			floor_names[1] = "Floor";

			std::vector<Object*> floors;
			obj->split(DIRECTION_Y, floor_ratios, floor_names, floors);

			stack.insert(stack.end(), floors.begin(), floors.end());
		} else if (obj->_name == "SideFacade") {
			std::vector<float> floor_ratios(2);
			floor_ratios[0] = 0.5f;
			floor_ratios[1] = 0.5f;
			std::vector<std::string> floor_names(2);
			floor_names[0] = "Floor";
			floor_names[1] = "Floor";

			std::vector<Object*> floors;
			obj->split(DIRECTION_Y, floor_ratios, floor_names, floors);

			stack.insert(stack.end(), floors.begin(), floors.end());
		} else if (obj->_name == "Floor") {
			std::vector<float> tile_ratios;
			std::vector<std::string> tile_names;

			// 分割比率、名前をセット
			float margin_ratio = 1.0f / obj->_scope.x;
			int numTiles = (obj->_scope.x - 2.0f) / 3.0f;
			float tile_width_ratio = (obj->_scope.x - 2.0f) / numTiles / obj->_scope.x;

			tile_ratios.push_back(margin_ratio);
			tile_names.push_back("Wall");
			for (int i = 0; i < numTiles; ++i) {
				tile_ratios.push_back(tile_width_ratio);
				tile_names.push_back("Tile");
			}
			tile_ratios.push_back(margin_ratio);
			tile_names.push_back("Wall");

			std::vector<Object*> tiles;
			obj->split(DIRECTION_X, tile_ratios, tile_names, tiles);

			stack.insert(stack.end(), tiles.begin(), tiles.end());
		} else if (obj->_name == "Tile") {
			std::vector<float> wall_ratios(3);
			wall_ratios[0] = 0.1f;
			wall_ratios[1] = 0.8f;
			wall_ratios[2] = 0.1f;
			std::vector<std::string> wall_names(3);
			wall_names[0] = "Wall";
			wall_names[1] = "WallWindowWall";
			wall_names[2] = "Wall";

			std::vector<Object*> walls;
			obj->split(DIRECTION_X, wall_ratios, wall_names, walls);

			stack.insert(stack.end(), walls.begin(), walls.end());
		} else if (obj->_name == "WallWindowWall") {
			std::vector<float> wall_ratios(3);
			wall_ratios[0] = 0.2f;
			wall_ratios[1] = 0.6f;
			wall_ratios[2] = 0.2f;
			std::vector<std::string> wall_names(3);
			wall_names[0] = "Wall";
			wall_names[1] = "Window";
			wall_names[2] = "Wall";

			std::vector<Object*> walls;
			obj->split(DIRECTION_Y, wall_ratios, wall_names, walls);

			stack.insert(stack.end(), walls.begin(), walls.end());
		} else {
			obj->generate(renderManager);
			delete obj;
		}
	}
}

void CGA::generateBuilding(RenderManager* renderManager) {
	std::list<Object*> stack;
	
	Rectangle* lot = new Rectangle("Lot", modelMat, 35, 10, glm::vec3(1, 1, 1));
	stack.push_back(lot);
	
	while (!stack.empty()) {
		Object* obj = stack.front();
		stack.pop_front();

		if (obj->_removed) {
			delete obj;
			continue;
		}
		
		if (obj->_name == "Lot") {
			stack.push_back(obj->extrude("Building", 11));
		} else if (obj->_name == "Building") {
			Rectangle* frontFacade;
			Polygon* roof;
			Polygon* base;
			std::vector<Rectangle*> sideFacades;
			obj->componentSplit("FrontFacade", &frontFacade, "SideFacade", sideFacades, "Roof", &roof, "Base", &base);

			stack.push_back(frontFacade);
			for (int i = 0; i < sideFacades.size(); ++i) {
				stack.push_back(sideFacades[i]);
			}
			stack.push_back(roof);
			stack.push_back(base);
		} else if (obj->_name == "Roof") {
			obj->setupProjection(obj->_scope.x, obj->_scope.y);
			obj->setTexture("textures/roof.jpg");

			Object* obj2 = obj->clone();
			obj2->_name = "Roof_1";
			stack.push_back(obj2);
		} else if (obj->_name == "FrontFacade") {
			obj->setupProjection(2.5f, 1.0f);
			obj->setTexture("textures/brick.jpg");

			std::vector<float> floor_ratios(3);
			floor_ratios[0] = 4.0f / obj->_scope.y;
			floor_ratios[1] = 3.5f / obj->_scope.y;
			floor_ratios[2] = 3.5f / obj->_scope.y;
			std::vector<std::string> floor_names(3);
			floor_names[0] = "Floor";
			floor_names[1] = "Floor";
			floor_names[2] = "Floor";

			std::vector<Object*> floors;
			obj->split(DIRECTION_Y, floor_ratios, floor_names, floors);

			stack.insert(stack.end(), floors.begin(), floors.end());
		} else if (obj->_name == "SideFacade") {
			obj->setupProjection(2.5f, 1.0f);
			obj->setTexture("textures/brick.jpg");

			// 分割比率、名前をセット
			std::vector<float> floor_ratios(3);
			floor_ratios[0] = 4.0f / obj->_scope.y;
			floor_ratios[1] = 3.5f / obj->_scope.y;
			floor_ratios[2] = 3.5f / obj->_scope.y;
			std::vector<std::string> floor_names(3);
			floor_names[0] = "Floor";
			floor_names[1] = "Floor";
			floor_names[2] = "Floor";

			std::vector<Object*> floors;
			obj->split(DIRECTION_Y, floor_ratios, floor_names, floors);

			stack.insert(stack.end(), floors.begin(), floors.end());
		} else if (obj->_name == "Floor") {
			std::vector<float> tile_ratios;
			std::vector<std::string> tile_names;

			// 分割比率、名前をセット
			float margin_ratio = 1.0f / obj->_scope.x;
			int numTiles = (obj->_scope.x - 2.0f) / 3.0f;
			float tile_width_ratio = (obj->_scope.x - 2.0f) / numTiles / obj->_scope.x;

			tile_ratios.push_back(margin_ratio);
			tile_names.push_back("Wall");
			for (int i = 0; i < numTiles; ++i) {
				tile_ratios.push_back(tile_width_ratio);
				tile_names.push_back("Tile");
			}
			tile_ratios.push_back(margin_ratio);
			tile_names.push_back("Wall");

			std::vector<Object*> tiles;
			obj->split(DIRECTION_X, tile_ratios, tile_names, tiles);

			stack.insert(stack.end(), tiles.begin(), tiles.end());
		} else if (obj->_name == "Tile") {
			std::vector<float> wall_ratios(3);
			wall_ratios[0] = 0.1f;
			wall_ratios[1] = 0.8f;
			wall_ratios[2] = 0.1f;
			std::vector<std::string> wall_names(3);
			wall_names[0] = "Wall";
			wall_names[1] = "WallWindowWall";
			wall_names[2] = "Wall";

			std::vector<Object*> walls;
			obj->split(DIRECTION_X, wall_ratios, wall_names, walls);

			stack.insert(stack.end(), walls.begin(), walls.end());
		} else if (obj->_name == "WallWindowWall") {
			std::vector<float> wall_ratios(3);
			wall_ratios[0] = 0.2f;
			wall_ratios[1] = 0.6f;
			wall_ratios[2] = 0.2f;
			std::vector<std::string> wall_names(3);
			wall_names[0] = "Wall";
			wall_names[1] = "Window";
			wall_names[2] = "Wall";

			std::vector<Object*> walls;
			obj->split(DIRECTION_Y, wall_ratios, wall_names, walls);

			stack.insert(stack.end(), walls.begin(), walls.end());
		} else if (obj->_name == "Window") {
			obj->translate(glm::vec3(0, 0, -0.25f));
			obj->setupProjection(obj->_scope.x, obj->_scope.y);
			obj->setTexture("textures/window.jpg");

			Object* obj2 = obj->clone();
			obj2->_name = "Window_1";
			stack.push_back(obj2);
		} else {
			obj->generate(renderManager);
			delete obj;
		}
	}
}

void CGA::generateVase1(RenderManager* renderManager) {
	std::list<Object*> stack;
	
	std::vector<glm::vec2> points;
	points.push_back(glm::vec2(0, 6));
	points.push_back(glm::vec2(38, 12));
	Line* line = new Line("Line", modelMat, points, glm::vec3(1, 1, 1));
	stack.push_back(line);
	
	while (!stack.empty()) {
		Object* obj = stack.front();
		stack.pop_front();

		if (obj->_removed) {
			delete obj;
			continue;
		}
		
		if (obj->_name == "Line") {
			Object* obj2 = obj->clone();
			obj2->_name = "RotatedLine";
			obj2->rotate(0, -90, 0);
			stack.push_back(obj2);
		} else if (obj->_name == "RotatedLine") {
			stack.push_back(obj->revolve("Vase", REVOLVE_X));
		} else {
			obj->generate(renderManager);
			delete obj;
		}
	}
}

void CGA::generateVase2(RenderManager* renderManager) {
	std::list<Object*> stack;
	
	std::vector<glm::vec2> points;
	for (int i = 0; i <= 30; ++i) {
		float y = 6.0f + 6.0f * sinf((float)i / 30 * M_PI * 0.5);
		points.push_back(glm::vec2(i, y));
	}
	points.push_back(glm::vec2(34, 8));
	points.push_back(glm::vec2(38, 12));
	Line* line = new Line("Line", modelMat, points, glm::vec3(1, 1, 1));
	stack.push_back(line);
	
	while (!stack.empty()) {
		Object* obj = stack.front();
		stack.pop_front();

		if (obj->_removed) {
			delete obj;
			continue;
		}
		
		if (obj->_name == "Line") {
			Object* obj2 = obj->clone();
			obj2->_name = "RotatedLine";
			obj2->rotate(0, -90, 0);
			stack.push_back(obj2);
		} else if (obj->_name == "RotatedLine") {
			stack.push_back(obj->revolve("Vase", REVOLVE_X));
		} else {
			obj->generate(renderManager);
			delete obj;
		}
	}
}

void CGA::generateVase3(RenderManager* renderManager) {
	std::list<Object*> stack;
	
	std::vector<glm::vec2> points;
	points.push_back(glm::vec2(0, 8));
	points.push_back(glm::vec2(0.5, 8));
	points.push_back(glm::vec2(0.6, 6));
	points.push_back(glm::vec2(2, 5));
	points.push_back(glm::vec2(2.1, 6));
	points.push_back(glm::vec2(3.0, 6));
	points.push_back(glm::vec2(4.5, 2));
	for (int i = 0; i < 5; ++i) {
		float x = 5.0f + (float)i / 5 * 3;
		float y = 2.0f + sinf((float)i / 5 * M_PI);
		points.push_back(glm::vec2(x, y));
	}

	//points.push_back(glm::vec2(5, 2));
	//points.push_back(glm::vec2(6, 4));
	for (int i = 0; i <= 22; ++i) {
		float y = 1.5f + 10.5f * sinf((float)i / 22 * M_PI * 0.5);
		points.push_back(glm::vec2(8 + i, y));
	}
	points.push_back(glm::vec2(34, 8));
	points.push_back(glm::vec2(38, 12));
	Line* line = new Line("Line", modelMat, points, glm::vec3(1, 1, 1));
	stack.push_back(line);
	
	while (!stack.empty()) {
		Object* obj = stack.front();
		stack.pop_front();

		if (obj->_removed) {
			delete obj;
			continue;
		}
		
		if (obj->_name == "Line") {
			Object* obj2 = obj->clone();
			obj2->_name = "RotatedLine";
			obj2->rotate(0, -90, 0);
			stack.push_back(obj2);
		} else if (obj->_name == "RotatedLine") {
			stack.push_back(obj->revolve("Vase", REVOLVE_X));
		} else {
			obj->generate(renderManager);
			delete obj;
		}
	}
}

void CGA::generateSaltShaker1(RenderManager* renderManager) {
	std::list<Object*> stack;
	
	Rectangle* base = new Rectangle("Base", modelMat, 10, 10, glm::vec3(1, 1, 1));
	stack.push_back(base);
	
	while (!stack.empty()) {
		Object* obj = stack.front();
		stack.pop_front();

		if (obj->_removed) {
			delete obj;
			continue;
		}
		
		if (obj->_name == "Base") {
			stack.push_back(obj->extrude("Body", 20));
		} else {
			obj->generate(renderManager);
			delete obj;
		}
	}
}

void CGA::generateSaltShaker2(RenderManager* renderManager) {
	std::list<Object*> stack;

	Rectangle* base = new Rectangle("Base", modelMat, 10, 10, glm::vec3(1, 1, 1));
	stack.push_back(base);
	
	while (!stack.empty()) {
		Object* obj = stack.front();
		stack.pop_front();

		if (obj->_removed) {
			delete obj;
			continue;
		}
		
		if (obj->_name == "Base") {
			Object* obj2 = obj->taper("Body", 20, 0.7f);
			Rectangle* front;
			std::vector<Rectangle*> sides;
			Polygon* top;
			Polygon* base;
			obj2->componentSplit("front", &front, "side", sides, "top", &top, "base", &base);
			stack.push_back(obj2);
			stack.push_back(top);
		} else if (obj->_name == "top") {
			stack.push_back(obj->offset("CapBase", 0.2f));
		} else if (obj->_name == "CapBase") {
			stack.push_back(obj->extrude("Cap", 6));
		} else {
			obj->generate(renderManager);
			delete obj;
		}
	}
}*/

}
