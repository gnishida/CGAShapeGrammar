#pragma once

#include "Operator.h"

namespace procedural_generation {

	class OffsetOperator : public Operator {
	public:
		OffsetOperator(const float offsetDistance, const std::string& inside, const std::string& border);

		virtual void apply(ShapePtr shape, std::queue<ShapePtr>& derivationQueue) const override;

	private:
		float _offsetDistance;
		std::string _inside;
		std::string _border;
	};

}
