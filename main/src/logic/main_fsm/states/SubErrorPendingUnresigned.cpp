/*
 * SubErrorPendingUnresigned.cpp
 *
 *  Created on: 14.06.2023
 *      Author: Maik
 */

#include "SubErrorPendingUnresigned.h"
#include "SubErrorSolvedUnresigned.h"
#include "SubErrorPendingResigned.h"
#include "logger/logger.hpp"

void SubErrorPendingUnresigned::entry(){
	actions->redLampFlashingFast();
}

void SubErrorPendingUnresigned::exit(){

}

bool SubErrorPendingUnresigned::selfSolvableErrorOccurred(){
	Logger::info("Self-solving error pending - Wait until it solves itself");
	selfSolving = true;
	return true;
}

bool SubErrorPendingUnresigned::nonSelfSolvableErrorOccurred(){
	Logger::user_info("Error pending - Press Reset button to resign it");
	manualSolving = true;
	return true;
}

bool SubErrorPendingUnresigned::errorSelfSolved(){
	selfSolving = false;
	if(!manualSolving) {
		exit();
		new(this) SubErrorSolvedUnresigned;
		entry();
		return true;
	}
	return false;
}

bool SubErrorPendingUnresigned::master_btnReset_Pressed(){
	if(!selfSolving) {
		exit();
		new(this) SubErrorPendingResigned;
		entry();
		return true;
	}
	return false;
}

bool SubErrorPendingUnresigned::slave_btnReset_Pressed(){
	if(!selfSolving) {
		exit();
		new(this) SubErrorPendingResigned;
		entry();
		return true;
	}
	return false;
}
