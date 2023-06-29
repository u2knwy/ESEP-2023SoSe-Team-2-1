/*
 * RunningFSM.cpp
 *
 *  Created on: 22.06.2023
 *      Author: Maik
 */

#include "mocks/EventManagerMock.h"
#include "mocks/EventSenderMock.h"

#include "configuration/Configuration.h"
#include "events/EventSender.h"
#include "data/Workpiece.h"
#include "data/WorkpieceManager.h"
#include "data/workpiecetype_enum.h"
#include "logic/main_fsm/MainContext.h"
#include <gtest/gtest.h>
#include <thread>
#include <chrono>


class IntegrationTest_Running : public ::testing::Test {
  protected:
	std::shared_ptr<EventManagerMock> evm = std::make_shared<EventManagerMock>();
    IEventSender* sender;
    MainActions* mainActions;
    MainContext* fsm;
    WorkpieceManager* wpm;

    /**
     * OPTIONAL: Prepare objects before each test
     */
    void SetUp() override {
    	Configuration::getInstance().setDesiredWorkpieceOrder({WS_F, WS_BOM, WS_OB});
    	sender = new EventSenderMock();
        mainActions = new MainActions(evm, sender);
        fsm = new MainContext(mainActions);
    	wpm = fsm->data->wpManager;
    	fsm->master_btnStart_PressedShort();
    	evm->clearLastHandledEvents();
    	clearBelt();
    	Configuration::getInstance().setOffsetCalibration(3600);
    	Configuration::getInstance().setReferenceCalibration(2500);
    }

    /**
     * OPTIONAL: Release any resources allocated in SetUp() method
     */
    void TearDown() override {
    	delete fsm;
    }

    void clearBelt() {
    	wpm->reset_wpm();
    }

    void wpRunUntilSwitchAtFBM1(EventType heightType, float avg, bool metal) {
    	fsm->master_LBA_Blocked();
    	fsm->master_LBA_Unblocked();
    	fsm->master_heightResultReceived(heightType, avg);
    	if(metal)
    		fsm->master_metalDetected();
    	fsm->master_LBW_Blocked();
    }

    void wpRunUntilRemovedAtFBM2(EventType heightType, float avg, bool metal) {
    	fsm->master_LBA_Blocked();
    	fsm->master_LBA_Unblocked();
    	fsm->master_heightResultReceived(heightType, avg);
    	if(metal)
    		fsm->master_metalDetected();
    	fsm->master_LBW_Blocked();
    	fsm->master_LBW_Unblocked();
    	fsm->master_LBE_Blocked();
    	fsm->master_LBE_Unblocked();
    	fsm->slave_LBA_Blocked();
    	fsm->slave_LBA_Unblocked();
    	fsm->slave_heightResultReceived(heightType, avg);
    	if(metal)
    		fsm->slave_metalDetected();
    	fsm->slave_LBW_Blocked();
    	fsm->slave_LBW_Unblocked();
    	fsm->slave_LBE_Blocked();
    	fsm->slave_LBE_Unblocked();
    }
};

TEST_F(IntegrationTest_Running, RampBlockedWhenRunningStarted) {
	evm->clearLastHandledEvents();
	EXPECT_FALSE(wpm->getRamp_one());
	EXPECT_FALSE(wpm->getRamp_two());

	fsm->master_LBR_Unblocked();
	fsm->slave_LBR_Unblocked();
	fsm->master_btnStop_Pressed();
	EXPECT_EQ(MainState::STANDBY, fsm->getCurrentState());

	fsm->master_LBR_Blocked();
	fsm->slave_LBR_Blocked();

	evm->clearLastHandledEvents();
	fsm->master_btnStart_PressedShort();
	EXPECT_EQ(MainState::RUNNING, fsm->getCurrentState());
	EXPECT_TRUE(wpm->getRamp_one());
	EXPECT_TRUE(wpm->getRamp_two());

	std::this_thread::sleep_for(std::chrono::milliseconds(1100));
	EXPECT_TRUE(evm->lastHandledEventsContain(Event{EventType::LAMP_M_YELLOW, (int) LampState::FLASHING_SLOW}));
	EXPECT_TRUE(evm->lastHandledEventsContain(Event{EventType::LAMP_S_YELLOW, (int) LampState::FLASHING_SLOW}));
}

