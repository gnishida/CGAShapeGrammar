#include "TaperOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

TaperOperator::TaperOperator(const std::string& height, const std::string& top_ratio) {
	this->name = "taper";
	this->height = height;
	this->top_ratio = top_ratio;
}

boost::shared_ptr<Shape> TaperOperator::apply(boost::shared_ptr<Shape>& shape, const RuleSet& ruleSet, std::list<boost::shared_ptr<Shape> >& stack) {
	float actual_height = ruleSet.evalFloat(height, shape);
	float actual_top_ratio = ruleSet.evalFloat(top_ratio, shape);
	
	return shape->taper(shape->_name, actual_height, actual_top_ratio);
}

}
