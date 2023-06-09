/*
 * Stopped.cpp
 *
 *  Created on: 09.06.2023
 *      Author: Maik
 */

#include "Stopped.h"
#include "RightFast.h"
#include "RightSlow.h"
#include "logger/logger.hpp"

void Stopped::entry() { actions->motorStop(); }

void Stopped::exit() {}

MotorState Stopped::getCurrentState() { return MotorState::STOPPED; };

bool Stopped::handleFlagsUpdated() {
    if (!data->getStop() && data->getRight() && !data->getSlow()) {
        exit();
        new (this) RightFast;
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
