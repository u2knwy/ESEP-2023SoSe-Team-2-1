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
#include "hal/HeightSensor.h"
#include "logic/hm/HeightSensorFSM.h"
#include "logic/hm/HeightBasestate.h"
#include "logic/hm/states/WaitForWorkpiece.h"
#include "logic/hm/states/High.h"
#include "logic/hm/states/WaitForBelt.h"

class HeightSensor_Test : public ::testing::Test {
protected:
	HeightSensorFSM *fsm;

  void SetUp() override {
	  fsm = new HeightSensorFSM();
  }

  void TearDown() override {
	  delete fsm;
  }
};


TEST_F(HeightSensor_Test, DefaultState) {
	HeightBasestate* state = fsm->getCurrentState();
	WaitForWorkpiece* initialState = dynamic_cast<WaitForWorkpiece*>(state);
	EXPECT_TRUE(initialState != nullptr);
}

TEST_F(HeightSensor_Test, HighDetected) {
	HeightBasestate* state;

	// HIGH WS detected
	fsm->heightValueReceived(21.0);
	state = fsm->getCurrentState();
	High* highState = dynamic_cast<High*>(state);
	EXPECT_TRUE(highState != nullptr);
	EXPECT_EQ(EventType::HM_M_WS_OB, fsm->getDetectedWorkpieceType());

	// NEXT COMES BELT
	fsm->heightValueReceived(1.0);
	state = fsm->getCurrentState();
	WaitForWorkpiece* initialState = dynamic_cast<WaitForWorkpiece*>(state);
	EXPECT_TRUE(initialState != nullptr);
}

TEST_F(HeightSensor_Test, FlatDetected) {
	HeightBasestate* state;

	// FLAT WS detected
	fsm->heightValueReceived(18.0);
	state = fsm->getCurrentState();
	WaitForBelt* waitForBeltState = dynamic_cast<WaitForBelt*>(state);
	EXPECT_TRUE(waitForBeltState != nullptr);
	EXPECT_EQ(EventType::HM_M_WS_F, fsm->getDetectedWorkpieceType());

	// NEXT COMES BELT
	fsm->heightValueReceived(1.0);
	state = fsm->getCurrentState();
	WaitForWorkpiece* initialState = dynamic_cast<WaitForWorkpiece*>(state);
	EXPECT_TRUE(initialState != nullptr);
}

TEST_F(HeightSensor_Test, HighWithHoleDetected) {
	HeightBasestate* state;

	// HIGH WS detected
	fsm->heightValueReceived(21.0);
	state = fsm->getCurrentState();
	High* highState = dynamic_cast<High*>(state);
	EXPECT_TRUE(highState != nullptr);

	// NEXT COMES HOLE
	fsm->heightValueReceived(10.0);
	state = fsm->getCurrentState();
	WaitForBelt* waitForBeltState = dynamic_cast<WaitForBelt*>(state);
	EXPECT_TRUE(waitForBeltState != nullptr);
	EXPECT_EQ(EventType::HM_M_WS_BOM, fsm->getDetectedWorkpieceType());

	// NEXT COMES BELT
	fsm->heightValueReceived(1.0);
	state = fsm->getCurrentState();
	WaitForWorkpiece* initialState = dynamic_cast<WaitForWorkpiece*>(state);
	EXPECT_TRUE(initialState != nullptr);
}

TEST_F(HeightSensor_Test, UnknownDetected) {
	HeightBasestate* state;

	// UNKNOWN WS detected
	fsm->heightValueReceived(30.0);
	state = fsm->getCurrentState();
	WaitForBelt* waitForBeltState = dynamic_cast<WaitForBelt*>(state);
	EXPECT_TRUE(waitForBeltState != nullptr);
	EXPECT_EQ(EventType::HM_M_WS_UNKNOWN, fsm->getDetectedWorkpieceType());

	// NEXT COMES BELT
	fsm->heightValueReceived(1.0);
	state = fsm->getCurrentState();
	WaitForWorkpiece* initialState = dynamic_cast<WaitForWorkpiece*>(state);
	EXPECT_TRUE(initialState != nullptr);
}
