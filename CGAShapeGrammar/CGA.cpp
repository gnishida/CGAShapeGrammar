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

Object* Object::clone() {
	throw "setupProjection() is not supported.";
}

void Object::setupProjection(float texWidth, float texHeight) {
	throw "setupProjection() is not supported.";
}

void Object::setupProjection(float u1, float v1, float u2, float v2) {
	throw "setupProjection() is not supported.";
}

Object* Object::extrude(const std::string& name, float height) {
	throw "extrude() is not supported.";
}

void Object::split(int direction, const std::vector<float> sizes, const std::vector<std::string> names, std::vector<Rectangle*>& rectangles) {
	throw "split() is not supported.";
}

void Object::componentSplit(const std::string& front_name, Rectangle** front, const std::string& sides_name, std::vector<Rectangle*>& sides, const std::string& top_name, Polygon** top, const std::string& base_name, Polygon** base) {
	throw "componentSplit() is not supported.";
}

void Object::generate(RenderManager* renderManager) {
	throw "generate() is not supported.";
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

Object* PrismObject::clone() {
	return new PrismObject(*this);
}

void PrismObject::setupProjection(float texWidth, float texHeight) {
}

void PrismObject::split(int direction, const std::vector<float> sizes, const std::vector<std::string> names, std::vector<Rectangle*>& rectangles) {
	throw "PrismObject does not support split operation.";
}


void PrismObject::componentSplit(const std::string& front_name, Rectangle** front, const std::string& sides_name, std::vector<Rectangle*>& sides, const std::string& top_name, Polygon** top, const std::string& base_name, Polygon** base) {
	// front face
	{
		*front = new Rectangle(front_name, glm::rotate(modelMat, M_PI * 0.5f, glm::vec3(1, 0, 0)), glm::length(points[1] - points[0]), height, color, texture);
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

			sides[i - 1] = new Rectangle(sides_name, modelMat * mat2, glm::length(points[(i + 1) % points.size()] - points[i]), height, color, texture);
		}
	}

	// top face
	{
		*top = new Polygon(top_name, glm::translate(modelMat, glm::vec3(0, 0, height)), points, color, texture);
	}

	// bottom face
	{
		std::vector<glm::vec2> basePoints = points;
		std::reverse(basePoints.begin(), basePoints.end());
		*base = new Polygon(base_name, modelMat, basePoints, color, texture);
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

	this->textureEnabled = false;
}

void Rectangle::setupProjection(float texWidth, float texHeight) {
	setupProjection(0, 0, width / texWidth, height / texHeight);
}

Object* Rectangle::clone() {
	return new Rectangle(*this);
}

void Rectangle::setupProjection(float u1, float v1, float u2, float v2) {
	textureEnabled = true;

	texCoords.resize(4);
	texCoords[0] = glm::vec2(u1, v1);
	texCoords[1] = glm::vec2(u2, v1);
	texCoords[2] = glm::vec2(u2, v2);
	texCoords[3] = glm::vec2(u1, v2);
}

Object* Rectangle::extrude(const std::string& name, float height) {
	std::vector<glm::vec2> points(4);
	points[0] = glm::vec2(0, 0);
	points[1] = glm::vec2(width, 0);
	points[2] = glm::vec2(width, height);
	points[3] = glm::vec2(0, height);
	return new PrismObject(name, modelMat, points, height, color);
}

void Rectangle::split(int direction, const std::vector<float> sizes, const std::vector<std::string> names, std::vector<Rectangle*>& rectangles) {
	rectangles.resize(sizes.size());

	float offset = 0.0f;
	
	for (int i = 0; i < sizes.size(); ++i) {
		glm::mat4 mat;
		if (direction == DIRECTION_X) {
			mat = glm::translate(glm::mat4(), glm::vec3(offset, 0, 0));
			rectangles[i] = new Rectangle(names[i], modelMat * mat, sizes[i], height, color, texture);
			if (textureEnabled) {
				rectangles[i]->setupProjection(
					texCoords[0].x + (texCoords[1].x - texCoords[0].x) * offset / width, texCoords[0].y,
					texCoords[0].x + (texCoords[1].x - texCoords[0].x) * (offset + sizes[i]) / width, texCoords[2].y);
			}
			offset += sizes[i];
		} else {
			mat = glm::translate(glm::mat4(), glm::vec3(0, offset, 0));
			rectangles[i] = new Rectangle(names[i], modelMat * mat, width, sizes[i], color, texture);
			if (textureEnabled) {
				rectangles[i]->setupProjection(
					texCoords[0].x, texCoords[0].y + (texCoords[2].y - texCoords[0].y) * offset / height,
					texCoords[1].x, texCoords[0].y + (texCoords[2].y - texCoords[0].y) * (offset + sizes[i]) / height);
			}
			offset += sizes[i];
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

	if (textureEnabled) {
		vertices[0] = Vertex(glm::vec3(p1), glm::vec3(normal), color, glm::vec3(texCoords[0], 0));
		vertices[1] = Vertex(glm::vec3(p2), glm::vec3(normal), color, glm::vec3(texCoords[1], 0));
		vertices[2] = Vertex(glm::vec3(p3), glm::vec3(normal), color, glm::vec3(texCoords[2], 0));

		vertices[3] = Vertex(glm::vec3(p1), glm::vec3(normal), color, glm::vec3(texCoords[0], 0));
		vertices[4] = Vertex(glm::vec3(p3), glm::vec3(normal), color, glm::vec3(texCoords[2], 0));
		vertices[5] = Vertex(glm::vec3(p4), glm::vec3(normal), color, glm::vec3(texCoords[3], 0));

		renderManager->addObject(name.c_str(), texture.c_str(), vertices);
	} else {
		vertices[0] = Vertex(glm::vec3(p1), glm::vec3(normal), color);
		vertices[1] = Vertex(glm::vec3(p2), glm::vec3(normal), color);
		vertices[2] = Vertex(glm::vec3(p3), glm::vec3(normal), color);

		vertices[3] = Vertex(glm::vec3(p1), glm::vec3(normal), color);
		vertices[4] = Vertex(glm::vec3(p3), glm::vec3(normal), color);
		vertices[5] = Vertex(glm::vec3(p4), glm::vec3(normal), color);

		renderManager->addObject(name.c_str(), "", vertices);
	}
	
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

Object* Polygon::clone() {
	return new Polygon(*this);
}

void Polygon::setupProjection(float texWidth, float texHeight) {
	texCoords.resize(points.size());
	for (int i = 0; i < points.size(); ++i) {
		texCoords[i] = glm::vec2(points[i].x / texWidth, points[i].y / texHeight);
	}
}

Object* Polygon::extrude(const std::string& name, float height) {
	return new PrismObject(name, modelMat, points, height, color);
}

void Polygon::split(int direction, const std::vector<float> sizes, const std::vector<std::string> names, std::vector<Rectangle*>& rectangles) {
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

ValueSet::ValueSet(Value* value, bool repeat) {
	this->type = TYPE_SET;
	values.push_back(value);
	this->repeat = repeat;
}

ValueSet::ValueSet(const std::vector<Value*>& values, bool repeat) {
	this->type = TYPE_SET;
	this->values = values;
	this->repeat = repeat;
}

float ValueSet::getEstimateValue(float size) {
	float sum = 0.0f;
	for (int i = 0; i < values.size(); ++i) {
		if (values[i]->type == Value::TYPE_ABSOLUTE) {
			sum += values[i]->value;
		} else if (values[i]->type == Value::TYPE_RELATIVE) {
			sum += size * values[i]->value;
		} else if (values[i]->type == Value::TYPE_FLOATING) {
			sum += values[i]->value;
		}
	}

	return sum;
}


void Rule::decodeSplitSizes(float size, const std::vector<Value*>& sizes, const std::vector<std::string>& output_names, std::vector<float>& decoded_sizes, std::vector<std::string>& decoded_output_names) {
	int regular_count = 0;
	int floating_count = 0;
	float regular_sum = 0.0f;
	float floating_sum = 0.0f;
	bool repeat = false;

	for (int i = 0; i < sizes.size(); ++i) {
		if (sizes[i]->type == Value::TYPE_ABSOLUTE) {
			regular_count++;
			regular_sum += sizes[i]->value;
		} else if (sizes[i]->type == Value::TYPE_RELATIVE) {
			regular_count++;
			regular_sum += size * sizes[i]->value * size;
		} else if (sizes[i]->type == Value::TYPE_FLOATING) {
			floating_count++;
			floating_sum += sizes[i]->value;
		}
	}

	float scale;
	if (floating_count > 0) {
		scale = (size - regular_sum) / floating_sum;
	}

	for (int i = 0; i < sizes.size(); ++i) {
		if (sizes[i]->type == Value::TYPE_ABSOLUTE) {
			decoded_sizes.push_back(sizes[i]->value);
			decoded_output_names.push_back(output_names[i]);
		} else if (sizes[i]->type == Value::TYPE_RELATIVE) {
			decoded_sizes.push_back(sizes[i]->value * size);
			decoded_output_names.push_back(output_names[i]);
		} else if (sizes[i]->type == Value::TYPE_FLOATING) {
			decoded_sizes.push_back(sizes[i]->value * scale);
			decoded_output_names.push_back(output_names[i]);
		} else if (sizes[i]->type == Value::TYPE_SET) {
			if (sizes[i]->repeat) {
				/*std::vector<Value*> temp_ratios(1);
				temp_ratios[0] = ratios[i];*/
				std::vector<std::string> temp_names(sizes[i]->values.size());
				for (int k = 0; k < temp_names.size(); ++k) {
					temp_names[k] = output_names[i];
				}

				float s = sizes[i]->getEstimateValue(size - regular_sum);
				int num = (size - regular_sum) / s;
				for (int k = 0; k < num; ++k) {
					decodeSplitSizes((size - regular_sum) / num, sizes[i]->values, temp_names, decoded_sizes, decoded_output_names);
				}
			} else {
				std::vector<Value*> temp_ratios(1);
				temp_ratios[0] = sizes[i];
				std::vector<std::string> temp_names(1);
				temp_names[0] = output_names[i];
				decodeSplitSizes(size - regular_sum, temp_ratios, temp_names, decoded_sizes, decoded_output_names);
			}
		}
	}
}

ExtrudeRule::ExtrudeRule(float height, const std::string& output_name) {
	this->height = height;
	this->output_name = output_name;
}

void ExtrudeRule::apply(Object* obj, std::list<Object*>& stack) {
	stack.push_back(obj->extrude(output_name, height));
}

ComponentSplitRule::ComponentSplitRule(const std::string& front_name, const std::string& side_name, const std::string& top_name, const std::string& bottom_name) {
	this->front_name = front_name;
	this->side_name = side_name;
	this->top_name = top_name;
	this->bottom_name = bottom_name;
}

void ComponentSplitRule::apply(Object* obj, std::list<Object*>& stack) {
	Rectangle* front;
	std::vector<Rectangle*> sides;
	Polygon* top;
	Polygon* bottom;
	
	obj->componentSplit(front_name, &front, side_name, sides, top_name, &top, bottom_name, &bottom);
	stack.push_back(front);
	stack.insert(stack.end(), sides.begin(), sides.end());
	stack.push_back(top);
	stack.push_back(bottom);
}

SplitRule::SplitRule(int direction, const std::vector<Value*>& sizes, const std::vector<std::string>& output_names) {
	this->direction = direction;
	this->sizes = sizes;
	this->output_names = output_names;
}

void SplitRule::apply(Object* obj, std::list<Object*>& stack) {
	std::vector<Rectangle*> floors;

	std::vector<float> decoded_sizes;
	std::vector<std::string> decoded_output_names;
	if (direction == DIRECTION_X) {
		decodeSplitSizes(obj->scope.x, sizes, output_names, decoded_sizes, decoded_output_names);
	} else {
		decodeSplitSizes(obj->scope.y, sizes, output_names, decoded_sizes, decoded_output_names);
	}

	obj->split(direction, decoded_sizes, decoded_output_names, floors);
	stack.insert(stack.end(), floors.begin(), floors.end());
}

SetupProjectionRule::SetupProjectionRule(int type, float texWidth, float texHeight, const std::string& output_name) {
	this->type = type;
	this->texWidth = texWidth;
	this->texHeight = texHeight;
	this->output_name = output_name;
}

/*SetupProjectionRule::SetupProjectionRule(float u1, float v1, float u2, float v2, const std::string& output_name) {
	this->u1 = u1;
	this->v1 = v1;
	this->u2 = u2;
	this->v2 = v2;
	this->output_name = output_name;
}*/

void SetupProjectionRule::apply(Object* obj, std::list<Object*>& stack) {
	Object *obj2 = obj->clone();
	obj2->name = output_name;

	if (type == TYPE_RELATIVE) {
		obj2->setupProjection(obj->scope.x, obj->scope.y);
	} else {
		obj2->setupProjection(texWidth, texHeight);
	}
	stack.push_back(obj2);
}

SetTextureRule::SetTextureRule(const std::string& texture, const std::string& output_name) {
	this->texture = texture;
	this->output_name = output_name;
}

void SetTextureRule::apply(Object* obj, std::list<Object*>& stack) {
	Object* obj2 = obj->clone();
	obj2->name = output_name;
	
	obj2->setTexture(texture);
	stack.push_back(obj2);
}

TranslateRule::TranslateRule(const glm::vec3& vec, const std::string& output_name) {
	this->vec = vec;
	this->output_name = output_name;
}

void TranslateRule::apply(Object* obj, std::list<Object*>& stack) {
	Object* obj2 = obj->clone();
	obj2->name = output_name;

	obj2->translate(vec);
	stack.push_back(obj2);
}

CGA::CGA() {
	rules = buildingRule();
}

void CGA::generate(RenderManager* renderManager) {
	std::list<Object*> stack;
	Rectangle* lot = new Rectangle("Lot", glm::rotate(glm::mat4(), -M_PI * 0.5f, glm::vec3(1, 0, 0)), 35, 10, glm::vec3(1, 1, 1), "");
	stack.push_back(lot);

	while (!stack.empty()) {
		Object* obj = stack.front();
		stack.pop_front();

		if (rules.find(obj->name) == rules.end()) {
			obj->generate(renderManager);
		} else {
			rules[obj->name]->apply(obj, stack);
		}

		delete obj;
	}
}

std::map<std::string, Rule*> CGA::buildingRule() {
	std::map<std::string, Rule*> rules;

	rules["Lot"] = new ExtrudeRule(11, "Building");
	rules["Building"] = new ComponentSplitRule("FrontFacade", "SideFacade", "Roof", "Base");
	rules["FrontFacade"] = new SetupProjectionRule(SetupProjectionRule::TYPE_ABSOLUTE, 2.5f, 1.0f, "FrontFacade_1");
	rules["FrontFacade_1"] = new SetTextureRule("textures/brick.jpg", "FrontFacade_2");
	rules["SideFacade"] = new SetupProjectionRule(SetupProjectionRule::TYPE_ABSOLUTE, 2.5f, 1.0f, "SideFacade_1");
	rules["SideFacade_1"] = new SetTextureRule("textures/brick.jpg", "SideFacade_2");
	{
		std::vector<Value*> floor_sizes(2);
		floor_sizes[0] = new SingleValue(Value::TYPE_ABSOLUTE, 4.0f);
		floor_sizes[1] = new ValueSet(new SingleValue(Value::TYPE_FLOATING, 3.5f), true);

		std::vector<std::string> floor_names(2);
		floor_names[0] = "Floor";
		floor_names[1] = "Floor";

		rules["FrontFacade_2"] = new SplitRule(DIRECTION_Y, floor_sizes, floor_names);
		rules["SideFacade_2"] = new SplitRule(DIRECTION_Y, floor_sizes, floor_names);
	}
	{
		std::vector<Value*> tile_sizes(3);
		tile_sizes[0] = new SingleValue(Value::TYPE_ABSOLUTE, 1.0f);
		tile_sizes[1] = new ValueSet(new SingleValue(Value::TYPE_FLOATING, 3.0f), true);
		tile_sizes[2] = new SingleValue(Value::TYPE_ABSOLUTE, 1.0f);

		std::vector<std::string> tile_names(3);
		tile_names[0] = "Wall";
		tile_names[1] = "Tile";
		tile_names[2] = "Wall";

		rules["Floor"] = new SplitRule(DIRECTION_X, tile_sizes, tile_names);
	}
	rules["Roof"] = new SetupProjectionRule(SetupProjectionRule::TYPE_RELATIVE, SCOPE_SX, SCOPE_SY, "Roof_1");
	rules["Roof_1"] = new SetTextureRule("textures/roof.jpg", "Roof_2");
	{
		std::vector<Value*> wall_sizes(3);
		wall_sizes[0] = new SingleValue(Value::TYPE_FLOATING, 1.0f);
		wall_sizes[1] = new SingleValue(Value::TYPE_ABSOLUTE, 2.0f);
		wall_sizes[2] = new SingleValue(Value::TYPE_FLOATING, 1.0f);
		std::vector<std::string> wall_names(3);
		wall_names[0] = "Wall";
		wall_names[1] = "WallWindowWall";
		wall_names[2] = "Wall";

		rules["Tile"] = new SplitRule(DIRECTION_X, wall_sizes, wall_names);
	}
	{
		std::vector<Value*> wall_sizes(3);
		wall_sizes[0] = new SingleValue(Value::TYPE_ABSOLUTE, 1.0f);
		wall_sizes[1] = new SingleValue(Value::TYPE_ABSOLUTE, 1.5f);
		wall_sizes[2] = new SingleValue(Value::TYPE_FLOATING, 1.0f);
		std::vector<std::string> wall_names(3);
		wall_names[0] = "Wall";
		wall_names[1] = "Window";
		wall_names[2] = "Wall";

		rules["WallWindowWall"] = new SplitRule(DIRECTION_Y, wall_sizes, wall_names);
	}
	rules["Window"] = new TranslateRule(glm::vec3(0, 0, -0.25f), "Window_1");
	rules["Window_1"] = new SetupProjectionRule(SetupProjectionRule::TYPE_RELATIVE, 0, 0, "Window_2");
	rules["Window_2"] = new SetTextureRule("textures/window.jpg", "Window_3");

	return rules;
}

}
