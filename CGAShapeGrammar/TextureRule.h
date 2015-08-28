#pragma once

#include "Rule.h"

namespace cga {

class TextureRule : public Rule {
private:
	std::string texture;

public:
	TextureRule(const std::string& texture, const std::string& output_name);
	void apply(Object* obj, std::list<Object*>& stack);
};

}
