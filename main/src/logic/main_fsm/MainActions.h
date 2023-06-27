/*
 * MainActions.h
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#ifndef SRC_LOGIC_MAIN_FSM_MAINACTIONS_H_
#define SRC_LOGIC_MAIN_FSM_MAINACTIONS_H_

#include "events/IEventManager.h"
#include "events/IEventSender.h"

#include <memory>

class MainActions {
  public:
    MainActions(std::shared_ptr<IEventManager> mngr, IEventSender* eventSender);
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
    void master_btnStartLedOn();
    void master_btnStartLedOff();
    void master_btnResetLedOn();
    void master_btnResetLedOff();
    void slave_btnStartLedOn();
    void slave_btnStartLedOff();
    void slave_btnResetLedOn();
    void slave_btnResetLedOff();
    void master_q1LedOn();
    void master_q1LedOff();
    void master_q2LedOn();
    void master_q2LedOff();
    void slave_q1LedOn();
    void slave_q1LedOff();
    void slave_q2LedOn();
    void slave_q2LedOff();
    void master_manualSolvingErrorOccurred();
    void slave_manualSolvingErrorOccurred();

    std::shared_ptr<IEventManager> eventManager;
    bool pusherMounted;
  private:
    IEventSender* sender;
};

#endif /* SRC_LOGIC_MAIN_FSM_MAINACTIONS_H_ */
