#pragma once

#include "Rule.h"

namespace cga {

class SetupProjectionOperator : public Operator {
public:
	static enum { TYPE_ABSOLUTE = 0, TYPE_RELATIVE };

private:
	int type;
	float texWidth;
	float texHeight;

public:
	SetupProjectionOperator(int type, float texWidth, float texHeight);
	//SetupProjectionRule(float u1, float v1, float u2, float v2, const std::string& output_name);
	Object* apply(Object* obj, std::list<Object*>& stack);
};

}