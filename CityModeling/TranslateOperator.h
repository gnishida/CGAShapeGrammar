#pragma once

#include "Operator.h"

namespace procedural_generation {

	class TranslateOperator : public Operator {
	public:
		TranslateOperator(const TranslateMode translateMode, const CoordinateSystem coordSystem, const Value& x, const Value& y, const Value& z);
		virtual void apply(ShapePtr shape, std::queue<ShapePtr>& derivationQueue) const override;

	private:
		TranslateMode _translateMode;
		CoordinateSystem _coordSystem;
		Value _x;
		Value _y;
		Value _z;
	};

}