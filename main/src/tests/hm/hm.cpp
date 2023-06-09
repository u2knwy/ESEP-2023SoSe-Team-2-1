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
#include <gtest/gtest.h>
#include <logic/hm/HeightContext.h>
#include "hal/HeightSensor.h"
#include "hal/IHeightSensor.h"
#include "logic/hm/HeightBasestate.h"
#include "logic/hm/states/WaitForWorkpiece.h"
#include "logic/hm/states/WaitForBelt.h"
#include "events/EventManager.h"
#include "events/events.h"
#include "HeightSensorMock.h"

class HeightSensor_Test : public ::testing::Test {
protected:
	HeightContext *fsm;

  void SetUp() override {
	  std::shared_ptr<IHeightSensor> heightSensor = std::make_shared<HeightSensorMock>();
	  std::shared_ptr<EventManager> eventManager = std::make_shared<EventManager>();
	  fsm = new HeightContext(eventManager, heightSensor);
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

TEST_F(HeightSensor_Test, InitialStateAfterStartup) {
	EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());
}

TEST_F(HeightSensor_Test, WaitForWorkpieceWhenMotorRunning) {
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

TEST_F(HeightSensor_Test, HighDetected) {
	Event ev;
	ev.type = EventType::MOTOR_M_FAST;
	fsm->handleEvent(ev);
	EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());

	// HIGH WS detected
	fsm->heightValueReceived(25.0);
	HeightResult res = fsm->getCurrentResult();
	EXPECT_EQ(WorkpieceType::WS_OB, res.type);
	EXPECT_EQ(HeightState::HIGH, fsm->getCurrentState());

	// NEXT COMES BELT
	fsm->heightValueReceived(1.0);
	EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());
}

TEST_F(HeightSensor_Test, FlatDetected) {
	Event ev;
	ev.type = EventType::MOTOR_M_FAST;
	fsm->handleEvent(ev);
	EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());

	// FLAT WS detected
	fsm->heightValueReceived(21.0);
	EXPECT_EQ(WorkpieceType::WS_F, fsm->getCurrentResult().type);
	EXPECT_EQ(HeightState::WAIT_FOR_BELT, fsm->getCurrentState());

	// NEXT COMES BELT
	fsm->heightValueReceived(1.0);
	EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());
}

TEST_F(HeightSensor_Test, HighWithHoleDetected) {
	Event ev;
	ev.type = EventType::MOTOR_M_FAST;
	fsm->handleEvent(ev);
	EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());

	// HIGH WS detected
	fsm->heightValueReceived(25.0);
	EXPECT_EQ(WorkpieceType::WS_OB, fsm->getCurrentResult().type);
	EXPECT_EQ(HeightState::HIGH, fsm->getCurrentState());

	// NEXT COMES HOLE
	fsm->heightValueReceived(6.0);
	EXPECT_EQ(WorkpieceType::WS_BOM, fsm->getCurrentResult().type);
	EXPECT_EQ(HeightState::WAIT_FOR_BELT, fsm->getCurrentState());

	// NEXT COMES BELT
	fsm->heightValueReceived(1.0);
	EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());
}

TEST_F(HeightSensor_Test, UnknownDetected) {
	Event ev;
	ev.type = EventType::MOTOR_M_FAST;
	fsm->handleEvent(ev);
	EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());

	// UNKNOWN WS detected
	fsm->heightValueReceived(30.0);
	EXPECT_EQ(WorkpieceType::WS_UNKNOWN, fsm->getCurrentResult().type);
	EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());
}

TEST_F(HeightSensor_Test, CalculateAverageAndMaxValue) {
	Event ev;
	HeightResult res;

	ev.type = EventType::MOTOR_M_FAST;
	fsm->handleEvent(ev);
	EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());
	res = fsm->getCurrentResult();
	EXPECT_EQ("0.00", formatFloat(res.average, 2));
	EXPECT_EQ("0.00", formatFloat(res.max, 2));

	fsm->heightValueReceived(25.3);
	fsm->heightValueReceived(25.1);
	fsm->heightValueReceived(6.2);
	fsm->heightValueReceived(21.2);
	fsm->heightValueReceived(25.0);
	fsm->heightValueReceived(24.9);
	res = fsm->getCurrentResult();
	EXPECT_EQ("21.28", formatFloat(res.average, 2));
	EXPECT_EQ("25.30", formatFloat(res.max, 2));

	ev.type = EventType::MOTOR_M_STOP;
	fsm->handleEvent(ev);
	EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());
	ev.type = EventType::MOTOR_M_FAST;
	fsm->handleEvent(ev);
	EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());
	res = fsm->getCurrentResult();
	EXPECT_EQ("0.00", formatFloat(res.average, 2));
	EXPECT_EQ("0.00", formatFloat(res.max, 2));
	fsm->heightValueReceived(25.0);
	EXPECT_EQ(HeightState::WAIT_FOR_BELT, fsm->getCurrentState());
	res = fsm->getCurrentResult();
	EXPECT_EQ(WorkpieceType::WS_OB, res.type);
	EXPECT_EQ("25.00", formatFloat(res.average, 2));
	EXPECT_EQ("25.00", formatFloat(res.max, 2));
}

TEST_F(HeightSensor_Test, CalculateAverageAndMaxValueWhenMotorStoppedInBetween) {
	HeightResult res;

	Event ev;
	ev.type = EventType::MOTOR_M_FAST;
	fsm->handleEvent(ev);
	EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());

	// FLAT received -> save measurements and stay in state WaitForBelt
	fsm->heightValueReceived(21.0);
	fsm->heightValueReceived(21.8);
	EXPECT_EQ(HeightState::WAIT_FOR_BELT, fsm->getCurrentState());

	// Motor is stopped -> HM should be "paused"
	ev.type = EventType::MOTOR_M_STOP;
	fsm->handleEvent(ev);
	// The following measurements should not influence the current average and max values
	fsm->heightValueReceived(25.0);
	fsm->heightValueReceived(50.0);
	res = fsm->getCurrentResult();
	EXPECT_EQ("21.4", formatFloat(res.average, 1));
	EXPECT_EQ("21.8", formatFloat(res.max, 1));

	// Motor is running again
	ev.type = EventType::MOTOR_M_FAST;
	fsm->handleEvent(ev);
	// the following values should influence the current average and max values
	fsm->heightValueReceived(22.0);
	res = fsm->getCurrentResult();
	EXPECT_EQ("21.6", formatFloat(res.average, 1));
	EXPECT_EQ("22.0", formatFloat(res.max, 1));

	// Motor is stopped -> HM should be "paused"
	ev.type = EventType::MOTOR_M_STOP;
	fsm->handleEvent(ev);
	// The following measurements should not influence the current average and max values
	fsm->heightValueReceived(25.0);
	fsm->heightValueReceived(50.0);
	res = fsm->getCurrentResult();
	EXPECT_EQ("21.6", formatFloat(res.average, 1));
	EXPECT_EQ("22.0", formatFloat(res.max, 1));

	// Motor is running again (in slow speed)
	ev.type = EventType::MOTOR_M_SLOW;
	fsm->handleEvent(ev);
	// the following values should influence the current average and max values
	fsm->heightValueReceived(22.4); // 87,2
	res = fsm->getCurrentResult();
	EXPECT_EQ("21.8", formatFloat(res.average, 1));
	EXPECT_EQ("22.4", formatFloat(res.max, 1));
}
