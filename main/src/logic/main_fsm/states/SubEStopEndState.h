/*
 * SubEStopEndState.h
 *
 *  Created on: 25.05.2023
 *      Author: Maik
 */
#pragma once

#include "../MainBasestate.h"

class SubEStopEndState : public MainBasestate {
    bool isSubEndState() override { return true; }
};
