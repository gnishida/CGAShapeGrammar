#pragma once

#include "Rule.h"

namespace cga {

class RoofHipOperator : public Operator {
private:
	std::string angle;

public:
	RoofHipOperator(const std::string& angle);

	boost::shared_ptr<Shape> apply(boost::shared_ptr<Shape>& shape, const RuleSet& ruleSet, std::list<boost::shared_ptr<Shape> >& stack);
};

}