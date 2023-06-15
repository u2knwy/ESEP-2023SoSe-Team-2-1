/*
 * SubErrorPendingUnresigned.cpp
 *
 *  Created on: 14.06.2023
 *      Author: Maik
 */

#include "SubErrorPendingUnresigned.h"
#include "SubErrorSolvedUnresigned.h"
#include "SubErrorPendingResigned.h"

void SubErrorPendingUnresigned::entry(){
	actions->redLampFlashingFast();
}

void SubErrorPendingUnresigned::exit(){

}

bool SubErrorPendingUnresigned::selfSolvableErrorOccurred(){
	selfSolving = true;
	return true;
}

bool SubErrorPendingUnresigned::nonSelfSolvableErrorOccurred(){
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
