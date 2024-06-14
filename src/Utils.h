#ifndef CA_TAMP_UTILS_H
#define CA_TAMP_UTILS_H

#include <iostream>
#include <string>
#include <Kin/kin.h>
#include <Gui/opengl.h>
#include <Core/graph.h>
#include <Kin/proxy.h>
#include <Kin/viewer.h>
#include <KOMO/komo.h>
#include <optional>
#include "../../src/GenericFolFile.h"
#include "../../src/GenerateDecisionRule.h"

namespace utils {
 std::optional<bool> generateProblemPNP(rai::Configuration& C, int environmentType, int numObj);
 std::optional<std::string> initializeFol(const std::string& rootPath, const std::string& testName, const std::string& folFileName, int targetCount);
 std::string capacityRule(int objectCount);
 std::string problem(int objectCount, int environmentType);

} // namespace utils

#endif // CA_TAMP_UTILS_H
