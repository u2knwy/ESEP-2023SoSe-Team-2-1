/*
 * SubServiceModeCalOffset.cpp
 *
 *  Created on: 06.06.2023
 *      Author: Maik
 */

#include "SubServiceModeCalOffset.h"
#include "SubServiceModeCalRef.h"
#include "SubServiceModeTestsFailed.h"
#include "logger/logger.hpp"

#include <iostream>

void SubServiceModeCalOffset::entry() {
	done = false;
	Logger::debug("SubServiceModeCalOffset::entry");
	Logger::info("Make sure no workpiece is below the HeightSensor and press START to calibrate offset.");
	Logger::info("Press RESET button to continue");
}

void SubServiceModeCalOffset::exit() {
	Logger::debug("SubServiceModeCalOffset::exit");
}

bool SubServiceModeCalOffset::master_btnStart_PressedShort() {
	actions->calibrateOffset();
	done = true;
	return true;
}

bool SubServiceModeCalOffset::master_btnReset_Pressed() {
	if(done) {
		exit();
		new(this) SubServiceModeCalRef;
		entry();
		return true;
	} else {
		return false;
	}
}

bool SubServiceModeCalOffset::slave_btnStart_PressedShort() {
	actions->calibrateOffset();
	done = true;
	return true;
}

bool SubServiceModeCalOffset::slave_btnReset_Pressed() {
	if(done) {
		exit();
		new(this) SubServiceModeCalRef;
		entry();
		return true;
	} else {
		return false;
	}
}
