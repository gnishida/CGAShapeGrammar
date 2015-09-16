#pragma once

#include "Rule.h"

namespace cga {

class RoofGableOperator : public Operator {
private:
	std::string angle;

public:
	RoofGableOperator(const std::string& angle);

	boost::shared_ptr<Shape> apply(boost::shared_ptr<Shape>& shape, const RuleSet& ruleSet, std::list<boost::shared_ptr<Shape> >& stack);
};

}