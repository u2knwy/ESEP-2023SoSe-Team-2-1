/*
 * SubErrorEndState.h
 *
 *  Created on: 14.06.2023
 *      Author: Maik
 */
#pragma once

#include "../MainBasestate.h"

class SubErrorEndState: public MainBasestate {
	bool isSubEndState() override {
		return true;
	}

};
