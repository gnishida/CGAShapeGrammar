#pragma once

#include "Rule.h"

namespace cga {

class InnerSemiCircleOperator : public Operator {
public:
	InnerSemiCircleOperator();

	boost::shared_ptr<Shape> apply(boost::shared_ptr<Shape>& shape, const RuleSet& ruleSet, std::list<boost::shared_ptr<Shape> >& stack);
};

}