#include "SetupProjectionOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

SetupProjectionOperator::SetupProjectionOperator(int type, float texWidth, float texHeight) {
	this->type = type;
	this->texWidth = texWidth;
	this->texHeight = texHeight;
}

/*SetupProjectionRule::SetupProjectionRule(float u1, float v1, float u2, float v2, const std::string& output_name) {
	this->u1 = u1;
	this->v1 = v1;
	this->u2 = u2;
	this->v2 = v2;
	this->output_name = output_name;
}*/

Shape* SetupProjectionOperator::apply(Shape* obj, const RuleSet& ruleSet, std::list<Shape*>& stack) {
	if (type == TYPE_RELATIVE) {
		obj->setupProjection(obj->_scope.x, obj->_scope.y);
	} else {
		obj->setupProjection(texWidth, texHeight);
	}

	return obj;
}

}
