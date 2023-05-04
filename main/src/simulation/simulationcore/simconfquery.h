/*
 * File:   simctrlaction.h
 * @author Lehmann
 * @date 05. Jan 2023
 */

#include "simconf.h"

#ifndef SIMCONFQUERRY
#undef SIMCONF
#define SIMCONF(Y) SimConfCodes::Y
#define SIMCONFQUERRY_ISACTIVE(X) SimConfiguration::getInstance()->isactive(SimConfCodes::X)
#define SIMCONF_ACTIVATE(X) SimConfiguration::getInstance()->activate(X)
#define SIMCONF_DEACTIVATE(X) SimConfiguration::getInstance()->deactivate(X)
#endif
