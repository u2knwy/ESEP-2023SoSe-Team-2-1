/*
 * SubEStopTwoPressed.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "SubEStopBothReleased.h"
#include "Standby.h"
#include "SubEStopEndState.h"
#include "SubEStopOnePressed.h"
#include "logger/logger.hpp"


#include <iostream>

void SubEStopBothReleased::entry() {
    Logger::debug("[EStop] Both released");
    Logger::user_info("Both E-Stop release - Press Reset button at Master and "
                      "Slave to leave EStop mode");
    masterReset = false;
    slaveReset = false;
    actions->master_btnResetLedOn();
    actions->slave_btnResetLedOn();
}

void SubEStopBothReleased::exit() {}

bool SubEStopBothReleased::master_btnReset_Pressed() {
    Logger::debug("[EStop] Master confirmed reset");
    this->masterReset = true;
    actions->master_btnResetLedOff();
    if (masterReset && slaveReset) {
        Logger::debug("EStop was resetted -> leave EStop mode");
        exit();
        new (this) SubEStopEndState;
        entry();
        return true;
    }
    return false;
}

bool SubEStopBothReleased::slave_btnReset_Pressed() {
    Logger::debug("[EStop] Slave confirmed reset");
    this->slaveReset = true;
    actions->slave_btnResetLedOff();
    if (masterReset && slaveReset) {
        Logger::debug("EStop was resetted -> leave EStop mode");
        exit();
        new (this) SubEStopEndState;
        entry();
        return true;
    }
    return false;
}

bool SubEStopBothReleased::master_EStop_Pressed() {
    exit();
    new (this) SubEStopOnePressed;
    entry();
    return true;
}

bool SubEStopBothReleased::slave_EStop_Pressed() {
    exit();
    new (this) SubEStopOnePressed;
    entry();
    return true;
}
