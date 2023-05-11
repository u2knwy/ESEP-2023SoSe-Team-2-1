/*
 * WaitForWorkpiece.cpp
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#include <logic/hm/states/WaitForWorkpiece.h>
#include "hal/HeightSensor.h"
#include "WaitForBelt.h"

void WaitForWorkpiece::entry() {
	data->avgValue = 0;
	data->maxValue = 0;
}

bool WaitForWorkpiece::heightValueReceived(int value) {
	if(value > HEIGHT_FLAT-1 && value < HEIGHT_FLAT+1) {
		data->currentType = EventType::HM_M_WS_F;
		exit();
		new(this) WaitForBelt;
		entry();
		return true;
	} else if(value > HEIGHT_HIGH-1 && value < HEIGHT_HIGH+1) {
		data->currentType = EventType::HM_M_WS_OB;
		return true;
	} else  {
		data->currentType = EventType::HM_M_WS_UNKNOWN;
		return true;
	}
	return false;
}
