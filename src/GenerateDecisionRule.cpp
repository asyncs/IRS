//
// Created by asy on 17.11.2023.
//

#include "GenerateDecisionRule.h"

GenerateDecisionRule::GenerateDecisionRule() = default;

std::string GenerateDecisionRule::getDecisionRule(const std::string& affordanceType, int targetCount) {
    return generateRule(affordanceType, targetCount);
}
// todo generate a rule that can clear the tray after carry operation so that it can be used multiple times, or make an assumption that object affordance is unlimited.
std::string GenerateDecisionRule::generateRule(const std::string& affordanceType, int targetCount) {
    std::string rule;

    if (affordanceType == "TransportAffordable"){
        rule += "DecisionRule ";
        rule += "cary";
        rule += affordanceType;
        rule += " {\n"
                "  X, Y, Z";
        for (int i=0; i<targetCount; i++){
            rule += ", O" + std::to_string(i+1);
        }
        rule += ",\n"
                "  { (picked X Y) (table Z) (held Y)";
        for (int i=0; i<targetCount; i++){
            rule += " (on Y O" + std::to_string(i+1) + ")";
        }
        rule += "}\n"
                "  { (picked X Y)! (busy X)! (busy Y)! (held Y)! # logic only\n"
                "    (stable ANY Y)! (touch X Y)! # NLP predicates\n"
                "    (on Z Y) (above Y Z) (stableOn Z Y) tmp(touch X Y) tmp(touch Y Z)";
        for (int i=0; i<targetCount; i++){
            rule += " (on Z O" + std::to_string(i+1) + ")";
        }
        rule += "\n"
                "    }\n"
                "}\n"
                "\n"
                "#####################################################################";

    }
    return rule;
}