TEST_F(IntegrationTest_Running, IfRampOccupiedDisplayWarning) {
	// both ramps are free
	EXPECT_FALSE(wpm->getRamp_one());
	EXPECT_FALSE(wpm->getRamp_two());

	// FBM1 Ramp is blocked
	evm->clearLastHandledEvents();
	fsm->master_LBR_Blocked();
	EXPECT_TRUE(wpm->getRamp_one());
	EXPECT_FALSE(wpm->getRamp_two());
	// after 1 sec. -> display warning
	std::this_thread::sleep_for(std::chrono::milliseconds(1100));
	EXPECT_TRUE(evm->lastHandledEventsContain(Event{EventType::LAMP_M_YELLOW, (int) LampState::FLASHING_SLOW}));
	// is unblocked -> ramp free again
	fsm->master_LBR_Unblocked();
	EXPECT_FALSE(wpm->getRamp_one());
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	EXPECT_TRUE(evm->lastHandledEventsContain(Event{EventType::LAMP_M_YELLOW, (int) LampState::OFF}));

	// FBM2 Ramp is blocked
	evm->clearLastHandledEvents();
	fsm->slave_LBR_Blocked();
	EXPECT_TRUE(wpm->getRamp_two());
	EXPECT_FALSE(wpm->getRamp_one());
	// after 1 sec. -> display warning
	std::this_thread::sleep_for(std::chrono::milliseconds(1100));
	EXPECT_TRUE(evm->lastHandledEventsContain(Event{EventType::LAMP_S_YELLOW, (int) LampState::FLASHING_SLOW}));
	// is unblocked -> ramp free again
	fsm->slave_LBR_Unblocked();
	EXPECT_FALSE(wpm->getRamp_two());
	EXPECT_TRUE(evm->lastHandledEventsContain(Event{EventType::LAMP_S_YELLOW, (int) LampState::OFF}));
}

TEST_F(IntegrationTest_Running, NewWorkpieceInsertedStartMotor) {
	fsm->master_btnStop_Pressed();
	evm->clearLastHandledEvents();
	// Go into Running - motor must be stopped
	fsm->master_btnStart_PressedShort();
	EXPECT_TRUE(evm->lastHandledEventsContain(Event{MODE_RUNNING}));
	EXPECT_FALSE(evm->lastHandledEventsContain(Event{MOTOR_M_RIGHT_REQ, 1}));

	// Insert workpiece - motor must run fast
	evm->clearLastHandledEvents();
	fsm->master_LBA_Blocked();
	// Motor must be running
	EXPECT_TRUE(evm->lastHandledEventsContain(Event{MOTOR_M_RIGHT_REQ, 1}));
	EXPECT_FALSE(evm->lastHandledEventsContain(Event{MOTOR_M_RIGHT_REQ, 0}));
}

TEST_F(IntegrationTest_Running, WorkpieceGetHeadOfArea) {
	// Insert 3 workpieces - check if head of Area A is always the first one
	fsm->master_LBA_Blocked();
	Workpiece* wp1 = wpm->getHeadOfArea(AreaType::AREA_A);
	EXPECT_EQ(1, wp1->id);
	fsm->master_LBA_Blocked();
	fsm->master_LBA_Blocked();
	wp1 = wpm->getHeadOfArea(AreaType::AREA_A);
	EXPECT_EQ(1, wp1->id);

	// WS1 passed to HeightSensor - must be in AREA_B
	fsm->master_heightResultReceived(EventType::HM_M_WS_F, 20);
	wp1 = wpm->getHeadOfArea(AreaType::AREA_B);
	EXPECT_EQ(1, wp1->id);

	fsm->master_metalDetected();
	wp1 = wpm->getHeadOfArea(AreaType::AREA_B);
	EXPECT_EQ(1, wp1->id);
	fsm->master_LBW_Blocked();
	EXPECT_EQ(false, wp1->sortOut);
	fsm->master_LBW_Unblocked();
	wp1 = wpm->getHeadOfArea(AreaType::AREA_C);
	EXPECT_EQ(1, wp1->id);

	// Arrived at FBM1 end -> still in Area C
	fsm->master_LBE_Blocked();
	wp1 = wpm->getHeadOfArea(AreaType::AREA_C);
	EXPECT_EQ(1, wp1->id);

	// Left FBM1 -> must be passed to Area D
	fsm->master_LBE_Unblocked();
	wp1 = wpm->getHeadOfArea(AreaType::AREA_D);
	EXPECT_EQ(1, wp1->id);

	fsm->slave_LBA_Blocked();
	wp1 = wpm->getHeadOfArea(AreaType::AREA_D);
	EXPECT_EQ(1, wp1->id);
	fsm->slave_LBA_Unblocked();
	wp1 = wpm->getHeadOfArea(AreaType::AREA_D);
	EXPECT_EQ(1, wp1->id);

	fsm->slave_heightResultReceived(EventType::HM_M_WS_F, 20);
	wp1 = wpm->getHeadOfArea(AreaType::AREA_D);
	EXPECT_EQ(1, wp1->id);
	fsm->slave_metalDetected();
	wp1 = wpm->getHeadOfArea(AreaType::AREA_D);
	EXPECT_EQ(1, wp1->id);
	fsm->slave_LBW_Blocked();
	EXPECT_EQ(false, wp1->sortOut);
	fsm->slave_LBW_Unblocked();

	fsm->slave_LBE_Blocked();
	wp1 = wpm->getHeadOfArea(AreaType::AREA_D);
	EXPECT_EQ(1, wp1->id);
	fsm->slave_LBE_Unblocked();
	wp1 = wpm->getHeadOfArea(AreaType::AREA_D);

	// Left FBM2 -> must be removed
	EXPECT_EQ(nullptr, wp1);
}

