// Purpose: Source file for GenerateDecisionRule class
// Author: Arda Sarp Yenicesu.
// Date: 2023/11/17

#include "GenerateDecisionRule.h"

GenerateDecisionRule::GenerateDecisionRule() = default;

std::string GenerateDecisionRule::getDecisionRule(const std::string &affordanceType, int targetCount) {
    return generateRule(affordanceType, targetCount);
}

std::string GenerateDecisionRule::generateRule(const std::string &affordanceType, int targetCount) {
    std::string rule;
    // Decision rule for the "TransportAffordable" affordance type
    // This affordance type represents the affordance of transporting group of objects from one place to another using an object like a tray
    if (affordanceType == "TransportAffordable") {
        rule += "DecisionRule ";
        rule += "carry";
        rule += affordanceType;
        rule += " {\n"
                "  X, Y, Z";
        for (int i = 0; i < targetCount; i++) {
            rule += ", O" + std::to_string(i + 1);
        }
        rule += ",\n"
                "  { (picked X Y) (table Z) (held Y)";
        for (int i = 0; i < targetCount; i++) {
            rule += " (on Y O" + std::to_string(i + 1) + ")";
        }
        rule += "}\n"
                "  { (picked X Y)! (busy X)! (busy Y)! (held Y)! # logic only\n"
                "    (stable ANY Y)! (touch X Y)! # NLP predicates\n"
                "    (on Z Y) (above Y Z) (stableOn Z Y) tmp(touch X Y) tmp(touch Y Z)";
        for (int i = 0; i < targetCount; i++) {
            rule += " (on Z O" + std::to_string(i + 1) + ")";
        }
        rule += "\n"
                "    }\n"
                "}\n"
                "\n"
                "#####################################################################";
    }
    return rule;
}

