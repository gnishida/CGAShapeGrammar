#include "ComponentSplitOperator.h"

namespace procedural_generation {

	ComponentSplitOperator::ComponentSplitOperator(const std::unordered_map<std::string, std::string>& nameMap) : Operator("componentSplit"), _nameMap(nameMap) {
	}

	void ComponentSplitOperator::apply(ShapePtr shape, std::queue<ShapePtr>& derivationQueue) const {
		std::vector<ShapePtr> shapes;
		shape->componentSplit(_nameMap, shapes);
		for (auto& shape : shapes) {
			derivationQueue.push(shape);
		}
	}

}