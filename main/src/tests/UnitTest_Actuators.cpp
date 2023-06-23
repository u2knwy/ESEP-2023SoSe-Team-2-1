/*
 * hal.cpp
 *
 *  Created on: 04.04.2023
 *      Author: Maik
 */
#include "mocks/ActuatorMock.h"
#include "mocks/EventManagerMock.h"

#include "hal/Actuators.h"
#include "hal/HeightSensor.h"
#include "hal/Sensors.h"
#include "hal/hal.h"

#include <gtest/gtest.h>


class UnitTest_Actuators : public ::testing::Test {
  protected:
    std::shared_ptr<IEventManager> mngr;
    ActuatorMock *actuators;

    void SetUp() override {
        mngr = std::make_shared<EventManagerMock>();
        actuators = new ActuatorMock(mngr);
        Configuration::getInstance().setMaster(true);
    }

    void TearDown() override { delete actuators; }
};

TEST_F(UnitTest_Actuators, GreenLampOnOffFlashing) {
    EXPECT_EQ(LampState::OFF, actuators->greenLamp);

    mngr->handleEvent(Event{EventType::LAMP_M_GREEN, LampState::ON});
    EXPECT_EQ(LampState::ON, actuators->greenLamp);

    mngr->handleEvent(Event{EventType::LAMP_M_GREEN, LampState::FLASHING_SLOW});
    EXPECT_EQ(LampState::FLASHING_SLOW, actuators->greenLamp);

    mngr->handleEvent(Event{EventType::LAMP_M_GREEN, LampState::OFF});
    EXPECT_EQ(LampState::OFF, actuators->greenLamp);
}

TEST_F(UnitTest_Actuators, MotorFastSlowStopped) {
    EXPECT_EQ(STOPPED, actuators->motor);

    mngr->handleEvent(Event{MOTOR_M_FAST});
    EXPECT_EQ(RIGHT_FAST, actuators->motor);

    mngr->handleEvent(Event{MOTOR_M_SLOW});
    EXPECT_EQ(RIGHT_SLOW, actuators->motor);

    mngr->handleEvent(Event{MOTOR_M_STOP});
    EXPECT_EQ(STOPPED, actuators->motor);
}
