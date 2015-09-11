#include "ColorOperator.h"
#include "CGA.h"
#include "Shape.h"
#include <sstream>

namespace cga {

ColorOperator::ColorOperator(const std::vector<std::string>& color) {
	this->name = "color";
	this->color = color;
}

Shape* ColorOperator::apply(Shape* obj, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	if (color.size() == 3) {
		obj->_color.r = ruleSet.evalFloat(color[0]);
		obj->_color.g = ruleSet.evalFloat(color[1]);
		obj->_color.b = ruleSet.evalFloat(color[2]);
	} else if (color.size() == 1) {
		decodeRGB(ruleSet.evalString(color[0]), obj->_color.r, obj->_color.g, obj->_color.b);
	}

	return obj;
}

void ColorOperator::decodeRGB(const std::string& str, float& r, float& g, float& b) {
	int ir, ig, ib;
    std::istringstream(str.substr(1, 2)) >> std::hex >> ir;
    std::istringstream(str.substr(3, 2)) >> std::hex >> ig;
    std::istringstream(str.substr(5, 2)) >> std::hex >> ib;

	r = (float)ir / 255;
	g = (float)ig / 255;
	b = (float)ib / 255;
}

}
