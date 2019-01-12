#pragma once

#include "Operator.h"
#include <unordered_map>

namespace procedural_generation {

	class ComponentSplitOperator : public Operator {
	public:
		ComponentSplitOperator(const std::unordered_map<std::string, std::string>& nameMap);
		virtual void apply(ShapePtr shape, std::queue<ShapePtr>& derivationQueue) const override;

	private:
		std::unordered_map<std::string, std::string> _nameMap;
	};

}
