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
}

bool High::heightValueReceived(int value) {
	if(value < HEIGHT_CONV_MAX) {
		actions->sendHeightResultFBM1(data->currentType, data->avgValue);
		exit();
		new(this) WaitForWorkpiece;
		entry();
		return true;
	} else if(value > HEIGHT_HOLE-1 && value < HEIGHT_HOLE+1) {
		actions->sendHeightResultFBM1(data->currentType, data->avgValue);
		exit();
		new(this) WaitForBelt;
		entry();
		return true;
	}
	return false;
}
