#include "ShapeGrammar.h"
#include "ExtrudeOperator.h"
#include "ComponentSplitOperator.h"
#include "SplitOperator.h"
#include "OffsetOperator.h"
#include "ColorOperator.h"
#include "SetupProjectionOperator.h"
#include "TextureOperator.h"
#include <iostream>

namespace procedural_generation {

	void ShapeGrammar::setAxiom(std::shared_ptr<Shape> axiom) {
		_derivationQueue.push(axiom);
	}

	void ShapeGrammar::addRule(const std::string& ruleName, std::shared_ptr<Operator> productionRule) {
		if (_productionRles.find(ruleName) != _productionRles.end()) {
			std::cerr << "Warning: duplicate production rules with the same name, " << ruleName << std::endl;
		}

		_productionRles[ruleName] = productionRule;
	}

	void ShapeGrammar::derive(Faces& faces, bool suppressWarning) {
		try {
			while (!_derivationQueue.empty()) {
				std::shared_ptr<Shape> shape = _derivationQueue.front();
				_derivationQueue.pop();

				if (_productionRles.find(shape->name()) != _productionRles.end()) {
					_productionRles[shape->name()]->apply(shape, _derivationQueue);
				}
				else {
					if (!suppressWarning && shape->name().back() != '!' && shape->name().back() != '.') {
						std::cout << "Warning: " << "no rule is found for " << shape->name() << "." << std::endl;
					}

					shape->generateGeometry(faces);
				}
			}
		}
		catch (std::exception& e) {
			std::cout << "Grammar derivation was interrupted by an exception:\n";
			std::cout << e.what() << '\n';
		}
		catch (...) {
			std::cout << "Grammar derivation was interrupted by an unknown error.\n";
		}
	}

	void ShapeGrammar::setSimpleBuildingRules(float height) {
		// building mass
		addRule("Footprint", std::shared_ptr<Operator>(new ExtrudeOperator(height, "BuildingMass")));
		addRule("BuildingMass", std::shared_ptr<Operator>(new ComponentSplitOperator{ { { "top", "TopFace" }, { "side", "SideFace" } } }));
		addRule("TopFace", std::shared_ptr<Operator>(new SetupProjectionOperator(AxisDirection::SCOPE_XY, Value(Value::TYPE_RELATIVE, 1), Value(Value::TYPE_RELATIVE, 1), "TopFace2")));
		addRule("TopFace2", std::shared_ptr<Operator>(new TextureOperator("roof.jpg", "TexturedTopFace")));
		addRule("SideFace", std::shared_ptr<Operator>(new SetupProjectionOperator(AxisDirection::SCOPE_XY, Value(Value::TYPE_ABSOLUTE, 5), Value(Value::TYPE_ABSOLUTE, 5), "SideFace2")));
		addRule("SideFace2", std::shared_ptr<Operator>(new TextureOperator("wall.jpg", "TexturedSideFace")));
	}

