#include "SizeOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

SizeOperator::SizeOperator(const SingleValue& xSize, const SingleValue& ySize, const SingleValue& zSize) {
	this->name = "size";
	this->xSize = xSize;
	this->ySize = ySize;
	this->zSize = zSize;
}

Shape* SizeOperator::apply(Shape* obj, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	obj->size(xSize, ySize, zSize);
	return obj;
}

}
