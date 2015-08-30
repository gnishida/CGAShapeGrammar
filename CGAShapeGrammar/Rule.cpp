#include "Rule.h"
#include "CGA.h"

namespace cga {

ValueSet::ValueSet(Value* value, bool repeat) {
	this->type = TYPE_SET;
	values.push_back(value);
	this->repeat = repeat;
}

ValueSet::ValueSet(const std::vector<Value*>& values, bool repeat) {
	this->type = TYPE_SET;
	this->values = values;
	this->repeat = repeat;
}

float ValueSet::getEstimateValue(float size) {
	float sum = 0.0f;
	for (int i = 0; i < values.size(); ++i) {
		if (values[i]->type == Value::TYPE_ABSOLUTE) {
			sum += values[i]->value;
		} else if (values[i]->type == Value::TYPE_RELATIVE) {
			sum += size * values[i]->value;
		} else if (values[i]->type == Value::TYPE_FLOATING) {
			sum += values[i]->value;
		}
	}

	return sum;
}

void Rule::apply(Shape* obj, std::list<Shape*>& stack) {
	for (int i = 0; i < operators.size(); ++i) {
		obj = operators[i]->apply(obj, stack);
	}
	
	if (obj != NULL) {
		obj->_name = output_name;
		stack.push_back(obj);
	}
}

void Rule::decodeSplitSizes(float size, const std::vector<Value*>& sizes, const std::vector<std::string>& output_names, std::vector<float>& decoded_sizes, std::vector<std::string>& decoded_output_names) {
	int regular_count = 0;
	int floating_count = 0;
	float regular_sum = 0.0f;
	float floating_sum = 0.0f;
	bool repeat = false;

	for (int i = 0; i < sizes.size(); ++i) {
		if (sizes[i]->type == Value::TYPE_ABSOLUTE) {
			regular_count++;
			regular_sum += sizes[i]->value;
		} else if (sizes[i]->type == Value::TYPE_RELATIVE) {
			regular_count++;
			regular_sum += size * sizes[i]->value * size;
		} else if (sizes[i]->type == Value::TYPE_FLOATING) {
			floating_count++;
			floating_sum += sizes[i]->value;
		}
	}

	float scale;
	if (floating_count > 0) {
		scale = (size - regular_sum) / floating_sum;
	}

	for (int i = 0; i < sizes.size(); ++i) {
		if (sizes[i]->type == Value::TYPE_ABSOLUTE) {
			decoded_sizes.push_back(sizes[i]->value);
			decoded_output_names.push_back(output_names[i]);
		} else if (sizes[i]->type == Value::TYPE_RELATIVE) {
			decoded_sizes.push_back(sizes[i]->value * size);
			decoded_output_names.push_back(output_names[i]);
		} else if (sizes[i]->type == Value::TYPE_FLOATING) {
			decoded_sizes.push_back(sizes[i]->value * scale);
			decoded_output_names.push_back(output_names[i]);
		} else if (sizes[i]->type == Value::TYPE_SET) {
			if (sizes[i]->repeat) {
				/*std::vector<Value*> temp_ratios(1);
				temp_ratios[0] = ratios[i];*/
				std::vector<std::string> temp_names(sizes[i]->values.size());
				for (int k = 0; k < temp_names.size(); ++k) {
					temp_names[k] = output_names[i];
				}

				float s = sizes[i]->getEstimateValue(size - regular_sum);
				int num = (size - regular_sum) / s;
				for (int k = 0; k < num; ++k) {
					decodeSplitSizes((size - regular_sum) / num, sizes[i]->values, temp_names, decoded_sizes, decoded_output_names);
				}
			} else {
				std::vector<Value*> temp_ratios(1);
				temp_ratios[0] = sizes[i];
				std::vector<std::string> temp_names(1);
				temp_names[0] = output_names[i];
				decodeSplitSizes(size - regular_sum, temp_ratios, temp_names, decoded_sizes, decoded_output_names);
			}
		}
	}
}

}
