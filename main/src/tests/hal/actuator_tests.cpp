/*
 * hal.cpp
 *
 *  Created on: 04.04.2023
 *      Author: Maik
 */
#include "ActuatorMock.h"
#include "events/EventManager.h"
#include "hal/Actuators.h"
#include "hal/HeightSensor.h"
#include "hal/Sensors.h"
#include "hal/hal.h"
#include <gtest/gtest.h>


class HAL_Test : public ::testing::Test {
  protected:
    std::shared_ptr<EventManager> mngr;
    ActuatorMock *actuators;

    void SetUp() override {
        mngr = std::make_shared<EventManager>();
        actuators = new ActuatorMock(mngr);
        Configuration::getInstance().setMaster(true);
    }

    void TearDown() override { delete actuators; }
};

TEST_F(HAL_Test, GreenLampOnOffFlashing) {
    EXPECT_EQ(LampState::OFF, actuators->greenLamp);

    mngr->sendEvent(Event{EventType::LAMP_M_GREEN, LampState::ON});
    EXPECT_EQ(LampState::ON, actuators->greenLamp);

    mngr->sendEvent(Event{EventType::LAMP_M_GREEN, LampState::FLASHING_SLOW});
    EXPECT_EQ(LampState::FLASHING_SLOW, actuators->greenLamp);

    mngr->sendEvent(Event{EventType::LAMP_M_GREEN, LampState::OFF});
    EXPECT_EQ(LampState::OFF, actuators->greenLamp);
}

TEST_F(HAL_Test, MotorFastSlowStopped) {
    EXPECT_EQ(STOPPED, actuators->motor);

    mngr->sendEvent(Event{MOTOR_M_FAST});
    EXPECT_EQ(RIGHT_FAST, actuators->motor);

    mngr->sendEvent(Event{MOTOR_M_SLOW});
    EXPECT_EQ(RIGHT_SLOW, actuators->motor);

    mngr->sendEvent(Event{MOTOR_M_STOP});
    EXPECT_EQ(STOPPED, actuators->motor);
}
