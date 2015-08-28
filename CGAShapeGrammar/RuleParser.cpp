#include "RuleParser.h"
#include "SetupProjectionRule.h"
#include "ExtrudeRule.h"
#include "ComponentSplitRule.h"
#include "SetTextureRule.h"
#include "SplitRule.h"
#include "TranslateRule.h"
#include "CGA.h"

namespace cga {

std::map<std::string, cga::Rule*> parseRule(char* filename) {
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
