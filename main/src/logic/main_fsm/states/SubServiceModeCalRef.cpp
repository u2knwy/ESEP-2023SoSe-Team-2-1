/*
 * SubServiceModeCalRef.cpp
 *
 *  Created on: 06.06.2023
 *      Author: Maik
 */

#include "SubServiceModeCalRef.h"
#include "SubServiceModeTestsFailed.h"
#include "SubServiceModeSelftestSensors.h"
#include "logger/logger.hpp"

#include <iostream>

void SubServiceModeCalRef::entry() {
	Logger::info("[ServiceMode] Calibrating HeightSensor reference (High)");
	Logger::info("Place a high workpiece below the HeightSensor and press START to calibrate reference.");
	Logger::info("Press RESET button to continue");
	done = false;
}

void SubServiceModeCalRef::exit() {
}

bool SubServiceModeCalRef::master_btnStart_PressedShort() {
	actions->calibrateReference();
	done = true;
	return true;
}

bool SubServiceModeCalRef::master_btnReset_Pressed() {
	if(done) {
		actions->saveCalibration();
		exit();
		new(this) SubServiceModeSelftestSensors;
		entry();
		return true;
	} else {
		return false;
	}
}

bool SubServiceModeCalRef::slave_btnStart_PressedShort() {
	actions->calibrateReference();
	done = true;
	return true;
}

bool SubServiceModeCalRef::slave_btnReset_Pressed() {
	if(done) {
		actions->saveCalibration();
		exit();
		new(this) SubServiceModeSelftestSensors;
		entry();
		return true;
	} else {
		return false;
	}
}
