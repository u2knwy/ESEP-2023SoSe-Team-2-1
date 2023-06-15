/*
 * SubErrorPendingResigned.h
 *
 *  Created on: 14.06.2023
 *      Author: Maik
 */
#pragma once

#include "../MainBasestate.h"

class SubErrorPendingResigned: public MainBasestate {
	void entry() override;
	void exit() override;

	bool master_btnStart_PressedShort() override;
	bool slave_btnStart_PressedShort() override;
};
