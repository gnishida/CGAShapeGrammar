#pragma once

#include "Rule.h"

namespace cga {

class CopyRule : public Rule {
private:
	std::string copy_name;

public:
	CopyRule(const std::string& copy_name, const std::string& output_name);

	void apply(Object* obj, std::list<Object*>& stack);
};

}