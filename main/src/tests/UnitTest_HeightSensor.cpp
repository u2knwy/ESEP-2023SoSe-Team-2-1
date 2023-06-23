/*
 * hm.cpp
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

/*
 * hal.cpp
 *
 *  Created on: 04.04.2023
 *      Author: Maik
 */
#include "mocks/HeightSensorMock.h"
#include "mocks/EventManagerMock.h"
#include "mocks/EventSenderMock.h"

#include "events/EventManager.h"
#include "events/events.h"
#include "hal/HeightSensor.h"
#include "hal/IHeightSensor.h"
#include "logic/hm/HeightBasestate.h"
#include "logic/hm/states/WaitForBelt.h"
#include "logic/hm/states/WaitForWorkpiece.h"
#include "logic/hm/HeightContext.h"

#include <gtest/gtest.h>

class UnitTest_HeightSensor : public ::testing::Test {
  protected:
	std::shared_ptr<IEventManager> eventManager = std::make_shared<EventManagerMock>();
    HeightContext *fsm;
    std::shared_ptr<HeightSensorMock> sensor;

    void SetUp() override {
        sensor = std::make_shared<HeightSensorMock>();
        HeightContextData* data = new HeightContextData();
		IEventSender* sender = new EventSenderMock();
        HeightActions* actions = new HeightActions(data, sender, eventManager);
        fsm = new HeightContext(actions, data, sensor);
    }

    void TearDown() override {
    	delete fsm;
    }
};

static std::string formatFloat(float value, int decimalPlaces) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(decimalPlaces) << value;
    return stream.str();
}

TEST_F(UnitTest_HeightSensor, InitialStateAfterStartup) {
    EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());
}

TEST_F(UnitTest_HeightSensor, WaitForWorkpieceWhenMotorRunning) {
    Event ev;
    ev.type = EventType::MOTOR_M_FAST;
    fsm->handleEvent(ev);
    EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());

    ev.type = EventType::MOTOR_M_STOP;
    fsm->handleEvent(ev);
    EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());

    ev.type = EventType::MOTOR_M_SLOW;
    fsm->handleEvent(ev);
    EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());
}

TEST_F(UnitTest_HeightSensor, HighDetected) {
    Event ev;
    ev.type = EventType::MOTOR_M_FAST;
    fsm->handleEvent(ev);
    EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());

    // HIGH WS detected
    fsm->heightValueReceived(25.0);
    fsm->heightValueReceived(25.0);
    fsm->heightValueReceived(25.0);
    fsm->heightValueReceived(25.0);
    fsm->heightValueReceived(25.0);
    fsm->heightValueReceived(25.0);
    fsm->heightValueReceived(25.0);
    fsm->heightValueReceived(25.0);
    fsm->heightValueReceived(25.0);
    fsm->heightValueReceived(25.0);
    HeightResult res = fsm->getCurrentResult();
    EXPECT_EQ(WorkpieceType::WS_OB, res.type);
    EXPECT_EQ(HeightState::WAIT_FOR_BELT, fsm->getCurrentState());

    // NEXT COMES BELT
    fsm->heightValueReceived(1.0);
    EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());
}

TEST_F(UnitTest_HeightSensor, FlatDetected) {
    Event ev;
    ev.type = EventType::MOTOR_M_FAST;
    fsm->handleEvent(ev);
    EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());

    // FLAT WS detected
    fsm->heightValueReceived(21.0);
    EXPECT_EQ(HeightState::WAIT_FOR_BELT, fsm->getCurrentState());
    fsm->heightValueReceived(21.0);
    fsm->heightValueReceived(21.0);
    fsm->heightValueReceived(21.0);
    fsm->heightValueReceived(21.0);

    EXPECT_EQ(WorkpieceType::WS_F, fsm->getCurrentResult().type);

    // NEXT COMES BELT
    fsm->heightValueReceived(1.0);
    EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());
}

TEST_F(UnitTest_HeightSensor, HighWithHoleDetected) {
    Event ev;
    ev.type = EventType::MOTOR_M_FAST;
    fsm->handleEvent(ev);
    EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());

    // HIGH WS detected
    fsm->heightValueReceived(25.0);
    EXPECT_EQ(HeightState::WAIT_FOR_BELT, fsm->getCurrentState());
    // Next comes HOLE
    fsm->heightValueReceived(6.0);
    // And HIGH again
    fsm->heightValueReceived(25.0);

    // Next comes BELT
    fsm->heightValueReceived(1.0);
    EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());
}

TEST_F(UnitTest_HeightSensor, UnknownDetected) {
    Event ev;
    ev.type = EventType::MOTOR_M_FAST;
    fsm->handleEvent(ev);
    EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());

    // UNKNOWN WS detected
    fsm->heightValueReceived(30.0);
    // Next comes BELT
    fsm->heightValueReceived(1.0);
    EXPECT_EQ(WorkpieceType::WS_UNKNOWN, fsm->getCurrentResult().type);
    EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());
}

TEST_F(UnitTest_HeightSensor, CalculateAverageAndMaxValue) {
    Event ev;
    HeightResult res;

    ev.type = EventType::MOTOR_M_FAST;
    fsm->handleEvent(ev);
    EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());
    res = fsm->getCurrentResult();
    EXPECT_EQ("0.00", formatFloat(res.average, 2));
    EXPECT_EQ("0.00", formatFloat(res.max, 2));

    // 1 invalid measurement - first value must be discarded
    fsm->heightValueReceived(11.0);

    // 18 valid measurements
    for (int i = 0; i < 50; i++) {
        fsm->heightValueReceived(25.0);
    }
    fsm->heightValueReceived(26.0);

    // 1 invalid measurement - last value must be discarded
    fsm->heightValueReceived(11.0);

    res = fsm->getCurrentResult();
    EXPECT_EQ("25.00", formatFloat(res.average, 2));
    EXPECT_EQ("26.00", formatFloat(res.max, 2));
}

TEST_F(UnitTest_HeightSensor, CalculateAverageAndMaxValueWhenMotorStoppedInBetween) {
    HeightResult res;

    Event ev;
    ev.type = EventType::MOTOR_M_FAST;
    fsm->handleEvent(ev);
    EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());

    // first measurement must be discarded
    fsm->heightValueReceived(21.0);

    // FLAT received -> save measurements and stay in state WaitForBelt
    EXPECT_EQ(HeightState::WAIT_FOR_BELT, fsm->getCurrentState());

    for (int i = 0; i < 10; i++) {
        fsm->heightValueReceived(25.0);
    }
    fsm->heightValueReceived(26.0);
    fsm->heightValueReceived(24.0);
    // Motor is stopped -> HM should be "paused"
    ev.type = EventType::MOTOR_M_STOP;
    fsm->handleEvent(ev);
    // these measurements must be ignored
    fsm->heightValueReceived(50.0);
    // Motor is running again
    ev.type = EventType::MOTOR_M_FAST;
    fsm->handleEvent(ev);

    // these measurements must be handled again
    for (int i = 0; i < 10; i++) {
        fsm->heightValueReceived(25.0);
    }

    // last measurement must be discarded
    fsm->heightValueReceived(11.0);

    res = fsm->getCurrentResult();
    EXPECT_EQ("25.0", formatFloat(res.average, 1));
    EXPECT_EQ("26.0", formatFloat(res.max, 1));
}
