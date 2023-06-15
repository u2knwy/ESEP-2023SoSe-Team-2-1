/*
 * MainActions.h
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#ifndef SRC_LOGIC_MAIN_FSM_MAINACTIONS_H_
#define SRC_LOGIC_MAIN_FSM_MAINACTIONS_H_

#include "events/EventManager.h"

#include <memory>

class MainActions {
public:
	MainActions(std::shared_ptr<EventManager> mngr);
	virtual ~MainActions();
	void master_sendMotorStopRequest(bool stop);
	void slave_sendMotorStopRequest(bool stop);
	void master_sendMotorRightRequest(bool right);
	void slave_sendMotorRightRequest(bool right);
	void master_openGate(bool open);
	void slave_openGate(bool open);
	void setStandbyMode();
	void setRunningMode();
	void setServiceMode();
	void setErrorMode();
	void redLampOn();
	void redLampFlashingFast();
	void redLampFlashingSlow();
	void setEStopMode();
	void allActuatorsOn();
	void allActuatorsOff();
	void master_warningOn();
	void master_warningOff();
	void slave_warningOn();
	void slave_warningOff();
	void calibrateOffset();
	void calibrateReference();
	void saveCalibration();
	void btnStartLedOn();
	void btnStartLedOff();
	void btnResetLedOn();
	void btnResetLedOff();
private:
	std::shared_ptr<EventManager> eventManager;
};

#endif /* SRC_LOGIC_MAIN_FSM_MAINACTIONS_H_ */
