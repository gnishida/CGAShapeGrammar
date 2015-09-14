#include "ColorOperator.h"
#include "CGA.h"
#include "Shape.h"
#include <sstream>

namespace cga {

ColorOperator::ColorOperator(const std::string& r, const std::string& g, const std::string& b) {
	this->name = "color";
	this->r = r;
	this->g = g;
	this->b = b;
	this->s = "";
}

ColorOperator::ColorOperator(const std::string& s) {
	this->name = "color";
	this->r = "";
	this->g = "";
	this->b = "";
	this->s = s;
}

Shape* ColorOperator::apply(Shape* shape, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	if (s.empty()) {
		shape->_color.r = ruleSet.evalFloat(r, shape);
		shape->_color.g = ruleSet.evalFloat(g, shape);
		shape->_color.b = ruleSet.evalFloat(b, shape);
	} else {
		decodeRGB(ruleSet.evalString(s, shape), shape->_color.r, shape->_color.g, shape->_color.b);
	}

	return shape;
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
