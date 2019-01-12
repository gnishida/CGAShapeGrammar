#pragma once

#include "Operator.h"

namespace procedural_generation {

	class ColorOperator : public Operator {
	public:
		ColorOperator(float red, float green, float blue, float alpha, const std::string& outputName);

		virtual void apply(std::shared_ptr<Shape> shape, std::queue<std::shared_ptr<Shape>>& derivationQueue) const override;

	private:
		float _red;
		float _green;
		float _blue;
		float _alpha;
		std::string _outputName;
	};

}