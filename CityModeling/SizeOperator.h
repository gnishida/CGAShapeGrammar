#pragma once

#include "Operator.h"

namespace procedural_generation {

	class SizeOperator : public Operator {
	public:
		SizeOperator(const Value& xSize, const Value& ySize, const Value& zSize, const bool centered, const std::string& outputName);

		virtual void apply(ShapePtr shape, std::queue<ShapePtr>& derivationQueue) const override;

	private:
		Value _xSize;
		Value _ySize;
		Value _zSize;
		bool _centered;
		std::string _outputName;
	};

}
