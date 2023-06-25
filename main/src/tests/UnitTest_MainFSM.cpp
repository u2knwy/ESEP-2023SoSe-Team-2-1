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
#include "mocks/EventManagerMock.h"
#include "mocks/EventSenderMock.h"

#include "configuration/Configuration.h"
#include "logic/main_fsm/MainContext.h"

#include <gtest/gtest.h>

class UnitTest_MainFSM : public ::testing::Test {
  protected:
	std::shared_ptr<IEventManager> eventManager = std::make_shared<EventManagerMock>();
    IEventSender* sender;
    MainActions* mainActions;
    MainContext* fsm;

    void SetUp() override {
    	Configuration::getInstance().setDesiredWorkpieceOrder({WS_F, WS_BOM, WS_OB});
    	sender = new EventSenderMock();
        mainActions = new MainActions(eventManager, sender);
        fsm = new MainContext(mainActions);
    }

    void TearDown() override {
    	delete fsm;
    }
};

// When launching the FSM -> Current state must be STANDBY
TEST_F(UnitTest_MainFSM, StartStateStandby) {
    EXPECT_EQ(MainState::STANDBY, fsm->getCurrentState());
    EXPECT_TRUE(true);
}

// Start pressed short -> Current state must be RUNNING
TEST_F(UnitTest_MainFSM, StateRunningAfterStartPressedShort) {
    fsm->master_btnStart_PressedShort();
    EXPECT_EQ(MainState::RUNNING, fsm->getCurrentState());

    delete fsm;
	sender = new EventSenderMock();
    mainActions = new MainActions(eventManager, sender);
    fsm = new MainContext(mainActions);

    fsm->slave_btnStart_PressedShort();
    EXPECT_EQ(MainState::RUNNING, fsm->getCurrentState());
}

// Start pressed long -> Current state must be SERVICE_MODE
TEST_F(UnitTest_MainFSM, StateServiceModeAfterStartPressedLong) {
    fsm->master_btnStart_PressedLong();
    EXPECT_EQ(MainState::SERVICEMODE,
              fsm->getCurrentState());                       // in ServiceMode
    fsm->master_btnStop_Pressed();
    EXPECT_EQ(MainState::STANDBY, fsm->getCurrentState());   // back in Standby

    delete fsm;
	sender = new EventSenderMock();
    mainActions = new MainActions(eventManager, sender);
    fsm = new MainContext(mainActions);

    fsm->slave_btnStart_PressedLong();
    EXPECT_EQ(MainState::SERVICEMODE,
              fsm->getCurrentState());                       // in ServiceMode
    fsm->slave_btnStop_Pressed();
    EXPECT_EQ(MainState::STANDBY, fsm->getCurrentState());   // back in Standby
}

// In state running and stop pressed -> Current state must be STANDBY
TEST_F(UnitTest_MainFSM, StateStandbyAfterRunningAndStopPressed) {
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
TEST_F(UnitTest_MainFSM, StateEStopAfterRunningAndEStopPressed) {
    // Buttons pressed at master
    fsm->master_btnStart_PressedShort();
    EXPECT_EQ(MainState::RUNNING, fsm->getCurrentState());
    fsm->master_EStop_Pressed();
    EXPECT_EQ(MainState::ESTOP, fsm->getCurrentState());   // must be in EStop

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
    EXPECT_EQ(MainState::ESTOP, fsm->getCurrentState());     // still in EStop
    fsm->slave_btnReset_Pressed();
    EXPECT_EQ(MainState::STANDBY, fsm->getCurrentState());   // now in Standby
}

TEST_F(UnitTest_MainFSM, ErrorManualSolvedAfterRunning) {
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
    EXPECT_EQ(MainState::RUNNING, fsm->getCurrentState());
}

TEST_F(UnitTest_MainFSM, ErrorSelfSolvedAfterRunning) {
    // Standby -> Running
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
    EXPECT_EQ(MainState::RUNNING, fsm->getCurrentState());
}

TEST_F(UnitTest_MainFSM, RunningToErrorToRunningHistory) {
    // Standby -> Running
    EXPECT_EQ(MainState::STANDBY, fsm->getCurrentState());
    fsm->master_btnStart_PressedShort();
    EXPECT_EQ(MainState::RUNNING, fsm->getCurrentState());

    // One workpiece at FBM1 and one on FBM2
	fsm->master_LBA_Blocked();
    Workpiece* wp1 = fsm->data->wpManager->getHeadOfArea(AreaType::AREA_A);
	fsm->master_LBA_Blocked();

	fsm->master_LBA_Unblocked();
	fsm->master_heightResultReceived(EventType::HM_M_WS_F, 20);
	fsm->master_LBW_Blocked();
	fsm->master_LBW_Unblocked();
	fsm->master_LBE_Blocked();
	fsm->master_LBE_Unblocked();
	fsm->slave_LBA_Blocked();
	fsm->slave_LBA_Unblocked();
	fsm->slave_heightResultReceived(EventType::HM_M_WS_F, 21);
    EXPECT_FALSE(fsm->data->wpManager->isFBM_MEmpty());
    EXPECT_FALSE(fsm->data->wpManager->isFBM_SEmpty());

    // Error occurred - check if both FBM's are still occupied
    fsm->nonSelfSolvableErrorOccurred();
    EXPECT_EQ(MainState::ERROR, fsm->getCurrentState());
    fsm->master_btnReset_Pressed();
    fsm->master_btnStart_PressedShort();
    fsm->master_btnStart_PressedShort();
    EXPECT_FALSE(fsm->data->wpManager->isFBM_MEmpty());
    EXPECT_FALSE(fsm->data->wpManager->isFBM_SEmpty());

    // Back to Running - change next expected workpiece type
    EXPECT_EQ(WorkpieceType::WS_F, fsm->data->wpManager->getNextWorkpieceType());
    fsm->slave_LBW_Blocked();
    EXPECT_FALSE(wp1->sortOut);
    fsm->slave_LBW_Unblocked();
    fsm->slave_LBE_Blocked();
    fsm->slave_LBE_Unblocked();
    WorkpieceType nextType = fsm->data->wpManager->getNextWorkpieceType();
    EXPECT_EQ(WorkpieceType::WS_BOM, fsm->data->wpManager->getNextWorkpieceType());

    // Error occurred - check if next expected workpiece type is preserved
    fsm->nonSelfSolvableErrorOccurred();
    EXPECT_EQ(MainState::ERROR, fsm->getCurrentState());
    fsm->master_btnReset_Pressed();
    fsm->master_btnStart_PressedShort();
    fsm->master_btnStart_PressedShort();
    EXPECT_EQ(MainState::RUNNING, fsm->getCurrentState());
    EXPECT_EQ(WorkpieceType::WS_BOM, fsm->data->wpManager->getNextWorkpieceType());
}
