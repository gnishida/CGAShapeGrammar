#include "CGA.h"
#include "GLUtils.h"
#include "OBJLoader.h"
#include <map>
#include <iostream>

namespace cga {

CGA::CGA() {
}

void CGA::generate(RenderManager* renderManager, const RuleSet& ruleSet, std::list<boost::shared_ptr<Shape> >& stack, bool showScopeCoordinateSystem) {
	while (!stack.empty()) {
		boost::shared_ptr<Shape> shape = stack.front();
		stack.pop_front();

		if (ruleSet.contain(shape->_name)) {
			ruleSet.getRule(shape->_name).apply(shape, ruleSet, stack);
		} else {
			if (shape->_name.back() != '!' && shape->_name.back() != '.') {
				std::cout << "Warning: " << "no rule is found for " << shape->_name << "." << std::endl;
			}
			shape->generate(renderManager, showScopeCoordinateSystem);
		}
	}
}

}
