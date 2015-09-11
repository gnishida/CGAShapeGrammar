#include "SplitOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

SplitOperator::SplitOperator(int splitAxis, const std::vector<Value*>& sizes, const std::vector<std::string>& output_names) {
	this->name = "split";
	this->splitAxis = splitAxis;
	this->sizes = sizes;
	this->output_names = output_names;
}

Shape* SplitOperator::apply(Shape* obj, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	std::vector<Shape*> floors;

	std::vector<float> decoded_sizes;
	std::vector<std::string> decoded_output_names;
	if (splitAxis == DIRECTION_X) {
		Rule::decodeSplitSizes(obj->_scope.x, sizes, output_names, decoded_sizes, decoded_output_names);
	} else if (splitAxis == DIRECTION_Y) {
		Rule::decodeSplitSizes(obj->_scope.y, sizes, output_names, decoded_sizes, decoded_output_names);
	} else if (splitAxis == DIRECTION_Z) {
		Rule::decodeSplitSizes(obj->_scope.z, sizes, output_names, decoded_sizes, decoded_output_names);
	}

	obj->split(splitAxis, decoded_sizes, decoded_output_names, floors);
	stack.insert(stack.end(), floors.begin(), floors.end());

	delete obj;
	return NULL;
}

}
