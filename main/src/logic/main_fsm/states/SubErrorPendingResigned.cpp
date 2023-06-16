/*
 * SubErrorPendingResigned.cpp
 *
 *  Created on: 14.06.2023
 *      Author: Maik
 */

#include "SubErrorPendingResigned.h"
#include "SubErrorEndState.h"
#include "logger/logger.hpp"

void SubErrorPendingResigned::entry(){
	Logger::user_info("Pending error was resigned - Press Start button to leave Error mode");
	actions->redLampFlashingSlow();
}

void SubErrorPendingResigned::exit(){

}

bool SubErrorPendingResigned::master_btnStart_PressedShort() {
	exit();
	new(this) SubErrorEndState;
	entry();
	return true;
}

bool SubErrorPendingResigned::slave_btnStart_PressedShort() {
	exit();
	new(this) SubErrorEndState;
	entry();
	return true;
}
