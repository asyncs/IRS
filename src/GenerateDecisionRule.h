// Purpose: Header file for GenerateDecisionRule class.
// Author: Arda Sarp Yenicesu.
// Date: 2023/11/17

#ifndef CA_TAMP_GENERATEDECISIONRULE_H
#define CA_TAMP_GENERATEDECISIONRULE_H

#include <string>

/**
 * @brief This class is used to generate a decision rule for a given affordance type and target count
 */
class GenerateDecisionRule {
public:
    /**
     * @brief Constructor
     */
    GenerateDecisionRule();

    /**
     * @brief Gets the decision rule for a given affordance type and target count
     * @param affordanceType The affordance type
     * @param targetCount The target count
     * @return The generated decision rule
     */
    static std::string getDecisionRule(const std::string &affordanceType, int targetCount);

private:
    /**
     * @brief Generates a decision rule for a given affordance type and target count
     * @param affordanceType The affordance type
     * @param targetCount The target count
     * @return The generated decision rule
     */
    static std::string generateRule(const std::string &affordanceType, int targetCount);
};


#endif //CA_TAMP_GENERATEDECISIONRULE_H
