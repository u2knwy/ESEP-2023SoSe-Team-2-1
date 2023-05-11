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
	Logger::debug("[HM] Waiting for new workpiece...");
}

bool WaitForWorkpiece::heightValueReceived(float valueMM) {
	if(valueMM > HEIGHT_FLAT-1 && valueMM < HEIGHT_FLAT+1) {
		data->currentType = EventType::HM_M_WS_F;
		exit();
		new(this) WaitForBelt;
		entry();
		return true;
	} else if(valueMM > HEIGHT_HIGH-1 && valueMM < HEIGHT_HIGH+1) {
		data->currentType = EventType::HM_M_WS_OB;
		return true;
	} else  {
		data->currentType = EventType::HM_M_WS_UNKNOWN;
		return true;
	}
	return false;
}
