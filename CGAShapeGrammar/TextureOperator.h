#pragma once

#include "Rule.h"

namespace cga {

class TextureOperator : public Operator {
private:
	std::string texture;

public:
	TextureOperator(const std::string& texture);
	Object* apply(Object* obj, std::list<Object*>& stack);
};

}
