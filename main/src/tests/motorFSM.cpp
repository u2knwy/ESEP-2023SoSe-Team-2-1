/*
 * motorFSM.cpp
 *
 *  Created on: 09.06.2023
 *      Author: Maik
 */
#include <gtest/gtest.h>
#include "logic/main_fsm/MainContext.h"
#include "logic/motor_fsm/MotorContext.h"
#include "events/EventManager.h"
#include "configuration/Configuration.h"

class MotorFSM_Test : public ::testing::Test {
protected:
	std::shared_ptr<MotorContext> motorfsm_master;
	std::shared_ptr<MotorContext> motorfsm_slave;
	std::shared_ptr<EventManager> eventManager;

  void SetUp() override {
	  eventManager = std::make_shared<EventManager>();
	  motorfsm_master = std::make_shared<MotorContext>(eventManager, true);
	  motorfsm_slave = std::make_shared<MotorContext>(eventManager, false);
	  Configuration::getInstance().setMaster(true);
  }

  void TearDown() override {
  }

};

static Event createMotorRequest(EventType type, bool motorFlag) {
	Event ev;
	ev.type = type;
	ev.data = (int) motorFlag;
	return ev;
}

// When launching the FSM -> Current state must be STOPPED
TEST_F(MotorFSM_Test, StartStateStopped) {
	EXPECT_EQ(MotorState::STOPPED, motorfsm_master->getCurrentState());
	EXPECT_EQ(MotorState::STOPPED, motorfsm_slave->getCurrentState());
	// Clear "Stop" flag -> still stopped
	motorfsm_master->handleEvent(createMotorRequest(EventType::MOTOR_M_STOP_REQ, false));
	motorfsm_slave->handleEvent(createMotorRequest(EventType::MOTOR_S_STOP_REQ, false));
	EXPECT_EQ(MotorState::STOPPED, motorfsm_master->getCurrentState());
	EXPECT_EQ(MotorState::STOPPED, motorfsm_slave->getCurrentState());
}

// STOPPED -> FAST -> STOPPED
TEST_F(MotorFSM_Test, StoppedToFastToStopped) {
	EXPECT_EQ(MotorState::STOPPED, motorfsm_master->getCurrentState());
	EXPECT_EQ(MotorState::STOPPED, motorfsm_slave->getCurrentState());

	// Clear "Stop" flag and set "Fast" flag -> must be in state "right fast"
	motorfsm_master->handleEvent(createMotorRequest(EventType::MOTOR_M_STOP_REQ, false));
	motorfsm_slave->handleEvent(createMotorRequest(EventType::MOTOR_S_STOP_REQ, false));
	motorfsm_master->handleEvent(createMotorRequest(EventType::MOTOR_M_RIGHT_REQ, true));
	motorfsm_slave->handleEvent(createMotorRequest(EventType::MOTOR_S_RIGHT_REQ, false));
	EXPECT_EQ(MotorState::RIGHT_FAST, motorfsm_master->getCurrentState());
	EXPECT_EQ(MotorState::STOPPED, motorfsm_slave->getCurrentState());

	motorfsm_master->handleEvent(createMotorRequest(EventType::MOTOR_M_RIGHT_REQ, false));
	motorfsm_slave->handleEvent(createMotorRequest(EventType::MOTOR_S_RIGHT_REQ, true));
	EXPECT_EQ(MotorState::STOPPED, motorfsm_master->getCurrentState());
	EXPECT_EQ(MotorState::RIGHT_FAST, motorfsm_slave->getCurrentState());
}

// STOPPED -> SLOW
TEST_F(MotorFSM_Test, StoppedToSlowToStopped) {
	EXPECT_EQ(MotorState::STOPPED, motorfsm_master->getCurrentState());

	// Clear "Stop" flag and set "Slow" flag -> must be in state "right slow"
	motorfsm_master->handleEvent(createMotorRequest(EventType::MOTOR_M_STOP_REQ, false));
	motorfsm_master->handleEvent(createMotorRequest(EventType::MOTOR_M_SLOW_REQ, true));
	motorfsm_master->handleEvent(createMotorRequest(EventType::MOTOR_M_RIGHT_REQ, true));
	EXPECT_EQ(MotorState::RIGHT_SLOW, motorfsm_master->getCurrentState());

	motorfsm_master->handleEvent(createMotorRequest(EventType::MOTOR_M_RIGHT_REQ, false));
	EXPECT_EQ(MotorState::STOPPED, motorfsm_master->getCurrentState());
}

// FAST -> SLOW
TEST_F(MotorFSM_Test, FastToSlow) {
	// Clear "Stop" flag and set "Fast" flag -> must be in state "right fast"
	motorfsm_master->handleEvent(createMotorRequest(EventType::MOTOR_M_STOP_REQ, false));
	EXPECT_EQ(MotorState::STOPPED, motorfsm_master->getCurrentState());
	motorfsm_master->handleEvent(createMotorRequest(EventType::MOTOR_M_RIGHT_REQ, true));
	EXPECT_EQ(MotorState::RIGHT_FAST, motorfsm_master->getCurrentState());

	// Set "Slow" flag -> must be in state "right slow"
	motorfsm_master->handleEvent(createMotorRequest(EventType::MOTOR_M_SLOW_REQ, true));
	EXPECT_EQ(MotorState::RIGHT_SLOW, motorfsm_master->getCurrentState());
}

// SLOW -> FAST
TEST_F(MotorFSM_Test, SlowToFast) {
	// Clear "Stop" flag and set "Fast" and "Slow" flag -> must be in state "right slow"
	motorfsm_master->handleEvent(createMotorRequest(EventType::MOTOR_M_STOP_REQ, false));
	motorfsm_master->handleEvent(createMotorRequest(EventType::MOTOR_M_RIGHT_REQ, true));
	motorfsm_master->handleEvent(createMotorRequest(EventType::MOTOR_M_SLOW_REQ, true));
	EXPECT_EQ(MotorState::RIGHT_SLOW, motorfsm_master->getCurrentState());

	// Clear "Slow" flag -> must be in state "right fast"
	motorfsm_master->handleEvent(createMotorRequest(EventType::MOTOR_M_SLOW_REQ, false));
	EXPECT_EQ(MotorState::RIGHT_FAST, motorfsm_master->getCurrentState());
}
