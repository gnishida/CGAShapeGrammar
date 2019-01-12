#pragma once

#include "Operator.h"
#include "Vertex.h"
#include <string>
#include <unordered_map>
#include <queue>

namespace procedural_generation {

	class ShapeGrammar {
	public:
		ShapeGrammar() {}
		~ShapeGrammar() {}

		void setAxiom(std::shared_ptr<Shape> axiom);
		void addRule(const std::string& ruleName, std::shared_ptr<Operator> productionRule);
		void derive(Faces& faces, bool suppressWarning = true);

		void setSimpleBuildingRules(float height);
		void setBuildingRules(float height);

	private:
		std::unordered_map<std::string, std::shared_ptr<Operator>> _productionRles;
		std::queue<std::shared_ptr<Shape>> _derivationQueue;

	};

}