#include "TaperOperator.h"
#include "CGA.h"

namespace cga {

TaperOperator::TaperOperator(float height, float top_ratio) {
	this->height = height;
	this->top_ratio = top_ratio;
}

Object* TaperOperator::apply(Object* obj, std::list<Object*>& stack) {
	Object* new_obj = obj->taper(obj->_name, height, top_ratio);
	delete obj;
	return new_obj;
}

}
