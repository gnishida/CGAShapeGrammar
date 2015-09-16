#pragma once

#include "Rule.h"

namespace cga {

class ShapeLOperator : public Operator {
private:
	float frontWidth;
	float leftWidth;

public:
	ShapeLOperator(float frontWidth, float leftWidth);

	boost::shared_ptr<Shape> apply(boost::shared_ptr<Shape>& shape, const RuleSet& ruleSet, std::list<boost::shared_ptr<Shape> >& stack);
};

}