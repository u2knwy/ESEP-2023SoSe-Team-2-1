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

    HeightState getCurrentState() override;

    bool beltDetected() override;

    bool workpieceHeightDetected(float height) override;
};
