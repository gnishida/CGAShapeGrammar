#pragma once

#include <string>
#include <vector>
#include <map>
#include <list>

namespace cga {

class Shape;

class Value  {
public:
	static enum { TYPE_ABSOLUTE = 0, TYPE_RELATIVE, TYPE_FLOATING, TYPE_SET };

public:
	int type;
	float value;
	std::vector<Value*> values;
	bool repeat;

public:
	virtual float getEstimateValue(float size) = 0;
};

class SingleValue : public Value {
public:
	SingleValue() { this->type = TYPE_ABSOLUTE; this->value = 0.0f; this->repeat = false; }
	SingleValue(int type, float value) { this->type = type; this->value = value; this->repeat = false; }

	float getEstimateValue(float size) { return value; }
};


class ValueSet : public Value {
public:
	ValueSet(Value* value, bool repeat = false);
	ValueSet(const std::vector<Value*>& values, bool repeat = false);

	float getEstimateValue(float size);
};

class Operator {
public:
	Operator() {}

	virtual Shape* apply(Shape* obj, std::list<Shape*>& stack) = 0;
};

class Rule {
public:
	std::string output_name;
	std::vector<Operator*> operators;

public:
	Rule() {}

	void apply(Shape* obj, std::list<Shape*>& stack) const;
	static void decodeSplitSizes(float size, const std::vector<Value*>& sizes, const std::vector<std::string>& output_names, std::vector<float>& decoded_sizes, std::vector<std::string>& decoded_output_names);
};

class RuleSet {
public:
	std::map<std::string, float> attrs;
	std::map<std::string, cga::Rule> rules;

public:
	RuleSet() {}

	bool contain(const std::string& name) const;
	Rule getRule(const std::string& name) const { return rules.at(name); }
	Rule& getRule(const std::string& name) { return rules[name]; }
	void addAttr(const std::string& name, float value);
	void addOperator(const std::string& name, Operator* op);
	void setRuleOutput(const std::string& name, const std::string& output_name);
};

}
