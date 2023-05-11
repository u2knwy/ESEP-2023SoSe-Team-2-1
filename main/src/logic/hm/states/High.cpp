/*
 * WaitForWorkpiece.cpp
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#include "hal/HeightSensor.h"
#include "WaitForWorkpiece.h"
#include "WaitForBelt.h"
#include "High.h"

void High::entry() {
	Logger::debug("[HM] High detected - waiting for belt or hole...");
}

bool High::heightValueReceived(float valueMM) {
	if(valueMM < HEIGHT_CONV_MAX) {
		actions->sendHeightResultFBM1(data->currentType, data->avgValue);
		exit();
		new(this) WaitForWorkpiece;
		entry();
		return true;
	} else if(valueMM > HEIGHT_HOLE-1 && valueMM < HEIGHT_HOLE+1) {
		actions->sendHeightResultFBM1(data->currentType, data->avgValue);
		exit();
		new(this) WaitForBelt;
		entry();
		return true;
	}
	return false;
}
