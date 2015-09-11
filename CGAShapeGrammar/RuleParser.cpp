#include "RuleParser.h"
#include "ColorOperator.h"
#include "CompOperator.h"
#include "CopyOperator.h"
#include "ExtrudeOperator.h"
#include "InsertOperator.h"
#include "OffsetOperator.h"
#include "RoofHipOperator.h"
#include "RotateOperator.h"
#include "SetupProjectionOperator.h"
#include "ShapeLOperator.h"
#include "SizeOperator.h"
#include "SplitOperator.h"
#include "TaperOperator.h"
#include "TextureOperator.h"
#include "TranslateOperator.h"
#include "CGA.h"
#include <iostream>

namespace cga {

void parseRule(char* filename, RuleSet& ruleSet) {
	QFile file(filename);

	QDomDocument doc;
	doc.setContent(&file, true);
	QDomElement root = doc.documentElement();

	QDomNode child_node = root.firstChild();
	while (!child_node.isNull()) {
		if (child_node.toElement().tagName() == "attr") {
			if (!child_node.toElement().hasAttribute("name")) {
				std::cout << "<attr> tag must contain name attribute." << std::endl;
				throw "<attr> tag must contain name attribute.";
			}
			std::string name = child_node.toElement().attribute("name").toUtf8().constData();

			if (!child_node.toElement().hasAttribute("value")) {
				std::cout << "<attr> tag must contain value attribute." << std::endl;
				throw "<attr> tag must contain value attribute.";
			}
			std::string value = child_node.toElement().attribute("value").toUtf8().constData();

			ruleSet.addAttr(name, value);
		} else if (child_node.toElement().tagName() == "rule") {
			if (!child_node.toElement().hasAttribute("name")) {
				std::cout << "<rule> tag must contain name attribute." << std::endl;
				throw "<rule> tag must contain name attribute.";
			}
			std::string name = child_node.toElement().attribute("name").toUtf8().constData();

			ruleSet.addRule(name);

			QDomNode operator_node = child_node.firstChild();
			while (!operator_node.isNull()) {
				if (operator_node.toElement().tagName() == "operator") {
					if (!operator_node.toElement().hasAttribute("name")) {
						std::cout << "<operator> tag must contain name attribute." << std::endl;
						throw "<operator> tag must contain name attribute.";
					}
					std::string operator_name = operator_node.toElement().attribute("name").toUtf8().constData();

					if (operator_name == "color") {
						ruleSet.addOperator(name, parseColorOperator(operator_node));
					} else if (operator_name == "comp") {
						ruleSet.addOperator(name, parseCompOperator(operator_node));
					} else if (operator_name == "copy") {
						ruleSet.addOperator(name, parseCopyOperator(operator_node));
					} else if (operator_name == "extrude") {
						ruleSet.addOperator(name, parseExtrudeOperator(operator_node));
					} else if (operator_name == "insert") {
						ruleSet.addOperator(name, parseInsertOperator(operator_node));
					} else if (operator_name == "offset") {
						ruleSet.addOperator(name, parseOffsetOperator(operator_node));
					} else if (operator_name == "roofHip") {
						ruleSet.addOperator(name, parseRoofHipOperator(operator_node));
					} else if (operator_name == "rotate") {
						ruleSet.addOperator(name, parseRotateOperator(operator_node));
					} else if (operator_name == "setupProjection") {
						ruleSet.addOperator(name, parseSetupProjectionOperator(operator_node));
					} else if (operator_name == "shapeL") {
						ruleSet.addOperator(name, parseShapeLOperator(operator_node));
					} else if (operator_name == "size") {
						ruleSet.addOperator(name, parseSizeOperator(operator_node));
					} else if (operator_name == "split") {
						ruleSet.addOperator(name, parseSplitOperator(operator_node));
					} else if (operator_name == "taper") {
						ruleSet.addOperator(name, parseTaperOperator(operator_node));
					} else if (operator_name == "texture") {
						ruleSet.addOperator(name, parseTextureOperator(operator_node));
					} else if (operator_name == "translate") {
						ruleSet.addOperator(name, parseTranslateOperator(operator_node));
					}
				}

				operator_node = operator_node.nextSibling();
			}
		}

		child_node = child_node.nextSibling();
	}
}

Operator* parseColorOperator(const QDomNode& node) {
	std::vector<std::string> color;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");

			if (name == "r") {
				color.push_back(child.toElement().attribute("value").toUtf8().constData());
			} else if (name == "g") {
				color.push_back(child.toElement().attribute("value").toUtf8().constData());
			} else if (name == "b") {
				color.push_back(child.toElement().attribute("value").toUtf8().constData());
			} else if (name == "s") {
				color.push_back(child.toElement().attribute("value").toUtf8().constData());
			}
		}

