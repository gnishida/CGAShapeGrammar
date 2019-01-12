#pragma once

#include "Operator.h"

namespace procedural_generation {

	class RotateOperator : public Operator {
	public:
		RotateOperator(float xAngle, float yAngle, float zAngle, const std::string& outputName);

		virtual void apply(std::shared_ptr<Shape> shape, std::queue<std::shared_ptr<Shape>>& derivationQueue) const override;

	private:
		float _xAngle;
		float _yAngle;
		float _zAngle;
		std::string _outputName;
	};

}