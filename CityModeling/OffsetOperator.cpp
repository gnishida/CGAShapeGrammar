#include "OffsetOperator.h"

namespace procedural_generation {

	OffsetOperator::OffsetOperator(const float offsetDistance, const std::string& inside, const std::string& border) : Operator("offset"), _offsetDistance(offsetDistance), _inside(inside), _border(border) {
	}

	void OffsetOperator::apply(ShapePtr shape, std::queue<ShapePtr>& derivationQueue) const {
		std::vector<ShapePtr> shapes;
		shape->offset(shape->name(), _offsetDistance, _inside, _border, shapes);
		for (auto& shape : shapes) {
			derivationQueue.push(shape);
		}
	}

}
