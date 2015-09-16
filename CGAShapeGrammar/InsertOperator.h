#pragma once

#include "Rule.h"

namespace cga {

class InsertOperator : public Operator {
private:
	std::string geometryPath;

public:
	InsertOperator(const std::string& geometryPath);

	boost::shared_ptr<Shape> apply(boost::shared_ptr<Shape>& shape, const RuleSet& ruleSet, std::list<boost::shared_ptr<Shape> >& stack);
};

}