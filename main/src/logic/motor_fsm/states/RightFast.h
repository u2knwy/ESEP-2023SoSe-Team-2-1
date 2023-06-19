/*
 * RightFast.h
 *
 *  Created on: 09.06.2023
 *      Author: Maik
 */
#pragma once

#include "../MotorBasestate.h"

class RightFast : public MotorBasestate {
    void entry() override;
    void exit() override;
    MotorState getCurrentState() override;
    bool handleFlagsUpdated() override;
};
