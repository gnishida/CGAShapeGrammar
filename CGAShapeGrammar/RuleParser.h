#pragma once

#include <map>
#include "Rule.h"
#include <QFile>
#include <QDomDocument>
#include <QDomNode>

namespace cga {

std::map<std::string, cga::Rule*> parseRule(char* filename);
Rule* parseCompRule(const QDomNode& node);
Rule* parseCopyRule(const QDomNode& node);
Rule* parseExtrudeRule(const QDomNode& node);
Rule* parseOffsetRule(const QDomNode& node);
Rule* parseRoofHipRule(const QDomNode& node);
Rule* parseSetupProjectionRule(const QDomNode& node);
Rule* parseSplitRule(const QDomNode& node);
Rule* parseTaperRule(const QDomNode& node);
Rule* parseTextureRule(const QDomNode& node);
Rule* parseTranslateRule(const QDomNode& node);

}