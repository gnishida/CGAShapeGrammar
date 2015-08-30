#include "InsertOperator.h"
#include "CGA.h"

namespace cga {

InsertOperator::InsertOperator(const std::string& geometryPath) {
	this->geometryPath = geometryPath;
}

Shape* InsertOperator::apply(Shape* obj, std::list<Shape*>& stack) {
	return obj->insert(obj->_name, geometryPath);
}

}
