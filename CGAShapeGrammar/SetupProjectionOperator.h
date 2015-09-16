#pragma once

#include "Rule.h"

namespace cga {

class SetupProjectionOperator : public Operator {
private:
	int axesSelector;
	Value texWidth;
	Value texHeight;

public:
	SetupProjectionOperator(int axesSelector, const Value& texWidth, const Value& texHeight);
	boost::shared_ptr<Shape> apply(boost::shared_ptr<Shape>& shape, const RuleSet& ruleSet, std::list<boost::shared_ptr<Shape> >& stack);
};

}