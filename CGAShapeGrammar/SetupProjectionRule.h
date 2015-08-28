#pragma once

#include "Rule.h"

namespace cga {

class SetupProjectionRule : public Rule {
public:
	static enum { TYPE_ABSOLUTE = 0, TYPE_RELATIVE };

private:
	int type;
	float texWidth;
	float texHeight;

public:
	SetupProjectionRule(int type, float texWidth, float texHeight, const std::string& output_name);
	//SetupProjectionRule(float u1, float v1, float u2, float v2, const std::string& output_name);
	void apply(Object* obj, std::list<Object*>& stack);
};

}