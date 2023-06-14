/*
 * SubErrorSolvedUnresigned.cpp
 *
 *  Created on: 14.06.2023
 *      Author: Maik
 */

#include "SubErrorSolvedUnresigned.h"
#include "SubErrorEndState.h"
#include "logger/logger.hpp"

void SubErrorSolvedUnresigned::entry(){
	actions->redLampFlashingSlow();
	Logger::info("Solved unresigned - Push the Reset button");
}

void SubErrorSolvedUnresigned::exit(){

}

bool SubErrorSolvedUnresigned::master_btnReset_Pressed(){
	exit();
	new(this) SubErrorEndState;
	entry();
	return true;
}

bool SubErrorSolvedUnresigned::slave_btnReset_Pressed(){
	exit();
	new(this) SubErrorEndState;
	entry();
	return true;
}