	void ShapeGrammar::setBuildingRules(float height) {
		// building mass
		addRule("Footprint", std::shared_ptr<Operator>(new ExtrudeOperator(height, "BuildingMass")));
		addRule("BuildingMass", std::shared_ptr<Operator>(new ComponentSplitOperator{ { { "top", "TopFace" }, { "side", "SideFace" } } }));
		addRule("SideFace", std::shared_ptr<Operator>(new SetupProjectionOperator(AxisDirection::SCOPE_XY, Value(Value::TYPE_ABSOLUTE, 5), Value(Value::TYPE_ABSOLUTE, 5), "SideFace2")));
		addRule("SideFace2", std::shared_ptr<Operator>(new TextureOperator("wall.jpg", "TexturedSideFace")));
		addRule("TexturedSideFace", std::shared_ptr<Operator>(new SplitOperator(SplitDirection::Y, { { "GF", Value(Value::TYPE_ABSOLUTE, 7) }, { "F", Value(Value::TYPE_FLOATING, 5, true) } })));

		// facade
		addRule("GF", std::shared_ptr<Operator>(new SplitOperator(SplitDirection::X, { { "W", Value(Value::TYPE_ABSOLUTE, 1) }, { "GT", Value(Value::TYPE_FLOATING, 6, true) }, { "W", Value(Value::TYPE_ABSOLUTE, 1) } })));
		addRule("F", std::shared_ptr<Operator>(new SplitOperator(SplitDirection::X, { { "W", Value(Value::TYPE_ABSOLUTE, 1) }, { "T", Value(Value::TYPE_FLOATING, 6, true) }, { "W", Value(Value::TYPE_ABSOLUTE, 1) } })));
		addRule("GT", std::shared_ptr<Operator>(new SplitOperator(SplitDirection::Y, { { "GT2", Value(Value::TYPE_FLOATING, 1) }, { "W", Value(Value::TYPE_ABSOLUTE, 1) } })));
		addRule("GT2", std::shared_ptr<Operator>(new SplitOperator(SplitDirection::X, { { "W", Value(Value::TYPE_ABSOLUTE, 1) }, { "DoorTile", Value(Value::TYPE_FLOATING, 1) }, { "W", Value(Value::TYPE_ABSOLUTE, 1) } })));
		addRule("T", std::shared_ptr<Operator>(new SplitOperator(SplitDirection::Y, { { "W", Value(Value::TYPE_ABSOLUTE, 1) }, { "T2", Value(Value::TYPE_FLOATING, 1) }, { "W", Value(Value::TYPE_ABSOLUTE, 0.8) } })));
		addRule("T2", std::shared_ptr<Operator>(new SplitOperator(SplitDirection::X, { { "W", Value(Value::TYPE_ABSOLUTE, 1.5) }, { "WindowTile", Value(Value::TYPE_FLOATING, 1) }, { "W", Value(Value::TYPE_ABSOLUTE, 1.5) } })));

		// door
		addRule("DoorTile", std::shared_ptr<Operator>(new SplitOperator(SplitDirection::Y, { { "DoorTile2", Value(Value::TYPE_FLOATING, 1) }, { "DoorRoofBase", Value(Value::TYPE_ABSOLUTE, 0.5) } })));
		addRule("DoorTile2", std::shared_ptr<Operator>(new ExtrudeOperator(-2, "EntraceSpace")));
		addRule("EntraceSpace", std::shared_ptr<Operator>(new ComponentSplitOperator{ { { "top", "Door" }, { "right", "EntranceWall" }, { "left", "EntranceWall" }, { "back", "EntranceWall" } } }));
		addRule("DoorRoofBase", std::shared_ptr<Operator>(new ExtrudeOperator(0.8, "DoorRoof")));
		addRule("Door", std::shared_ptr<Operator>(new SetupProjectionOperator(AxisDirection::SCOPE_XY, Value(Value::TYPE_RELATIVE, 1), Value(Value::TYPE_RELATIVE, 1), "Door2")));
		addRule("Door2", std::shared_ptr<Operator>(new TextureOperator("door.jpg", "TexturedDoor")));
		addRule("DoorRoof", std::shared_ptr<Operator>(new ColorOperator(0.4, 0.4, 0.4, 1, "ColoredDoorRoof")));
		addRule("EntranceWall", std::shared_ptr<Operator>(new ColorOperator(0.4, 0.4, 0.4, 1, "ColoredEntranceWall")));

		// window
		addRule("WindowTile", std::shared_ptr<Operator>(new SplitOperator(SplitDirection::Y, { { "WindowLedgeBase", Value(Value::TYPE_ABSOLUTE, 0.2) }, { "WindowTile2", Value(Value::TYPE_FLOATING, 1) }, { "WindowRoofBase", Value(Value::TYPE_ABSOLUTE, 0.2) } })));
		addRule("WindowTile2", std::shared_ptr<Operator>(new ExtrudeOperator(-0.2, "WindowSpace")));
		addRule("WindowLedgeBase", std::shared_ptr<Operator>(new ExtrudeOperator(0.5, "WindowFrame")));
		addRule("WindowRoofBase", std::shared_ptr<Operator>(new ExtrudeOperator(0.5, "WindowFrame")));
		addRule("WindowSpace", std::shared_ptr<Operator>(new ComponentSplitOperator{ { { "top", "WindowAndFrame" }, { "side", "WindowFrame" } } }));
		addRule("WindowAndFrame", std::shared_ptr<Operator>(new OffsetOperator(-0.1, "WindowArea", "WindowFrame")));
		addRule("WindowArea", std::shared_ptr<Operator>(new ExtrudeOperator(-0.2, "WindowAreaExtruded")));
		addRule("WindowAreaExtruded", std::shared_ptr<Operator>(new ComponentSplitOperator{ { { "top", "Window" }, { "side", "WindowFrame" } } }));
		addRule("Window", std::shared_ptr<Operator>(new SetupProjectionOperator(AxisDirection::SCOPE_XY, Value(Value::TYPE_RELATIVE, 1), Value(Value::TYPE_RELATIVE, 1), "Window2")));
		addRule("Window2", std::shared_ptr<Operator>(new TextureOperator("window.jpg", "TexturedWindow")));
		addRule("WindowFrame", std::shared_ptr<Operator>(new ColorOperator(0.4, 0.4, 0.4, 1, "ColoredWindowFrame")));

		// roof
		//addRule("TopFace", OperatorPtr(new SetupProjectionOperator(AxisDirection::SCOPE_XY, Value(Value::TYPE_RELATIVE, 1), Value(Value::TYPE_RELATIVE, 1), "TopFace2")));
		//addRule("TopFace2", OperatorPtr(new TextureOperator("roof.jpg", "TexturedTopFace")));

		addRule("TopFace", std::shared_ptr<Operator>(new OffsetOperator(0.4, "OffsetTopFace", "")));
		addRule("OffsetTopFace", std::shared_ptr<Operator>(new ExtrudeOperator(0.1, "Roof")));
		addRule("Roof", std::shared_ptr<Operator>(new ComponentSplitOperator{ { { "top", "RoofBase2" }, { "side", "RoofSide" }, { "bottom", "RoofSide" } } }));
		addRule("RoofBase2", std::shared_ptr<Operator>(new OffsetOperator(0.1, "OffsetRoofBase2", "")));
		addRule("OffsetRoofBase2", std::shared_ptr<Operator>(new ExtrudeOperator(0.4, "Roof2")));
		addRule("Roof2", std::shared_ptr<Operator>(new ComponentSplitOperator{ { { "top", "RoofTop" }, { "side", "RoofSide" }, { "bottom", "RoofSide" } } }));
		addRule("RoofTop", std::shared_ptr<Operator>(new OffsetOperator(-0.5, "OffsetRoofTop", "RoofSide")));
		addRule("OffsetRoofTop", std::shared_ptr<Operator>(new ExtrudeOperator(-0.3, "OffsetRoofTopNegativeExtruded")));
		addRule("OffsetRoofTopNegativeExtruded", std::shared_ptr<Operator>(new ComponentSplitOperator{ { { "top", "RoofTopFace" }, { "side", "RoofSide" } } }));
		addRule("RoofTopFace", std::shared_ptr<Operator>(new SetupProjectionOperator(AxisDirection::SCOPE_XY, Value(Value::TYPE_RELATIVE, 1), Value(Value::TYPE_RELATIVE, 1), "RoofTopFace2")));
		addRule("RoofTopFace2", std::shared_ptr<Operator>(new TextureOperator("roof.jpg", "TexturedTopFace")));
		addRule("RoofSide", std::shared_ptr<Operator>(new ColorOperator(0.4, 0.4, 0.4, 1, "ColoredRoofSide")));
	}

}
