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
    else if (affordanceType == "PourAffordable") {
        rule << "DecisionRule pourAffordable" << " {\n";
        rule << "  X, Q, Y, Z,\n";
        rule << "  { (gripper_support X) (gripper Q) (glass Y) (jug Z) (filled Z) (picked Q Z) (reached X Y) (held Z) (empty Y) }\n";
        rule << "  { (busy X)! (touch X Y)!\n";
        rule << "    (filled Y)\n";
        rule << "    tmp(touch X Y) tmp(touch Z Y)\n";
        rule << "    }\n";
        rule << "}\n";
        rule << "\n";
        rule << "#####################################################################";
    }
    else if (affordanceType == "HandOverAffordable") {
        rule << "DecisionRule handoverPick" << " {\n";
        rule << "  X, Y, Z\n";
        rule << "  { (helper_gripper X) (object Y) (helper_zone Z)  (on Z Y) (busy X)! (held Y)! }\n";
        rule << "  { (above Y ANY)! (on ANY Y)! (stableOn ANY Y)!\n";
        rule << "    (picked X Y) (held Y) (busy X)\n";
        rule << "    (touch X Y) (stable X Y)\n";
        rule << "    }\n";
        rule << "}\n";
        rule << "\n";
        rule << "#####################################################################";
    }
    return rule.str();
}