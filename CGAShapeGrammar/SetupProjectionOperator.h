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
	Shape* apply(Shape* obj, std::list<Shape*>& stack);
};

}