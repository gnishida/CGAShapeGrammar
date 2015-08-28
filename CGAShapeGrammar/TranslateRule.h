#pragma once

#include "Rule.h"
#include <glm/gtc/matrix_transform.hpp>

namespace cga {

class TranslateRule : public Rule {
private:
	glm::vec3 vec;

public:
	TranslateRule(const glm::vec3& vec, const std::string& output_name);
	void apply(Object* obj, std::list<Object*>& stack);
};

}
