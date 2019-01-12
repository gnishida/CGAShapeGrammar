#pragma once

#include <string>
#include <queue>
#include <memory>

#include "Shape.h"

namespace procedural_generation {

	class Operator {
	public:
		Operator(const std::string& name);
		virtual ~Operator() {}

		virtual void apply(ShapePtr shape, std::queue<ShapePtr>& derivationQueue) const = 0;

	protected:
		std::string _name;

	};

	using OperatorPtr = std::shared_ptr<Operator>;

}
