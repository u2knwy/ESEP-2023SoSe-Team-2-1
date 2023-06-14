/*
 * fsm.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

/*
 * hal.cpp
 *
 *  Created on: 04.04.2023
 *      Author: Maik
 */
#include <gtest/gtest.h>
#include "logic/main_fsm/MainContext.h"
#include "events/EventManager.h"

class MainFSM_Test : public ::testing::Test {
protected:
	std::shared_ptr<MainContext> fsm;
	std::shared_ptr<EventManager> eventManager;

  void SetUp() override {
	  eventManager = std::make_shared<EventManager>();
	  fsm = std::make_shared<MainContext>(eventManager);
  }

  void TearDown() override {
  }

};

// When launching the FSM -> Current state must be STANDBY
TEST_F(MainFSM_Test, StartStateStandby) {
	EXPECT_EQ(MainState::STANDBY, fsm->getCurrentState());
}

// Start pressed short -> Current state must be RUNNING
TEST_F(MainFSM_Test, StateRunningAfterStartPressedShort) {
	fsm->master_btnStart_PressedShort();
	EXPECT_EQ(MainState::RUNNING, fsm->getCurrentState());

	fsm.reset();
	fsm = std::make_shared<MainContext>(eventManager);

	fsm->slave_btnStart_PressedShort();
	EXPECT_EQ(MainState::RUNNING, fsm->getCurrentState());
}

// Start pressed long -> Current state must be SERVICE_MODE
TEST_F(MainFSM_Test, StateServiceModeAfterStartPressedLong) {
	fsm->master_btnStart_PressedLong();
	EXPECT_EQ(MainState::SERVICEMODE, fsm->getCurrentState());	// in ServiceMode
	fsm->master_btnStop_Pressed();
	EXPECT_EQ(MainState::STANDBY, fsm->getCurrentState()); // back in Standby

	fsm.reset();
	fsm = std::make_shared<MainContext>(eventManager);

	fsm->slave_btnStart_PressedLong();
	EXPECT_EQ(MainState::SERVICEMODE, fsm->getCurrentState());	// in ServiceMode
	fsm->slave_btnStop_Pressed();
	EXPECT_EQ(MainState::STANDBY, fsm->getCurrentState()); // back in Standby
}

// In state running and stop pressed -> Current state must be STANDBY
TEST_F(MainFSM_Test, StateStandbyAfterRunningAndStopPressed) {
	// Buttons pressed at master
	fsm->master_btnStart_PressedShort();
	fsm->master_btnStop_Pressed();
	EXPECT_EQ(MainState::STANDBY, fsm->getCurrentState());

	// Buttons pressed at slave
	fsm->slave_btnStart_PressedShort();
	fsm->slave_btnStop_Pressed();
	EXPECT_EQ(MainState::STANDBY, fsm->getCurrentState());
}

// In state running and EStop pressed -> Current state must be ESTOP
TEST_F(MainFSM_Test, StateEStopAfterRunningAndEStopPressed) {
	// Buttons pressed at master
	fsm->master_btnStart_PressedShort();
	EXPECT_EQ(MainState::RUNNING, fsm->getCurrentState());
	fsm->master_EStop_Pressed();
	EXPECT_EQ(MainState::ESTOP, fsm->getCurrentState()); // must be in EStop

	// One EStop pressed -> must still stay in EStop
	fsm->master_btnReset_Pressed();
	EXPECT_EQ(MainState::ESTOP, fsm->getCurrentState());
	fsm->slave_btnReset_Pressed();
	EXPECT_EQ(MainState::ESTOP, fsm->getCurrentState());

	// Second EStop pressed -> must still stay in EStop
	fsm->slave_EStop_Pressed();
	EXPECT_EQ(MainState::ESTOP, fsm->getCurrentState());
	fsm->master_btnReset_Pressed();
	EXPECT_EQ(MainState::ESTOP, fsm->getCurrentState());
	fsm->slave_btnReset_Pressed();
	EXPECT_EQ(MainState::ESTOP, fsm->getCurrentState());

	// First EStop released -> must still stay in EStop
	fsm->master_EStop_Released();
	fsm->master_btnReset_Pressed();
	EXPECT_EQ(MainState::ESTOP, fsm->getCurrentState());
	fsm->slave_btnReset_Pressed();
	EXPECT_EQ(MainState::ESTOP, fsm->getCurrentState());

	// Second EStop released -> must go to Standby if both reset buttons pressed
	fsm->slave_EStop_Released();
	fsm->master_btnReset_Pressed();
	EXPECT_EQ(MainState::ESTOP, fsm->getCurrentState()); // still in EStop
	fsm->slave_btnReset_Pressed();
	EXPECT_EQ(MainState::STANDBY, fsm->getCurrentState()); // now in Standby
}

TEST_F(MainFSM_Test, ErrorManualSolvedAfterRunning) {
	EXPECT_EQ(MainState::STANDBY, fsm->getCurrentState());
	fsm->master_btnStart_PressedShort();
	EXPECT_EQ(MainState::RUNNING, fsm->getCurrentState());

	fsm->nonSelfSolvableErrorOccurred();
	EXPECT_EQ(MainState::ERROR, fsm->getCurrentState());
	fsm->master_btnStart_PressedShort();
	EXPECT_EQ(MainState::ERROR, fsm->getCurrentState());
	fsm->master_btnReset_Pressed();
	EXPECT_EQ(MainState::ERROR, fsm->getCurrentState());
	fsm->master_btnStart_PressedShort();
	EXPECT_EQ(MainState::STANDBY, fsm->getCurrentState());
}

TEST_F(MainFSM_Test, ErrorSelfSolvedAfterRunning) {
	// Running -> Error
	EXPECT_EQ(MainState::STANDBY, fsm->getCurrentState());
	fsm->master_btnStart_PressedShort();
	EXPECT_EQ(MainState::RUNNING, fsm->getCurrentState());

	// Self-solvable error -> Error Pending Unresigned
	// selfSolving=true, manSolving=false
	fsm->selfSolvableErrorOccurred();
	EXPECT_EQ(MainState::ERROR, fsm->getCurrentState());
	fsm->master_btnStart_PressedShort();
	fsm->master_btnReset_Pressed();
	EXPECT_EQ(MainState::ERROR, fsm->getCurrentState());

	// Error was solved -> Solved unresigned
	// selfSolving=false, manSolving=false
	fsm->errorSelfSolved();
	EXPECT_EQ(MainState::ERROR, fsm->getCurrentState());
	// Reset pressed -> Leave Error mode
	fsm->master_btnReset_Pressed();
	EXPECT_EQ(MainState::STANDBY, fsm->getCurrentState());
}
