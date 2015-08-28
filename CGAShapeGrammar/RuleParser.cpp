#include "RuleParser.h"
#include "SetupProjectionRule.h"
#include "ExtrudeRule.h"
#include "ComponentSplitRule.h"
#include "TextureRule.h"
#include "SplitRule.h"
#include "TranslateRule.h"
#include "CGA.h"
#include <iostream>

namespace cga {

std::map<std::string, cga::Rule*> parseRule(char* filename) {
	std::map<std::string, Rule*> rules;

	QFile file(filename);

	QDomDocument doc;
	doc.setContent(&file, true);
	QDomElement root = doc.documentElement();

	QDomNode node = root.firstChild();
	while (!node.isNull()) {
		if (node.toElement().tagName() == "rule") {
			if (!node.toElement().hasAttribute("input_name")) {
				std::cout << "<rule> tag must contain input_name attribute." << std::endl;
				throw "<rule> tag must contain input_name attribute.";
			}
			std::string input_name = node.toElement().attribute("input_name").toUtf8().constData();

			if (!node.toElement().hasAttribute("op")) {
				std::cout << "<rule> tag must contain op attribute." << std::endl;
				throw "<rule> tag must contain op attribute.";
			}
			QString op = node.toElement().attribute("op");

			if (op == "extrude") {
				rules[input_name] = parseExtrudeRule(node);
			} else if (op == "comp") {
				rules[input_name] = parseCompRule(node);
			} else if (op == "setupProjection") {
				rules[input_name] = parseSetupProjectionRule(node);
			} else if (op == "texture") {
				rules[input_name] = parseTextureRule(node);
			} else if (op == "split") {
				rules[input_name] = parseSplitRule(node);
			} else if (op == "translate") {
				rules[input_name] = parseTranslateRule(node);
			}
		}

		node = node.nextSibling();
	}

	return rules;

	/*
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
	*/
}

Rule* parseExtrudeRule(const QDomNode& node) {
	float height;
	std::string output_name;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");

			if (name == "height") {
				height = child.toElement().attribute("value").toFloat();
			} else if (name == "output_name") {
				output_name = child.toElement().attribute("value").toUtf8().constData();
			}
		}

		child = child.nextSibling();
	}

	return new ExtrudeRule(height, output_name);
}

Rule* parseCompRule(const QDomNode& node) {
	std::string front_name;
	std::string side_name;
	std::string top_name;
	std::string bottom_name;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");

			if (name == "front") {
				front_name = child.toElement().attribute("value").toUtf8().constData();
			} else if (name == "side") {
				side_name = child.toElement().attribute("value").toUtf8().constData();
			} else if (name == "top") {
				top_name = child.toElement().attribute("value").toUtf8().constData();
			} else if (name == "bottom") {
				bottom_name = child.toElement().attribute("value").toUtf8().constData();
			}
		}

		child = child.nextSibling();
	}

	return new ComponentSplitRule(front_name, side_name, top_name, bottom_name);
}

Rule* parseSetupProjectionRule(const QDomNode& node) {
	int coordinateType;
	float texWidth;
	float texHeight;
	std::string output_name;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");

			if (name == "coordinateType") {
				if (child.toElement().attribute("value") == "absolute") {
					coordinateType = SetupProjectionRule::TYPE_ABSOLUTE;
				} else {
					coordinateType = SetupProjectionRule::TYPE_RELATIVE;
				}
			} else if (name == "texWidth") {
				texWidth = child.toElement().attribute("value").toFloat();
			} else if (name == "texHeight") {
				texHeight = child.toElement().attribute("value").toFloat();
			} else if (name == "output_name") {
				output_name = child.toElement().attribute("value").toUtf8().constData();
			}
		}

		child = child.nextSibling();
	}

	return new SetupProjectionRule(coordinateType, texWidth, texHeight, output_name);
}

Rule* parseTextureRule(const QDomNode& node) {
	std::string texture;
	std::string output_name;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");

			if (name == "texture") {
				texture = child.toElement().attribute("value").toUtf8().constData();
			} else if (name == "output_name") {
				output_name = child.toElement().attribute("value").toUtf8().constData();
			}
		}

		child = child.nextSibling();
	}

	return new TextureRule(texture, output_name);
}

Rule* parseSplitRule(const QDomNode& node) {
	int direction;
	std::vector<Value*> sizes;
	std::vector<std::string> names;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");

			if (name == "direction") {
				if (child.toElement().attribute("value") == "y") {
					direction = DIRECTION_Y;
				} else {
					direction = DIRECTION_X;
				}
			} else if (name == "size") {
				QDomNode element = child.firstChild();
				while (!element.isNull()) {
					if (element.toElement().tagName() == "element") {
						QString type = element.toElement().attribute("type");
						float value = element.toElement().attribute("value").toFloat();
						bool repeat = false;
						if (element.toElement().hasAttribute("repeat")) {
							repeat = true;
						}

						if (repeat) {
							if (type == "absolute") {
								sizes.push_back(new ValueSet(new SingleValue(Value::TYPE_ABSOLUTE, value), true));
							} else if (type == "relative") {
								sizes.push_back(new ValueSet(new SingleValue(Value::TYPE_RELATIVE, value), true));
							} else {
								sizes.push_back(new ValueSet(new SingleValue(Value::TYPE_FLOATING, value), true));
							}
						} else {
							if (type == "absolute") {
								sizes.push_back(new SingleValue(Value::TYPE_ABSOLUTE, value));
							} else if (type == "relative") {
								sizes.push_back(new SingleValue(Value::TYPE_RELATIVE, value));
							} else {
								sizes.push_back(new SingleValue(Value::TYPE_FLOATING, value));
							}
						}
					}

					element = element.nextSibling();
				}
			} else if (name == "name") {
				QDomNode element = child.firstChild();
				while (!element.isNull()) {
					if (element.toElement().tagName() == "element") {
						names.push_back(element.toElement().attribute("value").toUtf8().constData());
					}

					element = element.nextSibling();
				}
			}
		}

		child = child.nextSibling();
	}

	return new SplitRule(direction, sizes, names);
}

Rule* parseTranslateRule(const QDomNode& node) {
	float x, y, z;
	std::string output_name;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");

			if (name == "x") {
				x = child.toElement().attribute("value").toFloat();
			} else if (name == "y") {
				y = child.toElement().attribute("value").toFloat();
			} else if (name == "z") {
				z = child.toElement().attribute("value").toFloat();
			} else if (name == "output_name") {
				output_name = child.toElement().attribute("value").toUtf8().constData();
			}
		}

		child = child.nextSibling();
	}

	return new TranslateRule(glm::vec3(x, y, z), output_name);
}

}
