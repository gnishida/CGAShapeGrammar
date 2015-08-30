#include "SplitOperator.h"
#include "CGA.h"

namespace cga {

SplitOperator::SplitOperator(int direction, const std::vector<Value*>& sizes, const std::vector<std::string>& output_names) {
	this->direction = direction;
	this->sizes = sizes;
	this->output_names = output_names;
}

Shape* SplitOperator::apply(Shape* obj, std::list<Shape*>& stack) {
	std::vector<Shape*> floors;

	std::vector<float> decoded_sizes;
	std::vector<std::string> decoded_output_names;
	if (direction == DIRECTION_X) {
		Rule::decodeSplitSizes(obj->_scope.x, sizes, output_names, decoded_sizes, decoded_output_names);
	} else {
		Rule::decodeSplitSizes(obj->_scope.y, sizes, output_names, decoded_sizes, decoded_output_names);
	}

	obj->split(direction, decoded_sizes, decoded_output_names, floors);
	stack.insert(stack.end(), floors.begin(), floors.end());

	delete obj;
	return NULL;
}

}
