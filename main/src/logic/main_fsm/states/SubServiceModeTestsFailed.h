/*
 * SubServiceModeTestsFailed.h
 *
 *  Created on: 06.06.2023
 *      Author: Maik
 */
#pragma once

#include "../MainBasestate.h"

class SubServiceModeTestsFailed: public MainBasestate {
	void entry() override;
	void exit() override;

	bool isSubEndState() override;

	bool master_btnStop_Pressed() override;
	bool slave_btnStop_Pressed() override;
};
