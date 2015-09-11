#pragma once

#include "Rule.h"

namespace cga {

class TextureOperator : public Operator {
private:
	std::string texture;

public:
	TextureOperator(const std::string& texture);
	Shape* apply(Shape* obj, const RuleSet& ruleSet, std::list<Shape*>& stack);
};

}
