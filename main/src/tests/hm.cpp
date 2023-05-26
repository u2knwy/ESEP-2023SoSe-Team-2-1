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
	EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());
}

TEST_F(HeightSensor_Test, HighDetected) {
	// HIGH WS detected
	fsm->heightValueReceived(25.0);
	EXPECT_EQ(WorkpieceType::WS_OB, fsm->getDetectedWorkpieceType());
	EXPECT_EQ(HeightState::HIGH, fsm->getCurrentState());

	// NEXT COMES BELT
	fsm->heightValueReceived(1.0);
	EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());
}

TEST_F(HeightSensor_Test, FlatDetected) {
	// FLAT WS detected
	fsm->heightValueReceived(21.0);
	EXPECT_EQ(WorkpieceType::WS_F, fsm->getDetectedWorkpieceType());
	EXPECT_EQ(HeightState::WAIT_FOR_BELT, fsm->getCurrentState());

	// NEXT COMES BELT
	fsm->heightValueReceived(1.0);
	EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());
}

TEST_F(HeightSensor_Test, HighWithHoleDetected) {
	// HIGH WS detected
	fsm->heightValueReceived(25.0);
	EXPECT_EQ(WorkpieceType::WS_OB, fsm->getDetectedWorkpieceType());
	EXPECT_EQ(HeightState::HIGH, fsm->getCurrentState());

	// NEXT COMES HOLE
	fsm->heightValueReceived(6.0);
	EXPECT_EQ(WorkpieceType::WS_BOM, fsm->getDetectedWorkpieceType());
	EXPECT_EQ(HeightState::WAIT_FOR_BELT, fsm->getCurrentState());

	// NEXT COMES BELT
	fsm->heightValueReceived(1.0);
	EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());
}

TEST_F(HeightSensor_Test, UnknownDetected) {
	// UNKNOWN WS detected
	fsm->heightValueReceived(30.0);
	EXPECT_EQ(WorkpieceType::UNKNOWN, fsm->getDetectedWorkpieceType());
	EXPECT_EQ(HeightState::WAIT_FOR_WS, fsm->getCurrentState());
}
