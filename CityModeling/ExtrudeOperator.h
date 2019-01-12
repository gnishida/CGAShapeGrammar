#pragma once

#include "Operator.h"

namespace procedural_generation {

	class ExtrudeOperator : public Operator {
	public:
		ExtrudeOperator(const float height, const std::string& outputName);

		virtual void apply(std::shared_ptr<Shape> shape, std::queue<std::shared_ptr<Shape>>& derivationQueue) const override;

	private:
		float _height;
		std::string _outputName;
	};

}