TEST_F(IntegrationTest_Running, WorkpieceDefaultValues) {
	fsm->master_LBA_Blocked();
	Workpiece* wp1 = wpm->getHeadOfArea(AreaType::AREA_A);
	EXPECT_EQ(1, wp1->id);
	EXPECT_EQ(WorkpieceType::WS_UNKNOWN, wp1->M_type);
	EXPECT_EQ(WorkpieceType::WS_UNKNOWN, wp1->S_type);
	EXPECT_EQ(0.0, wp1->avgHeight);
	EXPECT_EQ(0.0, wp1->maxHeightFBM2);
	EXPECT_EQ(false, wp1->metal);
	EXPECT_EQ(false, wp1->flipped);
	EXPECT_EQ(false, wp1->sortOut);
}

TEST_F(IntegrationTest_Running, SetHeightResult) {
	fsm->master_LBA_Blocked();
	fsm->master_heightResultReceived(EventType::HM_M_WS_F, 20.5);
	Workpiece* wp1 = wpm->getHeadOfArea(AreaType::AREA_B);
	EXPECT_EQ(WorkpieceType::WS_F, wp1->M_type);
	EXPECT_EQ(20.5, wp1->avgHeight);
}

TEST_F(IntegrationTest_Running, SetMetalResult) {
	fsm->master_LBA_Blocked();
	fsm->master_LBA_Unblocked();
	fsm->master_heightResultReceived(EventType::HM_M_WS_BOM, 20);
	Workpiece* wp1 = wpm->getHeadOfArea(AreaType::AREA_B);
	EXPECT_EQ(WorkpieceType::WS_BOM, wp1->M_type);
	EXPECT_EQ(false, wp1->metal);
	fsm->master_metalDetected();
	EXPECT_EQ(true, wp1->metal);
	EXPECT_EQ(WorkpieceType::WS_BUM, wp1->M_type);
}

TEST_F(IntegrationTest_Running, WorkpieceInOrderLetPass) {
	Configuration::getInstance().setDesiredWorkpieceOrder({WS_F, WS_BOM, WS_OB});
	wpRunUntilSwitchAtFBM1(EventType::HM_M_WS_F, 21, false);
	Workpiece* wp1 = wpm->getHeadOfArea(AreaType::AREA_C);
	EXPECT_EQ(WorkpieceType::WS_F, wp1->M_type);
	EXPECT_EQ(21, wp1->avgHeight);
	EXPECT_FALSE(wp1->sortOut);
}

TEST_F(IntegrationTest_Running, WorkpieceNotInOrderSortOutAtFBM1) {
	// first one is flat - next one will be BOM
	wpRunUntilRemovedAtFBM2(EventType::HM_M_WS_F, 20, false);
	EXPECT_EQ(WorkpieceType::WS_BOM, wpm->getNextWorkpieceType());

	// make sure ramp is free
	fsm->master_LBR_Unblocked();
	EXPECT_FALSE(wpm->getRamp_one());

	// WS_F run until switch
	wpRunUntilSwitchAtFBM1(EventType::HM_M_WS_F, 20, false);
	Workpiece* wp1 = wpm->getHeadOfArea(AreaType::AREA_B);
	EXPECT_EQ(WorkpieceType::WS_F, wp1->M_type);
	EXPECT_EQ(20, wp1->avgHeight);
	EXPECT_TRUE(wp1->sortOut);
}

TEST_F(IntegrationTest_Running, WorkpieceNotInOrderSortOutAtFBM2) {
	wpRunUntilSwitchAtFBM1(EventType::HM_M_WS_BOM, 25, false);
	Workpiece* wp1 = wpm->getHeadOfArea(AreaType::AREA_C);
	EXPECT_EQ(WorkpieceType::WS_BOM, wp1->M_type);
	EXPECT_EQ(25, wp1->avgHeight);
	EXPECT_FALSE(wp1->sortOut);
}

