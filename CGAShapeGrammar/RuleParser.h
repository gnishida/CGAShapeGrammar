#pragma once

#include <map>
#include "Rule.h"

namespace cga {

std::map<std::string, cga::Rule*> parseRule(char* filename);

}