#include "Rule.h"
#include "CGA.h"
#include "Shape.h"
#include <sstream>

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

void Rule::apply(Shape* obj, const RuleSet& ruleSet, std::list<Shape*>& stack) const {
	for (int i = 0; i < operators.size(); ++i) {
		obj = operators[i]->apply(obj, ruleSet, stack);
		if (obj == NULL) break;
	}
	
	if (obj != NULL) {
		if (operators.size() == 0 || operators.back()->name == "copy") {
			delete obj;
			obj = NULL;
		} else {
			std::stringstream ss;
			ss << obj->_name << "!";
			obj->_name = ss.str();
			stack.push_back(obj);
		}
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

bool RuleSet::contain(const std::string& name) const {
	if (rules.find(name) == rules.end()) return false;
	else return true;
}

void RuleSet::addAttr(const std::string& name, const std::string& value) {
	attrs[name] = value;
}

void RuleSet::addRule(const std::string& name) {
	rules[name].operators.clear();
}

void RuleSet::addOperator(const std::string& name, Operator* op) {
	rules[name].operators.push_back(op);
}

float RuleSet::evalFloat(const std::string& attr_name) const {
	if (attrs.find(attr_name) == attrs.end()) {
		return ::atof(attr_name.c_str());
	} else {
		return ::atof(attrs.at(attr_name).c_str());
	}
}

std::string RuleSet::evalString(const std::string& attr_name) const {
	if (attrs.find(attr_name) == attrs.end()) {
		return attr_name;
	} else {
		return attrs.at(attr_name);
	}
}

}
