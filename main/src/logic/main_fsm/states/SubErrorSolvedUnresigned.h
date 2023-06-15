/*
 * SubErrorSolvedUnresigned.h
 *
 *  Created on: 14.06.2023
 *      Author: Maik
 */
#pragma once

#include "../MainBasestate.h"

class SubErrorSolvedUnresigned: public MainBasestate {
	void entry() override;
	void exit() override;

	bool master_btnReset_Pressed() override;
	bool slave_btnReset_Pressed() override;
};
