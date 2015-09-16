#include "CenterOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

CenterOperator::CenterOperator(int axesSelector) {
	this->name = "center";
	this->axesSelector = axesSelector;
}

boost::shared_ptr<Shape> CenterOperator::apply(boost::shared_ptr<Shape>& shape, const RuleSet& ruleSet, std::list<boost::shared_ptr<Shape> >& stack) {
	shape->center(axesSelector);

	return shape;
}

}
