/*
 * WaitForBelt.h
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#pragma once

#include "../HeightBasestate.h"

class WaitForBelt : public HeightBasestate {
	void entry() override;
	bool heightValueReceived(float valueMM) override;
};
