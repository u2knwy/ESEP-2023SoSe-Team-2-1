/*
 * SubErrorSolvedUnresigned.cpp
 *
 *  Created on: 14.06.2023
 *      Author: Maik
 */

#include "SubErrorSolvedUnresigned.h"
#include "SubErrorEndState.h"
#include "logger/logger.hpp"

void SubErrorSolvedUnresigned::entry() {
    actions->redLampFlashingSlow();
    Logger::user_info("Error has solved itself - Push the Reset button to "
                      "resign it and leave Error mode");
    actions->master_btnResetLedOn();
    actions->slave_btnResetLedOn();
}

void SubErrorSolvedUnresigned::exit() {}

bool SubErrorSolvedUnresigned::master_btnReset_Pressed() {
    exit();
    new (this) SubErrorEndState;
    entry();
    return true;
}

bool SubErrorSolvedUnresigned::slave_btnReset_Pressed() {
    exit();
    new (this) SubErrorEndState;
    entry();
    return true;
}
