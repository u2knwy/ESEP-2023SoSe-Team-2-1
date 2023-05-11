/*
 * WaitForWorkpiece.h
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#pragma once

#include "../HeightBasestate.h"

class WaitForWorkpiece : public HeightBasestate {
	void entry() override;
	bool heightValueReceived(int value) override;
};

