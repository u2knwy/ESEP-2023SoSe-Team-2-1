/*
 * SubEStopOnePressed.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "SubEStopOnePressed.h"
#include "SubEStopBothReleased.h"
#include "SubEStopTwoPressed.h"
#include "logger/logger.hpp"


#include <iostream>

void SubEStopOnePressed::entry() {
	Logger::info("One E-Stop is pressed");
    actions->master_btnResetLedOff();
    actions->slave_btnResetLedOff();
}

void SubEStopOnePressed::exit() {}

bool SubEStopOnePressed::master_EStop_Pressed() {
    exit();
    new (this) SubEStopTwoPressed;
    entry();
    return true;
}

bool SubEStopOnePressed::master_EStop_Released() {
    exit();
    new (this) SubEStopBothReleased;
    entry();
    return true;
}

bool SubEStopOnePressed::slave_EStop_Pressed() {
    exit();
    new (this) SubEStopTwoPressed;
    entry();
    return true;
}

bool SubEStopOnePressed::slave_EStop_Released() {
    exit();
    new (this) SubEStopBothReleased;
    entry();
    return true;
}
