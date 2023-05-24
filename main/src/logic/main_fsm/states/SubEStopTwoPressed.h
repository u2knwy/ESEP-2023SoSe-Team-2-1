/*
 * SubEStopTwoPressed.h
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */
#pragma once

#include "../MainBasestate.h"

class SubEStopTwoPressed: public MainBasestate {
	bool master_EStop_Released() override;
	bool slave_EStop_Released() override;
};
