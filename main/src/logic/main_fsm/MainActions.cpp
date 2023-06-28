/*
 * MainActions.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "MainActions.h"
#include "configuration/Configuration.h"
#include "hal/HeightSensor.h"
#include "hal/IActuators.h"
#include "logger/logger.hpp"
#include <chrono>
#include <thread>

#define HM_CAL_MEASUREMENTS 10

MainActions::MainActions(std::shared_ptr<IEventManager> mngr, IEventSender* eventSender) {
    pusherMounted = Configuration::getInstance().pusherMounted();
    this->eventManager = mngr;
    this->sender = eventSender;
    if (sender->connect(mngr)) {
        Logger::debug("[MainActions] Connected to EventManager");
    } else {
        Logger::error("[MainActions] Error while connecting to EventManager");
    }
}

MainActions::~MainActions() {
	sender->disconnect();
	delete sender;
}

void MainActions::master_sendMotorStopRequest(bool stop) {
    Event event;
    event.type = EventType::MOTOR_M_STOP_REQ;
    event.data = (int) stop;
    sender->sendEvent(event);
}

void MainActions::master_sendMotorRightRequest(bool right) {
    Event event;
    event.type = EventType::MOTOR_M_RIGHT_REQ;
    event.data = (int) right;
    sender->sendEvent(event);
}

void MainActions::slave_sendMotorStopRequest(bool stop) {
    Event event;
    event.type = EventType::MOTOR_S_STOP_REQ;
    event.data = (int) stop;
    sender->sendEvent(event);
}

void MainActions::slave_sendMotorRightRequest(bool right) {
    Event event;
    event.type = EventType::MOTOR_S_RIGHT_REQ;
    event.data = (int) right;
    sender->sendEvent(event);
}

void MainActions::master_openGate(bool open) {
    // (EventData) 0: sort out, 1: open gate
    int eventData = open ? 0 : 1;
    sender->sendEvent(Event{SORT_M_OUT, eventData});
}

void MainActions::slave_openGate(bool open) {
    // (EventData) 0: sort out, 1: open gate
    int eventData = open ? 0 : 1;
    sender->sendEvent(Event{SORT_S_OUT, eventData});
}

void MainActions::setStandbyMode() {
    Event event;
    event.type = EventType::MODE_STANDBY;
    sender->sendEvent(event);
}

void MainActions::setRunningMode() {
    sender->sendEvent(Event{EventType::MODE_RUNNING});
}

void MainActions::setServiceMode() {
    Event event;
    event.type = EventType::MODE_SERVICE;
    sender->sendEvent(event);
}

void MainActions::setErrorMode() {
    Event event;
    event.type = EventType::MODE_ERROR;
    sender->sendEvent(event);
}

void MainActions::redLampFlashingFast() {
    sender->sendEvent(Event{EventType::LAMP_M_RED, (int) LampState::FLASHING_FAST});
    sender->sendEvent(Event{EventType::LAMP_S_RED, (int) LampState::FLASHING_FAST});
}

void MainActions::redLampFlashingSlow() {
    sender->sendEvent(Event{EventType::LAMP_M_RED, (int) LampState::FLASHING_SLOW});
    sender->sendEvent(Event{EventType::LAMP_S_RED, (int) LampState::FLASHING_SLOW});
}

void MainActions::redLampOn() {
    sender->sendEvent(Event{EventType::LAMP_M_RED, (int) LampState::ON});
    sender->sendEvent(Event{EventType::LAMP_S_RED, (int) LampState::ON});
}

void MainActions::setEStopMode() {
    Event event;
    event.type = EventType::MODE_ESTOP;
    sender->sendEvent(event);
}

void MainActions::allActuatorsOn() {
    sender->sendEvent(Event{EventType::LAMP_M_GREEN, (int) LampState::ON});
    sender->sendEvent(Event{EventType::LAMP_M_YELLOW, (int) LampState::ON});
    sender->sendEvent(Event{EventType::LAMP_M_RED, (int) LampState::ON});
    sender->sendEvent(Event{EventType::LED_M_START, (int) LampState::ON});
    sender->sendEvent(Event{EventType::LED_M_RESET, (int) LampState::ON});
    sender->sendEvent(Event{EventType::LED_M_Q1, (int) LampState::ON});
    sender->sendEvent(Event{EventType::LED_M_Q2, (int) LampState::ON});

    sender->sendEvent(Event{EventType::LAMP_S_GREEN, (int) LampState::ON});
    sender->sendEvent(Event{EventType::LAMP_S_YELLOW, (int) LampState::ON});
    sender->sendEvent(Event{EventType::LAMP_S_RED, (int) LampState::ON});
    sender->sendEvent(Event{EventType::LED_S_START, (int) LampState::ON});
    sender->sendEvent(Event{EventType::LED_S_RESET, (int) LampState::ON});
    sender->sendEvent(Event{EventType::LED_S_Q1, (int) LampState::ON});
    sender->sendEvent(Event{EventType::LED_S_Q2, (int) LampState::ON});

    if(pusherMounted) {
    	// Pusher mounted -> sort out
    	sender->sendEvent(Event{EventType::SORT_M_OUT, 1});
    } else {
    	// Switch mounted -> let pass
    	sender->sendEvent(Event{EventType::SORT_S_OUT, 0});
    }
}

void MainActions::allActuatorsOff() {
    sender->sendEvent(Event{EventType::LAMP_M_GREEN, (int) LampState::OFF});
    sender->sendEvent(Event{EventType::LAMP_M_YELLOW, (int) LampState::OFF});
    sender->sendEvent(Event{EventType::LAMP_M_RED, (int) LampState::OFF});
    sender->sendEvent(Event{EventType::LED_M_START, (int) LampState::OFF});
    sender->sendEvent(Event{EventType::LED_M_RESET, (int) LampState::OFF});
    sender->sendEvent(Event{EventType::LED_M_Q1, (int) LampState::OFF});
    sender->sendEvent(Event{EventType::LED_M_Q2, (int) LampState::OFF});

    sender->sendEvent(Event{EventType::LAMP_S_GREEN, (int) LampState::OFF});
    sender->sendEvent(Event{EventType::LAMP_S_YELLOW, (int) LampState::OFF});
    sender->sendEvent(Event{EventType::LAMP_S_RED, (int) LampState::OFF});
    sender->sendEvent(Event{EventType::LED_S_START, (int) LampState::OFF});
    sender->sendEvent(Event{EventType::LED_S_RESET, (int) LampState::OFF});
    sender->sendEvent(Event{EventType::LED_S_Q1, (int) LampState::OFF});
    sender->sendEvent(Event{EventType::LED_S_Q2, (int) LampState::OFF});

    if(pusherMounted) {
    	// Pusher mounted -> let pass
    	sender->sendEvent(Event{EventType::SORT_M_OUT, 0});
    } else {
    	// Switch mounted -> sort out (deactivate)
    	sender->sendEvent(Event{EventType::SORT_S_OUT, 1});
    }
}

void MainActions::master_warningOn() {
    sender->sendEvent(Event{EventType::LAMP_M_YELLOW, (int) LampState::FLASHING_SLOW});
}

void MainActions::master_warningOff() {
    sender->sendEvent(Event{EventType::LAMP_M_YELLOW, (int) LampState::OFF});
}

void MainActions::slave_warningOn() {
    sender->sendEvent(Event{EventType::LAMP_S_YELLOW, (int) LampState::FLASHING_SLOW});
}

void MainActions::slave_warningOff() {
    sender->sendEvent(Event{EventType::LAMP_S_YELLOW, (int) LampState::OFF});
}

void MainActions::calibrateOffset() {
    Logger::info("Calibrating HeightSensor offset...");
    sender->sendEvent(Event{HM_M_CAL_OFFSET});
    sender->sendEvent(Event{HM_S_CAL_OFFSET});
}

void MainActions::calibrateReference() {
    Logger::info("Calibrating HeightSensor reference (high)...");
    sender->sendEvent(Event{HM_M_CAL_REF});
    sender->sendEvent(Event{HM_S_CAL_REF});
}

void MainActions::saveCalibration() {
    Configuration &conf = Configuration::getInstance();
    conf.saveCurrentConfigToFile();
    Calibration cal = conf.getCalibration();
    std::stringstream ss;
    ss << "HeightSensor calibration: CAL_OFFSET=" << cal.calOffset;
    ss << "; CAL_REF=" << cal.calRef;
    ss << " -> saved to file!" << std::endl;
    Logger::debug(ss.str());
}

void MainActions::master_btnStartLedOn() {
    sender->sendEvent(Event{EventType::LED_M_START, (int) LampState::ON});
}

void MainActions::master_btnResetLedOn() {
    sender->sendEvent(Event{EventType::LED_M_RESET, (int) LampState::ON});
}

void MainActions::master_btnStartLedOff() {
    sender->sendEvent(Event{EventType::LED_M_START, (int) LampState::OFF});
}

void MainActions::master_btnResetLedOff() {
    sender->sendEvent(Event{EventType::LED_M_RESET, (int) LampState::OFF});
}


void MainActions::slave_btnStartLedOn() {
    sender->sendEvent(Event{EventType::LED_S_START, (int) LampState::ON});
}

void MainActions::slave_btnResetLedOn() {
    sender->sendEvent(Event{EventType::LED_S_RESET, (int) LampState::ON});
}

void MainActions::slave_btnStartLedOff() {
    sender->sendEvent(Event{EventType::LED_S_START, (int) LampState::OFF});
}

void MainActions::slave_btnResetLedOff() {
    sender->sendEvent(Event{EventType::LED_S_RESET, (int) LampState::OFF});
}

void MainActions::master_q1LedOn() {
    sender->sendEvent(Event{EventType::LED_M_Q1, (int) LampState::ON});
}

void MainActions::master_q1LedOff() {
    sender->sendEvent(Event{EventType::LED_M_Q1, (int) LampState::OFF});
}

void MainActions::master_q2LedOn() {
    sender->sendEvent(Event{EventType::LED_M_Q2, (int) LampState::ON});
}

void MainActions::master_q2LedOff() {
    sender->sendEvent(Event{EventType::LED_M_Q2, (int) LampState::OFF});
}

void MainActions::slave_q1LedOn() {
    sender->sendEvent(Event{EventType::LED_S_Q1, (int) LampState::ON});
}

void MainActions::slave_q1LedOff() {
    sender->sendEvent(Event{EventType::LED_S_Q1, (int) LampState::OFF});
}

void MainActions::slave_q2LedOn() {
    sender->sendEvent(Event{EventType::LED_S_Q2, (int) LampState::ON});
}

void MainActions::slave_q2LedOff() {
    sender->sendEvent(Event{EventType::LED_S_Q2, (int) LampState::OFF});
}

void MainActions::master_manualSolvingErrorOccurred() {
    sender->sendEvent(Event{EventType::MOTOR_M_STOP});
    sender->sendEvent(Event{MOTOR_S_STOP});
    sender->sendEvent(Event{EventType::ERROR_M_MAN_SOLVABLE});

}
void MainActions::slave_manualSolvingErrorOccurred() {
    sender->sendEvent(Event{EventType::MOTOR_M_STOP});
    sender->sendEvent(Event{MOTOR_S_STOP});
    sender->sendEvent(Event{EventType::ERROR_S_MAN_SOLVABLE});
}
