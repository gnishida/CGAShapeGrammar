#pragma once

#include "Rule.h"

namespace cga {

class CenterOperator : public Operator {
private:
	int axesSelector;

public:
	CenterOperator(int axesSelector);

	boost::shared_ptr<Shape> apply(boost::shared_ptr<Shape>& shape, const RuleSet& ruleSet, std::list<boost::shared_ptr<Shape> >& stack);
};

}