		child = child.nextSibling();
	}

	return new ColorOperator(color);
}

Operator* parseCompOperator(const QDomNode& node) {
	std::string front_name;
	std::string side_name;
	std::string top_name;
	std::string bottom_name;
	std::string inside_name;
	std::string border_name;
	std::map<std::string, std::string> name_map;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");
			std::string value = child.toElement().attribute("value").toUtf8().constData();

			if (name == "front") {
				name_map["front"] = value;
			} else if (name == "side") {
				name_map["side"] = value;
			} else if (name == "top") {
				name_map["top"] = value;
			} else if (name == "bottom") {
				name_map["bottom"] = value;
			} else if (name == "inside") {
				name_map["inside"] = value;
			} else if (name == "border") {
				name_map["border"] = value;
			}
		}

		child = child.nextSibling();
	}

	return new CompOperator(name_map);
}

Operator* parseCopyOperator(const QDomNode& node) {
	std::string copy_name;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			copy_name = child.toElement().attribute("value").toUtf8().constData();
		}

		child = child.nextSibling();
	}

	return new CopyOperator(copy_name);
}

Operator* parseExtrudeOperator(const QDomNode& node) {
	//float height;
	std::string height;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");

			if (name == "height") {
				//height = child.toElement().attribute("value").toFloat();
				height = child.toElement().attribute("value").toUtf8().constData();
			}
		}

		child = child.nextSibling();
	}

	return new ExtrudeOperator(height);
}

Operator* parseInsertOperator(const QDomNode& node) {
	std::string geometryPath;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");

			if (name == "geometryPath") {
				geometryPath = child.toElement().attribute("value").toUtf8().constData();
			}
		}

		child = child.nextSibling();
	}

	return new InsertOperator(geometryPath);
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

Operator* parseSizeOperator(const QDomNode& node) {
	SingleValue xSize;
	SingleValue ySize;
	SingleValue zSize;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");
			QString type;
			if (child.toElement().hasAttribute("type")) {
				type = child.toElement().attribute("type");
			}
			std::string value = child.toElement().attribute("value").toUtf8().constData();

			if (name == "xSize") {
				if (type == "relative") {
					xSize = SingleValue(Value::TYPE_RELATIVE, value);
				} else {
					xSize = SingleValue(Value::TYPE_ABSOLUTE, value);
				}
			} else if (name == "ySize") {
				if (type == "relative") {
					ySize = SingleValue(Value::TYPE_RELATIVE, value);
				} else {
					ySize = SingleValue(Value::TYPE_ABSOLUTE, value);
				}
			} else if (name == "zSize") {
				if (type == "relative") {
					zSize = SingleValue(Value::TYPE_RELATIVE, value);
				} else {
					zSize = SingleValue(Value::TYPE_ABSOLUTE, value);
				}
			}
		}

		child = child.nextSibling();
	}

	return new SizeOperator(xSize, ySize, zSize);
}

Operator* parseSplitOperator(const QDomNode& node) {
	int splitAxis;
	std::vector<Value*> sizes;
	std::vector<std::string> names;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");

			if (name == "splitAxis") {
				if (child.toElement().attribute("value") == "x") {
					splitAxis = DIRECTION_X;
				} else if (child.toElement().attribute("value") == "y") {
					splitAxis = DIRECTION_Y;
				} else if (child.toElement().attribute("value") == "z") {
					splitAxis = DIRECTION_Z;
				}
			} else if (name == "size") {
				QDomNode element = child.firstChild();
				while (!element.isNull()) {
					if (element.toElement().tagName() == "element") {
						QString type = element.toElement().attribute("type");
						std::string value = element.toElement().attribute("value").toUtf8().constData();
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

	return new SplitOperator(splitAxis, sizes, names);
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
	int mode;
	int coordSystem;
	float x, y, z;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");

			if (name == "mode") {
				if (child.toElement().attribute("value") == "abs") {
					mode = MODE_ABSOLUTE;
				} else if (child.toElement().attribute("value") == "rel") {
					mode = MODE_RELATIVE;
				} else {
					throw "mode has to be either abs or rel.";
				}
			} else if (name == "coordSystem") {
				if (child.toElement().attribute("value") == "world") {
					coordSystem = COORD_SYSTEM_WORLD;
				} else if (child.toElement().attribute("value") == "object") {
					coordSystem = COORD_SYSTEM_OBJECT;
				} else {
					throw "coordSystem has to be either world or object.";
				}
			} else if (name == "x") {
				x = child.toElement().attribute("value").toFloat();
			} else if (name == "y") {
				y = child.toElement().attribute("value").toFloat();
			} else if (name == "z") {
				z = child.toElement().attribute("value").toFloat();
			}
		}

		child = child.nextSibling();
	}

	return new TranslateOperator(mode, coordSystem, glm::vec3(x, y, z));
}

}
