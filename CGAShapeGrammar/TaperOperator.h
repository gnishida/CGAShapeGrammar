#pragma once

#include "Rule.h"

namespace cga {

class TaperOperator : public Operator {
private:
	std::string height;
	std::string top_ratio;

public:
	TaperOperator(const std::string& height, const std::string& top_ratio);

	boost::shared_ptr<Shape> apply(boost::shared_ptr<Shape>& shape, const RuleSet& ruleSet, std::list<boost::shared_ptr<Shape> >& stack);
};

}