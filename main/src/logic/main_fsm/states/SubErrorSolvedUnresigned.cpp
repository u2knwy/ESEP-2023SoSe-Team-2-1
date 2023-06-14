/*
 * SubErrorSolvedUnresigned.cpp
 *
 *  Created on: 14.06.2023
 *      Author: Maik
 */

#include "SubErrorSolvedUnresigned.h"
#include "logger/logger.hpp"

void SubErrorSolvedUnresigned::entry(){
	actions->redLampFlashingSlow();
	Logger::info("Solved unresigned - Push the Reset button");
}

void SubErrorSolvedUnresigned::exit(){

}

bool SubErrorSolvedUnresigned::selfSolvableErrorOccurred(){
	return true;
}

bool SubErrorSolvedUnresigned::nonSelfSolvableErrorOccurred(){
	return true;
}

bool SubErrorSolvedUnresigned::errorSelfSolved(){
	return true;
}

bool SubErrorSolvedUnresigned::master_btnReset_Pressed(){
	return true;
}

bool SubErrorSolvedUnresigned::slave_btnReset_Pressed(){
	return true;
}

bool SubErrorSolvedUnresigned::master_btnStart_PressedShort(){
	return true;
}

bool SubErrorSolvedUnresigned::slave_btnStart_PressedShort(){
	return true;
}