TEST_F(IntegrationTest_Running, CalibrationInvalidDoNotGoToRunning) {
	// Go to Standby
	fsm->master_btnStop_Pressed();

	// Set invalid calibration values (default)
	Configuration::getInstance().setOffsetCalibration(3333);
	Configuration::getInstance().setReferenceCalibration(2222);

	// Try to go to Running mode - we must stay in Standby
	fsm->master_btnStart_PressedShort();
	EXPECT_EQ(MainState::STANDBY, fsm->getCurrentState());

	// Calibration values valid - go to Running
	Configuration::getInstance().setOffsetCalibration(3600);
	Configuration::getInstance().setReferenceCalibration(2200);
	fsm->master_btnStart_PressedShort();
	EXPECT_EQ(MainState::RUNNING, fsm->getCurrentState());
}

TEST_F(IntegrationTest_Running, RunningToStandbyToRunningResume) {
	// Insert two workpieces
	fsm->master_LBA_Blocked();
	EXPECT_TRUE(evm->lastHandledEventsContain(Event{EventType::MOTOR_M_RIGHT_REQ, 1}));
	fsm->master_LBA_Blocked();

	// WP1 go to FBM2
	fsm->master_LBA_Unblocked();
	fsm->master_heightResultReceived(EventType::HM_M_WS_F, 21.0);
	fsm->master_LBW_Blocked();
	fsm->master_LBW_Unblocked();
	fsm->master_LBE_Blocked();
	fsm->master_LBE_Unblocked();
	EXPECT_FALSE(evm->lastHandledEventsContain(Event{EventType::MOTOR_M_RIGHT_REQ, 0}));
	fsm->slave_LBA_Blocked();

	// Go to Standby and Running again - state must be continued
	fsm->master_btnStop_Pressed();
	EXPECT_FALSE(evm->lastHandledEventsContain(Event{EventType::MOTOR_M_RIGHT_REQ, 0}));
	EXPECT_FALSE(evm->lastHandledEventsContain(Event{EventType::MOTOR_S_RIGHT_REQ, 0}));
	evm->clearLastHandledEvents();
	fsm->master_btnStart_PressedShort();
	EXPECT_FALSE(wpm->isFBM_MEmpty());
	EXPECT_FALSE(wpm->isFBM_SEmpty());
	EXPECT_FALSE(evm->lastHandledEventsContain(Event{EventType::MOTOR_M_RIGHT_REQ, 0}));
	EXPECT_FALSE(evm->lastHandledEventsContain(Event{EventType::MOTOR_S_RIGHT_REQ, 0}));

	Workpiece* wp1 = wpm->getHeadOfArea(AreaType::AREA_A);
	EXPECT_NE(nullptr, wp1);
	EXPECT_EQ(2, wp1->id);
	Workpiece* wp2 = wpm->getHeadOfArea(AreaType::AREA_D);
	EXPECT_NE(nullptr, wp2);
	EXPECT_EQ(1, wp2->id);
}

TEST_F(IntegrationTest_Running, RunningToStandbyToRunningBothFBMFree) {
	// Insert two 'OK' workpieces which will be removed at FBM2
	wpRunUntilRemovedAtFBM2(EventType::HM_M_WS_F, 21.0, false);
	wpRunUntilRemovedAtFBM2(EventType::HM_M_WS_BOM, 21.0, false);

	// Go to Standby and Running again - state must be continued
	evm->clearLastHandledEvents();
	fsm->master_btnStop_Pressed();
	EXPECT_FALSE(evm->lastHandledEventsContain(Event{EventType::MOTOR_M_RIGHT_REQ, 0}));
	EXPECT_FALSE(evm->lastHandledEventsContain(Event{EventType::MOTOR_S_RIGHT_REQ, 0}));
	EXPECT_TRUE(evm->lastHandledEventsContain(Event{EventType::MOTOR_M_STOP_REQ, 1}));
	EXPECT_TRUE(evm->lastHandledEventsContain(Event{EventType::MOTOR_S_STOP_REQ, 1}));
	fsm->master_btnStart_PressedShort();
	EXPECT_TRUE(wpm->isFBM_MEmpty());
	EXPECT_TRUE(wpm->isFBM_SEmpty());
	EXPECT_TRUE(evm->lastHandledEventsContain(Event{EventType::MOTOR_M_STOP_REQ, 0}));
	EXPECT_TRUE(evm->lastHandledEventsContain(Event{EventType::MOTOR_S_STOP_REQ, 0}));
	EXPECT_FALSE(evm->lastHandledEventsContain(Event{MOTOR_M_RIGHT_REQ, 0}));
	EXPECT_FALSE(evm->lastHandledEventsContain(Event{MOTOR_S_RIGHT_REQ, 0}));
	EXPECT_FALSE(evm->lastHandledEventsContain(Event{MOTOR_M_RIGHT_REQ, 1}));
	EXPECT_FALSE(evm->lastHandledEventsContain(Event{MOTOR_S_RIGHT_REQ, 1}));
}
