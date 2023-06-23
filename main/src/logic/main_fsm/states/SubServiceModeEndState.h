/*
 * SubServiceModeEndState.h
 *
 *  Created on: 06.06.2023
 *      Author: Maik
 */
#pragma once

#include "../MainBasestate.h"

class SubServiceModeEndState : public MainBasestate {
    bool isSubEndState() override { return true; }
};
