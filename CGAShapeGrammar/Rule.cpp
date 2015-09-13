#include "Rule.h"
#include "CGA.h"
#include "Shape.h"
#include "NumberEval.h"
#include <sstream>
#include <boost/algorithm/string/replace.hpp>

namespace cga {

float Value::getEstimateValue(float size, const RuleSet& ruleSet, Shape* shape) const {
	if (type == Value::TYPE_ABSOLUTE) {
		return ruleSet.evalFloat(value, shape);
	} else if (type == Value::TYPE_RELATIVE) {
		return ruleSet.evalFloat(value, shape) * size;
	} else {
		return ruleSet.evalFloat(value, shape);
	}
}

void Rule::apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack) const {
	for (int i = 0; i < operators.size(); ++i) {
		shape = operators[i]->apply(shape, ruleSet, stack);
		if (shape == NULL) break;
	}
	
	if (shape != NULL) {
		if (operators.size() == 0 || operators.back()->name == "copy") {
			delete shape;
			shape = NULL;
		} else {
			std::stringstream ss;
			ss << shape->_name << "!";
			shape->_name = ss.str();
			stack.push_back(shape);
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
void Rule::decodeSplitSizes(float size, const std::vector<Value>& sizes, const std::vector<std::string>& output_names, const RuleSet& ruleSet, Shape* shape, std::vector<float>& decoded_sizes, std::vector<std::string>& decoded_output_names) {
	float regular_sum = 0.0f;
	float floating_sum = 0.0f;
	int repeat_count = 0;

	for (int i = 0; i < sizes.size(); ++i) {
		if (sizes[i].repeat) {
			repeat_count++;
		} else {
			if (sizes[i].type == Value::TYPE_ABSOLUTE) {
				regular_sum += ruleSet.evalFloat(sizes[i].value, shape);
			} else if (sizes[i].type == Value::TYPE_RELATIVE) {
				regular_sum += size * ruleSet.evalFloat(sizes[i].value, shape) * size;
			} else if (sizes[i].type == Value::TYPE_FLOATING) {
				floating_sum += ruleSet.evalFloat(sizes[i].value, shape);
			}
		}
	}

	float floating_scale = 1.0f;
	if (floating_sum > 0 && repeat_count == 0) {
		floating_scale = (size - regular_sum) / floating_sum;
	}

	for (int i = 0; i < sizes.size(); ++i) {
		if (sizes[i].repeat) {
			float s = sizes[i].getEstimateValue(size - regular_sum - floating_sum * floating_scale, ruleSet, shape);
			int num = (size - regular_sum - floating_sum * floating_scale) / s;
			s = (size - regular_sum - floating_sum * floating_scale) / num;
			for (int k = 0; k < num; ++k) {
				decoded_sizes.push_back(s);
				decoded_output_names.push_back(output_names[i]);
			}
		} else {
			if (sizes[i].type == Value::TYPE_ABSOLUTE) {
				decoded_sizes.push_back(ruleSet.evalFloat(sizes[i].value, shape));
				decoded_output_names.push_back(output_names[i]);
			} else if (sizes[i].type == Value::TYPE_RELATIVE) {
				decoded_sizes.push_back(ruleSet.evalFloat(sizes[i].value, shape) * size);
				decoded_output_names.push_back(output_names[i]);
			} else if (sizes[i].type == Value::TYPE_FLOATING) {
				decoded_sizes.push_back(ruleSet.evalFloat(sizes[i].value, shape) * floating_scale);
				decoded_output_names.push_back(output_names[i]);
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
