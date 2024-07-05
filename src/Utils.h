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
 std::optional<bool> generateProblem(rai::Configuration& C, int environmentType, int taskSpecific, int numObj, int task);
 std::optional<std::string> initializeFol(const std::string& rootPath, const std::string& testName, const std::string& folFileName, int targetCount, int task);
 std::string trayCapacityRule(int objectCount);
 std::string fillPitcherRule();
 std::string problem(int objectCount, int environmentType, int task);

} // namespace utils

#endif // CA_TAMP_UTILS_H
