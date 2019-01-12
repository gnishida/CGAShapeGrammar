#pragma once

#include "Operator.h"

namespace procedural_generation {

	class SetupProjectionOperator : public Operator {

	public:
		SetupProjectionOperator(AxisDirection axesSelector, const Value& texWidth, const Value& texHeight, const std::string& outputName);
		virtual void apply(std::shared_ptr<Shape> shape, std::queue<std::shared_ptr<Shape>>& derivationQueue) const override;

	private:
		AxisDirection _axesSelector;
		Value _texWidth;
		Value _texHeight;
		std::string _outputName;
	};

}