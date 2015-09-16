#pragma once

#include "Rule.h"

namespace cga {

class OffsetOperator : public Operator {
private:
	std::string offsetDistance;
	int offsetSelector;

public:
	OffsetOperator(const std::string& offsetDistance, int offsetSelector);

	boost::shared_ptr<Shape> apply(boost::shared_ptr<Shape>& shape, const RuleSet& ruleSet, std::list<boost::shared_ptr<Shape> >& stack);
};

}