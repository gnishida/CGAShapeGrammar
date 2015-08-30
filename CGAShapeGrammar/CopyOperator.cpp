#include "CopyOperator.h"
#include "CGA.h"

namespace cga {

CopyOperator::CopyOperator(const std::string& copy_name) {
	this->copy_name = copy_name;
}

Object* CopyOperator::apply(Object* obj, std::list<Object*>& stack) {
	Object* copy = obj->clone(copy_name);
	stack.push_back(copy);

	return obj->clone(obj->_name);
}

}
