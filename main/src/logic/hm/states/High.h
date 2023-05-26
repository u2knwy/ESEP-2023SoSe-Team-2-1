/*
 * High.h
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#pragma once

#include "../HeightBasestate.h"

class High : public HeightBasestate {
	void entry() override;

	HeightState getCurrentState() override;

	bool beltDetected() override;
	bool holeDetected() override;
	bool unknownDetected() override;
};
