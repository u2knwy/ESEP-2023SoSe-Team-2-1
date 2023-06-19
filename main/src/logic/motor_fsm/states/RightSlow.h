/*
 * RightSlow.h
 *
 *  Created on: 09.06.2023
 *      Author: Maik
 */
#pragma once

#include "../MotorBasestate.h"

class RightSlow : public MotorBasestate {
    void entry() override;
    void exit() override;
    MotorState getCurrentState() override;
    bool handleFlagsUpdated() override;
};
