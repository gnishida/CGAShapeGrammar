#pragma once

#include "Rule.h"

namespace cga {

class TextureOperator : public Operator {
private:
	std::string texture;

public:
	TextureOperator(const std::string& texture);
	boost::shared_ptr<Shape> apply(boost::shared_ptr<Shape>& shape, const RuleSet& ruleSet, std::list<boost::shared_ptr<Shape> >& stack);
};

}
