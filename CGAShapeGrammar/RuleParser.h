#pragma once

#include <map>
#include "Rule.h"
#include <QFile>
#include <QDomDocument>
#include <QDomNode>

namespace cga {

void parseRule(char* filename, RuleSet& ruleSet);
Operator* parseCenterOperator(const QDomNode& node);
Operator* parseColorOperator(const QDomNode& node);
Operator* parseCompOperator(const QDomNode& node);
Operator* parseCopyOperator(const QDomNode& node);
Operator* parseExtrudeOperator(const QDomNode& node);
Operator* parseInsertOperator(const QDomNode& node);
Operator* parseOffsetOperator(const QDomNode& node);
Operator* parseRoofGableOperator(const QDomNode& node);
Operator* parseRoofHipOperator(const QDomNode& node);
Operator* parseRotateOperator(const QDomNode& node);
Operator* parseSetupProjectionOperator(const QDomNode& node);
Operator* parseShapeLOperator(const QDomNode& node);
Operator* parseSizeOperator(const QDomNode& node);
Operator* parseSplitOperator(const QDomNode& node);
Operator* parseTaperOperator(const QDomNode& node);
Operator* parseTextureOperator(const QDomNode& node);
Operator* parseTranslateOperator(const QDomNode& node);

}