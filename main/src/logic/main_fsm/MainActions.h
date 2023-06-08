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
	void setMotorStop(bool stop);
	void setMotorFast(bool fast);
	void setMotorSlow(bool slow);
	void setStandbyMode();
	void setRunningMode();
	void setServiceMode();
	void setErrorMode();
	void setEStopMode();
	void allActuatorsOn();
	void allActuatorsOff();
	void displayWarning();
	void warningOff();
	void calibrateOffset();
	void calibrateReference();
	void saveCalibration();
private:
	std::shared_ptr<EventManager> eventManager;
};

#endif /* SRC_LOGIC_MAIN_FSM_MAINACTIONS_H_ */
