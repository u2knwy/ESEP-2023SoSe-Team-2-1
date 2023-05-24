/*
 * EStop.h
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */
#pragma once

#include "../MainBasestate.h"

class EStop: public MainBasestate {
	void entry() override;
	void exit() override;

	bool master_EStop_Pressed() override;
	bool master_EStop_Released() override;

	bool slave_EStop_Pressed() override;
	bool slave_EStop_Released() override;

	bool master_btnReset_Pressed() override;
	bool slave_btnReset_Pressed() override;
};
