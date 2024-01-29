// Purpose: Header filer for utils.
// Author: Arda Sarp Yenicesu.
// Date: 2023/12/01

#ifndef CA_TAMP_UTILS_H
#define CA_TAMP_UTILS_H

#include <iostream>
#include <Kin/kin.h>
#include <Gui/opengl.h>
#include <Core/graph.h>
#include <Kin/proxy.h>
#include <Kin/viewer.h>
#include <KOMO/komo.h>
#include "../../src/GenericFolFile.h"
#include "../../src/GenerateDecisionRule.h"

/**
 * @brief This function is used to initialize the environment for the pick and place problem
 * @param kin The kinematic world
 * @param environmentType The type of the environment
 */
bool generateProblemPNP(rai::Configuration &C, int environmentType, int numObj);
/**
 * @brief This function is used for initializing the first order logic file
 * @param rootPath Root path of the project
 * @param testName Test name, e.g. pickAndPlace
 * @param folFileName Any first order logic file name, e.g. fol-pnp-switch.g
 */
std::string initializeFol(const std::string& rootPath, const std::string& testName,const std::string& folFileName, int targetCount);

/**
 * @brief This function is used to generate the capacity rule for the first order logic file
 * @param objectCount Number of objects to be transported
 */
std::string capacityRule(int objectCount);

/**
 * @brief This function is used to generate the terminal rule for the first order logic file
 * @param objectCount Number of objects to be transported
 * @param environmentType The type of the environment
 */
std::string problem(int objectCount, int environmentType);

#endif //CA_TAMP_UTILS_H
