#include "GenerateDecisionRule.h"
#include <sstream>

std::string GenerateDecisionRule::getDecisionRule(const std::string& affordanceType, int targetCount) {
    return generateRule(affordanceType, targetCount);
}

std::string GenerateDecisionRule::generateRule(const std::string& affordanceType, int targetCount) {
    std::ostringstream rule;

    if (affordanceType == "TransportAffordable") {
        rule << "DecisionRule carry" << affordanceType << " {\n";
        rule << "  X, Y, Z";
        for (int i = 0; i < targetCount; ++i) {
            rule << ", O" << (i + 1);
        }
        rule << ",\n";
        rule << "  { (picked X Y) (table Z) (held Y)";
        for (int i = 0; i < targetCount; ++i) {
            rule << " (on Y O" << (i + 1) << ")";
        }
        rule << " }\n";
        rule << "  { (picked X Y)! (busy X)! (busy Y)! (held Y)! # logic only\n";
        rule << "    (stable ANY Y)! (touch X Y)! # NLP predicates\n";
        rule << "    (on Z Y) (above Y Z) (stableOn Z Y) tmp(touch X Y) tmp(touch Y Z)";
        for (int i = 0; i < targetCount; ++i) {
            rule << " (on Z O" << (i + 1) << ")";
        }
        rule << "\n";
        rule << "    }\n";
        rule << "}\n";
        rule << "\n";
        rule << "#####################################################################";
    }

    return rule.str();
}
