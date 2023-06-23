/*
 * RightFast.cpp
 *
 *  Created on: 09.06.2023
 *      Author: Maik
 */

#include "RightFast.h"
#include "RightSlow.h"
#include "Stopped.h"
#include "logger/logger.hpp"


void RightFast::entry() { actions->motorRightFast(); }

void RightFast::exit() {}

MotorState RightFast::getCurrentState() { return MotorState::RIGHT_FAST; };

bool RightFast::handleFlagsUpdated() {
    std::stringstream ss;
    if (data->getStop() || !data->getRight()) {
        exit();
        new (this) Stopped;
        entry();
        return true;
    } else if (!data->getStop() && data->getRight() && data->getSlow()) {
        exit();
        new (this) RightSlow;
        entry();
        return true;
    }
    return false;
}
