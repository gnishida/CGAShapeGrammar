#pragma once

#include <string>
#include <vector>
#include <map>
#include <list>

namespace cga {

class Object;

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
	SingleValue(int type, float value) { this->type = type; this->value = value; this->repeat = false; }

	float getEstimateValue(float size) { return value; }
};


class ValueSet : public Value {
public:
	ValueSet(Value* value, bool repeat = false);
	ValueSet(const std::vector<Value*>& values, bool repeat = false);

	float getEstimateValue(float size);
};

class Rule {
protected:
	std::string output_name;

public:
	Rule() {}

	virtual void apply(Object* obj, std::list<Object*>& stack) = 0;
	static void decodeSplitSizes(float size, const std::vector<Value*>& sizes, const std::vector<std::string>& output_names, std::vector<float>& decoded_sizes, std::vector<std::string>& decoded_output_names);
	static std::map<std::string, cga::Rule*> parseRule(char* filename);
};

}
