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
	void exit() override;

	bool flatDetected() override;
	bool highDetected() override;
};

