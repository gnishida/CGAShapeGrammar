#include "Rule.h"
#include "CGA.h"
#include "Shape.h"
#include "NumberEval.h"
#include <sstream>
#include <boost/algorithm/string/replace.hpp>

namespace cga {

float SingleValue::getEstimateValue(float size, const RuleSet& ruleSet, Shape* obj) {
	if (type == Value::TYPE_ABSOLUTE) {
		return ruleSet.evalFloat(value, obj);
	} else if (type == Value::TYPE_RELATIVE) {
		return ruleSet.evalFloat(value, obj) * size;
	} else {
		return ruleSet.evalFloat(value, obj);
	}
}

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

float ValueSet::getEstimateValue(float size, const RuleSet& ruleSet, Shape* obj) {
	float sum = 0.0f;
	for (int i = 0; i < values.size(); ++i) {
		if (values[i]->type == Value::TYPE_ABSOLUTE) {
			sum += ruleSet.evalFloat(values[i]->value, obj);
		} else if (values[i]->type == Value::TYPE_RELATIVE) {
			sum += ruleSet.evalFloat(values[i]->value, obj) * size;
		} else if (values[i]->type == Value::TYPE_FLOATING) {
			sum += ruleSet.evalFloat(values[i]->value, obj);
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

/**
 * 指定されたsizeをsplitする後の、各断片のサイズを計算する。
 *
 * @param size							もとのsize
 * @param sizes							指定された、各断片のサイズ
 * @param output_names					指定された、各断片の名前
 * @param ruleSet						ルール (sizeなどで変数が使用されている場合、解決するため)
 * @param decoded_sizes	[OUT]			計算された、各断片のサイズ
 * @param decoded_output_names [OUT]	計算された、各断片の名前
 */
void Rule::decodeSplitSizes(float size, const std::vector<Value*>& sizes, const std::vector<std::string>& output_names, const RuleSet& ruleSet, Shape* obj, std::vector<float>& decoded_sizes, std::vector<std::string>& decoded_output_names) {
	int regular_count = 0;
	int floating_count = 0;
	float regular_sum = 0.0f;
	float floating_sum = 0.0f;
	bool repeat = false;
	int repeat_count = 0;

	for (int i = 0; i < sizes.size(); ++i) {
		if (sizes[i]->type == Value::TYPE_ABSOLUTE) {
			regular_count++;
			regular_sum += ruleSet.evalFloat(sizes[i]->value, obj);
		} else if (sizes[i]->type == Value::TYPE_RELATIVE) {
			regular_count++;
			regular_sum += size * ruleSet.evalFloat(sizes[i]->value, obj) * size;
		} else if (sizes[i]->type == Value::TYPE_FLOATING) {
			floating_count++;
			floating_sum += ruleSet.evalFloat(sizes[i]->value, obj);
		} else if (sizes[i]->type == Value::TYPE_SET) {
			repeat_count++;
		}
	}

	float floating_scale = 1.0f;
	if (floating_count > 0 && repeat_count == 0) {
		floating_scale = (size - regular_sum) / floating_sum;
	}

	for (int i = 0; i < sizes.size(); ++i) {
		if (sizes[i]->type == Value::TYPE_ABSOLUTE) {
			decoded_sizes.push_back(ruleSet.evalFloat(sizes[i]->value, obj));
			decoded_output_names.push_back(output_names[i]);
		} else if (sizes[i]->type == Value::TYPE_RELATIVE) {
			decoded_sizes.push_back(ruleSet.evalFloat(sizes[i]->value, obj) * size);
			decoded_output_names.push_back(output_names[i]);
		} else if (sizes[i]->type == Value::TYPE_FLOATING) {
			decoded_sizes.push_back(ruleSet.evalFloat(sizes[i]->value, obj) * floating_scale);
			decoded_output_names.push_back(output_names[i]);
		} else if (sizes[i]->type == Value::TYPE_SET) {
			if (sizes[i]->repeat) {
				/*std::vector<Value*> temp_ratios(1);
				temp_ratios[0] = ratios[i];*/
				std::vector<std::string> temp_names(sizes[i]->values.size());
				for (int k = 0; k < temp_names.size(); ++k) {
					temp_names[k] = output_names[i];
				}

				float s = sizes[i]->getEstimateValue(size - regular_sum, ruleSet, obj);
				int num = (size - regular_sum - floating_sum * floating_scale) / s;
				for (int k = 0; k < num; ++k) {
					decodeSplitSizes((size - regular_sum - floating_sum * floating_scale) / num, sizes[i]->values, temp_names, ruleSet, obj, decoded_sizes, decoded_output_names);
				}
			} else {
				std::vector<Value*> temp_ratios(1);
				temp_ratios[0] = sizes[i];
				std::vector<std::string> temp_names(1);
				temp_names[0] = output_names[i];
				decodeSplitSizes(size - regular_sum - floating_sum * floating_scale, temp_ratios, temp_names, ruleSet, obj, decoded_sizes, decoded_output_names);
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

float RuleSet::evalFloat(const std::string& attr_name, Shape* shape) const {
	// To be fixed
	// 置換だと、変数BCが、変数ABCを置換してしまう。
	// 対策は？

	// scope.sx|y|zを置換
	std::string decoded_str = attr_name;
	std::string scope_x = std::to_string((long double)shape->_scope.x);
	std::string scope_y = std::to_string((long double)shape->_scope.y);
	std::string scope_z = std::to_string((long double)shape->_scope.z);
	boost::replace_all(decoded_str, "scope.sx", scope_x);
	boost::replace_all(decoded_str, "scope.sy", scope_y);
	boost::replace_all(decoded_str, "scope.sz", scope_z);

	// 変数を置換
	for (auto it = attrs.begin(); it != attrs.end(); ++it) {
		boost::replace_all(decoded_str, it->first, it->second);
	}

	return calculate(decoded_str);
}

std::string RuleSet::evalString(const std::string& attr_name, Shape* shape) const {
	if (attrs.find(attr_name) == attrs.end()) {
		return attr_name;
	} else {
		return attrs.at(attr_name);
	}
}

}
