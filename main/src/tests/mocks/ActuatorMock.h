/*
 * ActuatorMock.h
 *
 *  Created on: 12.06.2023
 *      Author: Maik
 */

#ifndef SRC_TESTS_HAL_ACTUATORMOCK_H_
#define SRC_TESTS_HAL_ACTUATORMOCK_H_

#include "hal/IActuators.h"
#include "logic/motor_fsm/MotorBasestate.h"
#include "EventManagerMock.h"

class ActuatorMock : public IActuators {
  public:
    ActuatorMock(std::shared_ptr<IEventManager> mngr);

    void standbyMode() override;
    void runningMode() override;
    void serviceMode() override;
    void errorMode() override;
    void estopMode() override;
    void greenLampOn() override;
    void setGreenBlinking(bool on) override;
    void greenLampOff() override;
    void yellowLampOn() override;
    void setYellowBlinking(bool on) override;
    void yellowLampOff() override;
    void redLampOn() override;
    void setRedBlinking(bool on, bool fast) override;
    void redLampOff() override;
    void startLedOn() override;
    void startLedOff() override;
    void resetLedOn() override;
    void resetLedOff() override;
    void q1LedOn() override;
    void q1LedOff() override;
    void q2LedOn() override;
    void q2LedOff() override;
    void motorStop() override;
    void motorSlow() override;
    void motorFast() override;
    void openSwitch() override;
    void closeSwitch() override;
    void sortOut() override;
    void letPass() override;
    void allOff() override;
    void connectionLost() override;

    // To test function calls:
    LampState greenLamp{OFF};
    LampState yellowLamp{OFF};
    LampState redLamp{OFF};
    LampState startLed{OFF};
    LampState resetLed{OFF};
    LampState q1Led{OFF};
    LampState q2Led{OFF};
    MotorState motor{STOPPED};
    bool switchOpen{false};
};

#endif /* SRC_TESTS_HAL_ACTUATORMOCK_H_ */
