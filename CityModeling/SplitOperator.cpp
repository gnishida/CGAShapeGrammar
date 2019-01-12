#include "SplitOperator.h"

namespace procedural_generation {

	SplitOperator::SplitOperator(SplitDirection splitAxis, const std::vector<std::pair<std::string, Value>>& sizeAndNames) : Operator("split"), _sizeAndNames(sizeAndNames) {
	}

	void SplitOperator::apply(std::shared_ptr<Shape> shape, std::queue<std::shared_ptr<Shape>>& derivationQueue) const {
		std::vector<std::shared_ptr<Shape>> shapes;

		shape->split(_splitAxis, _sizeAndNames, shapes);
		for (const auto& shape : shapes) {
			derivationQueue.push(shape);
		}
	}

}
