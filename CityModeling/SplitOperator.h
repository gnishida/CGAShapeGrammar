#pragma once

#include "Operator.h"

namespace procedural_generation {

	class SplitOperator : public Operator {
	public:
		SplitOperator(SplitDirection splitAxis, const std::vector<std::pair<std::string, Value>> & sizeAndNames);
		virtual void apply(std::shared_ptr<Shape> shape, std::queue<std::shared_ptr<Shape>>& derivationQueue) const override;

	private:
		SplitDirection _splitAxis;
		std::vector<std::pair<std::string, Value>> _sizeAndNames;
	};

}
