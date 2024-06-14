#ifndef CA_TAMP_GENERATEDECISIONRULE_H
#define CA_TAMP_GENERATEDECISIONRULE_H

#include <string>

class GenerateDecisionRule {
public:
 GenerateDecisionRule() = default;
 static std::string getDecisionRule(const std::string& affordanceType, int targetCount);

private:
 static std::string generateRule(const std::string& affordanceType, int targetCount);
};

#endif // CA_TAMP_GENERATEDECISIONRULE_H
