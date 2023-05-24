/*
 * Standby.h
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#pragma once

#include "../MainBasestate.h"

class Standby: public MainBasestate {
	void entry() override;
	void exit() override;

	bool master_btnStart_PressedShort() override;
	bool master_btnStart_PressedLong() override;
	bool master_EStop_Pressed() override;

	bool slave_btnStart_PressedShort() override;
	bool slave_btnStart_PressedLong() override;
	bool slave_EStop_Pressed() override;
};
