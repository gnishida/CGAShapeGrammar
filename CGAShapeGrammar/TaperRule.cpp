#include "TaperRule.h"
#include "CGA.h"

namespace cga {

TaperRule::TaperRule(float height, float top_ratio, const std::string& output_name) {
	this->height = height;
	this->top_ratio = top_ratio;
	this->output_name = output_name;
}

void TaperRule::apply(Object* obj, std::list<Object*>& stack) {
	stack.push_back(obj->taper(output_name, height, top_ratio));
}

}
