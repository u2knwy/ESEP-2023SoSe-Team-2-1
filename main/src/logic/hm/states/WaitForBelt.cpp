/*
 * WaitForWorkpiece.cpp
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#include "hal/HeightSensor.h"
#include "WaitForWorkpiece.h"
#include "WaitForBelt.h"

void WaitForBelt::entry() {
	data->avgValue = 0;
	data->maxValue = 0;
}

bool WaitForBelt::heightValueReceived(int value) {
	if(value < HEIGHT_CONV_MAX) {
		actions->sendHeightResultFBM1(data->currentType, data->avgValue);
		exit();
		new(this) WaitForWorkpiece;
		entry();
		return true;
	}
	return false;
}
