#include "RuleParser.h"
#include "CompOperator.h"
#include "CopyOperator.h"
#include "ExtrudeOperator.h"
#include "OffsetOperator.h"
#include "RoofHipOperator.h"
#include "RotateOperator.h"
#include "SetupProjectionOperator.h"
#include "ShapeLOperator.h"
#include "SplitOperator.h"
#include "TaperOperator.h"
#include "TextureOperator.h"
#include "TranslateOperator.h"
#include "CGA.h"
#include <iostream>

namespace cga {

std::map<std::string, Rule> parseRule(char* filename) {
	std::map<std::string, Rule> rules;

	QFile file(filename);

	QDomDocument doc;
	doc.setContent(&file, true);
	QDomElement root = doc.documentElement();

	QDomNode rule_node = root.firstChild();
	while (!rule_node.isNull()) {
		if (rule_node.toElement().tagName() == "rule") {
			if (!rule_node.toElement().hasAttribute("name")) {
				std::cout << "<rule> tag must contain name attribute." << std::endl;
				throw "<rule> tag must contain name attribute.";
			}
			std::string name = rule_node.toElement().attribute("name").toUtf8().constData();

			QDomNode operator_node = rule_node.firstChild();
			while (!operator_node.isNull()) {
				if (operator_node.toElement().tagName() == "operator") {
					if (!operator_node.toElement().hasAttribute("name")) {
						std::cout << "<operator> tag must contain name attribute." << std::endl;
						throw "<operator> tag must contain name attribute.";
					}
					std::string operator_name = operator_node.toElement().attribute("name").toUtf8().constData();

					if (operator_name == "copy") {
						rules[name].operators.push_back(parseCopyOperator(operator_node));
					} else if (operator_name == "comp") {
						rules[name].operators.push_back(parseCompOperator(operator_node));
					} else if (operator_name == "extrude") {
						rules[name].operators.push_back(parseExtrudeOperator(operator_node));
					} else if (operator_name == "offset") {
						rules[name].operators.push_back(parseOffsetOperator(operator_node));
					} else if (operator_name == "roofHip") {
						rules[name].operators.push_back(parseRoofHipOperator(operator_node));
					} else if (operator_name == "setupProjection") {
						rules[name].operators.push_back(parseSetupProjectionOperator(operator_node));
					} else if (operator_name == "shapeL") {
						rules[name].operators.push_back(parseShapeLOperator(operator_node));
					} else if (operator_name == "split") {
						rules[name].operators.push_back(parseSplitOperator(operator_node));
					} else if (operator_name == "taper") {
						rules[name].operators.push_back(parseTaperOperator(operator_node));
					} else if (operator_name == "texture") {
						rules[name].operators.push_back(parseTextureOperator(operator_node));
					} else if (operator_name == "translate") {
						rules[name].operators.push_back(parseTranslateOperator(operator_node));
					}

				} else if (operator_node.toElement().tagName() == "output") {
					if (!operator_node.toElement().hasAttribute("name")) {
						std::cout << "<output> tag must contain name attribute." << std::endl;
						throw "<output> tag must contain name attribute.";
					}
					rules[name].output_name = operator_node.toElement().attribute("name").toUtf8().constData();
				}

				operator_node = operator_node.nextSibling();
			}
		}

		rule_node = rule_node.nextSibling();
	}

	return rules;
}

Operator* parseCompOperator(const QDomNode& node) {
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

	return new CompOperator(front_name, side_name, top_name, bottom_name);
}

Operator* parseCopyOperator(const QDomNode& node) {
	std::string copy_name;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");

			if (name == "copy_name") {
				copy_name = child.toElement().attribute("value").toUtf8().constData();
			}
		}

		child = child.nextSibling();
	}

	return new CopyOperator(copy_name);
}

Operator* parseExtrudeOperator(const QDomNode& node) {
	float height;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");

			if (name == "height") {
				height = child.toElement().attribute("value").toFloat();
			}
		}

		child = child.nextSibling();
	}

	return new ExtrudeOperator(height);
}

Operator* parseOffsetOperator(const QDomNode& node) {
	float offsetDistance;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");

			if (name == "offsetDistance") {
				offsetDistance = child.toElement().attribute("value").toFloat();
			}
		}

		child = child.nextSibling();
	}

	return new OffsetOperator(offsetDistance);
}

Operator* parseRoofHipOperator(const QDomNode& node) {
	float angle = 0.0f;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");

			if (name == "angle") {
				angle = child.toElement().attribute("value").toFloat();
			}
		}

		child = child.nextSibling();
	}

	return new RoofHipOperator(angle);
}

Operator* parseRotateOperator(const QDomNode& node) {
	float xAngle = 0.0f;
	float yAngle = 0.0f;
	float zAngle = 0.0f;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");

			if (name == "xAngle") {
				xAngle = child.toElement().attribute("value").toFloat();
			} else if (name == "yAngle") {
				yAngle = child.toElement().attribute("value").toFloat();
			} else if (name == "zAngle") {
				zAngle = child.toElement().attribute("value").toFloat();
			}
		}

		child = child.nextSibling();
	}

	return new RotateOperator(xAngle, yAngle, zAngle);
}

Operator* parseSetupProjectionOperator(const QDomNode& node) {
	int coordinateType;
	float texWidth;
	float texHeight;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");

			if (name == "coordinateType") {
				if (child.toElement().attribute("value") == "absolute") {
					coordinateType = SetupProjectionOperator::TYPE_ABSOLUTE;
				} else {
					coordinateType = SetupProjectionOperator::TYPE_RELATIVE;
				}
			} else if (name == "texWidth") {
				texWidth = child.toElement().attribute("value").toFloat();
			} else if (name == "texHeight") {
				texHeight = child.toElement().attribute("value").toFloat();
			}
		}

		child = child.nextSibling();
	}

	return new SetupProjectionOperator(coordinateType, texWidth, texHeight);
}

Operator* parseShapeLOperator(const QDomNode& node) {
	float frontWidth;
	float leftWidth;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");

			if (name == "frontWidth") {
				frontWidth = child.toElement().attribute("value").toFloat();
			} else if (name == "leftWidth") {
				leftWidth = child.toElement().attribute("value").toFloat();
			}
		}

		child = child.nextSibling();
	}

	return new ShapeLOperator(frontWidth, leftWidth);
}

Operator* parseSplitOperator(const QDomNode& node) {
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

	return new SplitOperator(direction, sizes, names);
}

Operator* parseTaperOperator(const QDomNode& node) {
	float height;
	float top_ratio = 0.0f;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");

			if (name == "height") {
				height = child.toElement().attribute("value").toFloat();
			} else if (name == "top_ratio") {
				top_ratio = child.toElement().attribute("value").toFloat();
			}
		}

		child = child.nextSibling();
	}

	return new TaperOperator(height, top_ratio);
}

Operator* parseTextureOperator(const QDomNode& node) {
	std::string texture;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");

			if (name == "texture") {
				texture = child.toElement().attribute("value").toUtf8().constData();
			}
		}

		child = child.nextSibling();
	}

	return new TextureOperator(texture);
}

Operator* parseTranslateOperator(const QDomNode& node) {
	float x, y, z;

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
			}
		}

		child = child.nextSibling();
	}

	return new TranslateOperator(glm::vec3(x, y, z));
}

}
