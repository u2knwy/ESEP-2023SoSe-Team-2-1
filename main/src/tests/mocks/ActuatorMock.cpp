/*
 * ActuatorMock.cpp
 *
 *  Created on: 12.06.2023
 *      Author: Maik
 */

#include "ActuatorMock.h"

ActuatorMock::ActuatorMock(std::shared_ptr<IEventManager> mngr) : IActuators(mngr) {}

void ActuatorMock::standbyMode() {}

void ActuatorMock::runningMode() {}

void ActuatorMock::serviceMode() {}

void ActuatorMock::errorMode() {}

void ActuatorMock::estopMode() {}
void ActuatorMock::greenLampOn() { greenLamp = LampState::ON; }

void ActuatorMock::setGreenBlinking(bool on) {
    greenLamp = on ? FLASHING_SLOW : OFF;
}

void ActuatorMock::greenLampOff() { greenLamp = LampState::OFF; }

void ActuatorMock::yellowLampOn() { yellowLamp = LampState::ON; }

void ActuatorMock::setYellowBlinking(bool on) {
    yellowLamp = on ? FLASHING_SLOW : OFF;
}

void ActuatorMock::yellowLampOff() { yellowLamp = LampState::OFF; }

void ActuatorMock::redLampOn() { redLamp = LampState::ON; }

void ActuatorMock::setRedBlinking(bool on, bool fast) {
    if (on) {
        redLamp = fast ? FLASHING_FAST : FLASHING_SLOW;
    } else {
        redLamp = OFF;
    }
}

void ActuatorMock::redLampOff() { redLamp = LampState::OFF; }

void ActuatorMock::startLedOn() { startLed = LampState::ON; }

void ActuatorMock::startLedOff() { startLed = LampState::OFF; }

void ActuatorMock::resetLedOn() { resetLed = LampState::ON; }

void ActuatorMock::resetLedOff() { resetLed = LampState::OFF; }

void ActuatorMock::q1LedOn() { q1Led = LampState::ON; }

void ActuatorMock::q1LedOff() { q1Led = LampState::OFF; }

void ActuatorMock::q2LedOn() { q2Led = LampState::ON; }

void ActuatorMock::q2LedOff() { q2Led = LampState::OFF; }

void ActuatorMock::motorStop() { motor = MotorState::STOPPED; }

void ActuatorMock::motorSlow() { motor = MotorState::RIGHT_SLOW; }

void ActuatorMock::motorFast() { motor = MotorState::RIGHT_FAST; }

void ActuatorMock::openSwitch() { switchOpen = true; }

void ActuatorMock::closeSwitch() { switchOpen = false; }

void ActuatorMock::sortOut() { switchOpen = false; }

void ActuatorMock::letPass() { switchOpen = true; }

void ActuatorMock::allOff() { }
