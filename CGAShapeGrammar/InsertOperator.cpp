#include "InsertOperator.h"
#include "CGA.h"

namespace cga {

InsertOperator::InsertOperator(const std::string& geometryPath) {
	this->geometryPath = geometryPath;
}

Object* InsertOperator::apply(Object* obj, std::list<Object*>& stack) {
	return obj->insert(obj->_name, geometryPath);
}

}
