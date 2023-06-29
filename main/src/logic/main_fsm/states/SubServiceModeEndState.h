/*
 * SubServiceModeEndState.h
 *
 *  Created on: 06.06.2023
 *      Author: Maik
 */
#pragma once

#include "../MainBasestate.h"

class SubServiceModeEndState : public MainBasestate {
	void entry() override {
	    Logger::user_info("[ServiceMode] All tests pass! Next: Standby mode.");
	}

    bool isSubEndState() override { return true; }
};
