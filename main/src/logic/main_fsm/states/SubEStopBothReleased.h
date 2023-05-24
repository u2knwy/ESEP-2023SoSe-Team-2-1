/*
 * SubEStopTwoPressed.h
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */
#pragma once

#include "../MainBasestate.h"

class SubEStopBothReleased: public MainBasestate {
	void entry() override;
	bool master_btnReset_Pressed() override;
	bool slave_btnReset_Pressed() override;

	bool master_EStop_Pressed() override;
	bool slave_EStop_Pressed() override;
private:
	bool masterReset{false};
	bool slaveReset{false};
};
