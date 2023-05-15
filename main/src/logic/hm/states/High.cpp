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
		Logger::debug("[HM] Belt detected -> WS_OB");
		data->setCurrentType(EventType::HM_M_WS_OB);
		actions->sendHeightResultFBM1(data->getCurrentType(), data->avgValue);
		exit();
		new(this) WaitForWorkpiece;
		entry();
		return true;
	} else if(valueMM > HEIGHT_HOLE-1 && valueMM < HEIGHT_HOLE+1) {
		Logger::debug("[HM] Hole detected -> WS_BOM");
		data->setCurrentType(EventType::HM_M_WS_BOM);
		actions->sendHeightResultFBM1(data->getCurrentType(), data->avgValue);
		exit();
		new(this) WaitForBelt;
		entry();
		return true;
	}
	return false;
}
