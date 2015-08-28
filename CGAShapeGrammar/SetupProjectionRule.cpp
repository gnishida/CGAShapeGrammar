#include "SetupProjectionRule.h"
#include "CGA.h"

namespace cga {

SetupProjectionRule::SetupProjectionRule(int type, float texWidth, float texHeight, const std::string& output_name) {
	this->type = type;
	this->texWidth = texWidth;
	this->texHeight = texHeight;
	this->output_name = output_name;
}

/*SetupProjectionRule::SetupProjectionRule(float u1, float v1, float u2, float v2, const std::string& output_name) {
	this->u1 = u1;
	this->v1 = v1;
	this->u2 = u2;
	this->v2 = v2;
	this->output_name = output_name;
}*/

void SetupProjectionRule::apply(Object* obj, std::list<Object*>& stack) {
	Object *obj2 = obj->clone();
	obj2->_name = output_name;

	if (type == TYPE_RELATIVE) {
		obj2->setupProjection(obj->_scope.x, obj->_scope.y);
	} else {
		obj2->setupProjection(texWidth, texHeight);
	}
	stack.push_back(obj2);
}

}
