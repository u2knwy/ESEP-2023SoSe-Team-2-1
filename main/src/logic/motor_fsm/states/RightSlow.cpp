/*
 * RightSlow.cpp
 *
 *  Created on: 09.06.2023
 *      Author: Maik
 */

#include "RightSlow.h"
#include "RightFast.h"
#include "Stopped.h"
#include "logger/logger.hpp"


void RightSlow::entry() { actions->motorRightSlow(); }

void RightSlow::exit() {}

MotorState RightSlow::getCurrentState() { return MotorState::RIGHT_SLOW; };

bool RightSlow::handleFlagsUpdated() {
    if (!data->getStop() && data->getRight() && !data->getSlow()) {
        exit();
        new (this) RightFast;
        entry();
        return true;
    } else if (data->getStop() || !data->getRight()) {
        exit();
        new (this) Stopped;
        entry();
        return true;
    }
    return true;
}
