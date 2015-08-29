#include "RuleParser.h"
#include "CompRule.h"
#include "CopyRule.h"
#include "ExtrudeRule.h"
#include "OffsetRule.h"
#include "RoofHipRule.h"
#include "SetupProjectionRule.h"
#include "SplitRule.h"
#include "TaperRule.h"
#include "TextureRule.h"
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

			if (op == "copy") {
				rules[input_name] = parseCopyRule(node);
			} else if (op == "extrude") {
				rules[input_name] = parseExtrudeRule(node);
			} else if (op == "taper") {
				rules[input_name] = parseTaperRule(node);
			} else if (op == "offset") {
				rules[input_name] = parseOffsetRule(node);
			} else if (op == "comp") {
				rules[input_name] = parseCompRule(node);
			} else if (op == "roofHip") {
				rules[input_name] = parseRoofHipRule(node);
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

	return new CompRule(front_name, side_name, top_name, bottom_name);
}

Rule* parseCopyRule(const QDomNode& node) {
	std::string copy_name;
	std::string output_name;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");

			if (name == "copy_name") {
				copy_name = child.toElement().attribute("value").toUtf8().constData();
			} else if (name == "output_name") {
				output_name = child.toElement().attribute("value").toUtf8().constData();
			}
		}

		child = child.nextSibling();
	}

	return new CopyRule(copy_name, output_name);
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

Rule* parseOffsetRule(const QDomNode& node) {
	float offsetDistance;
	std::string output_name;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");

			if (name == "offsetDistance") {
				offsetDistance = child.toElement().attribute("value").toFloat();
			} else if (name == "output_name") {
				output_name = child.toElement().attribute("value").toUtf8().constData();
			}
		}

		child = child.nextSibling();
	}

	return new OffsetRule(offsetDistance, output_name);
}

Rule* parseRoofHipRule(const QDomNode& node) {
	float angle = 0.0f;
	std::string output_name;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");

			if (name == "angle") {
				angle = child.toElement().attribute("value").toFloat();
			} else if (name == "output_name") {
				output_name = child.toElement().attribute("value").toUtf8().constData();
			}
		}

		child = child.nextSibling();
	}

	return new RoofHipRule(angle, output_name);
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

Rule* parseTaperRule(const QDomNode& node) {
	float height;
	float top_ratio = 0.0f;
	std::string output_name;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");

			if (name == "height") {
				height = child.toElement().attribute("value").toFloat();
			} else if (name == "top_ratio") {
				top_ratio = child.toElement().attribute("value").toFloat();
			} else if (name == "output_name") {
				output_name = child.toElement().attribute("value").toUtf8().constData();
			}
		}

		child = child.nextSibling();
	}

	return new TaperRule(height, top_ratio, output_name);
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
