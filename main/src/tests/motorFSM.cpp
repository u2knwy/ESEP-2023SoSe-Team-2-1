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
	std::shared_ptr<MotorContext> fsm;
	std::shared_ptr<EventManager> eventManager;

  void SetUp() override {
	  eventManager = std::make_shared<EventManager>();
	  fsm = std::make_shared<MotorContext>(eventManager, true);
	  Configuration::getInstance().setMaster(true);
  }

  void TearDown() override {
  }

};

static Event createEvent(EventType type, bool motorFlag) {
	Event ev;
	ev.type = type;
	ev.data = (int) motorFlag;
	return ev;
}

// When launching the FSM -> Current state must be STOPPED
TEST_F(MotorFSM_Test, StartStateStopped) {
	EXPECT_EQ(MotorState::STOPPED, fsm->getCurrentState());
	// Clear "Stop" flag -> still stopped
	fsm->handleEvent(createEvent(EventType::MOTOR_M_SET_STOP, false));
	EXPECT_EQ(MotorState::STOPPED, fsm->getCurrentState());
}

// STOPPED -> FAST
TEST_F(MotorFSM_Test, StoppedToFast) {
	// Clear "Stop" flag and set "Fast" flag -> must be in state "right fast"
	fsm->handleEvent(createEvent(EventType::MOTOR_M_SET_STOP, false));
	fsm->handleEvent(createEvent(EventType::MOTOR_M_SET_FAST, true));
	EXPECT_EQ(MotorState::RIGHT_FAST, fsm->getCurrentState());
}

// STOPPED -> SLOW
TEST_F(MotorFSM_Test, StoppedToSlow) {
	// Clear "Stop" flag and set "Slow" flag -> must be in state "right slow"
	fsm->handleEvent(createEvent(EventType::MOTOR_M_SET_STOP, false));
	EXPECT_EQ(MotorState::STOPPED, fsm->getCurrentState());
	fsm->handleEvent(createEvent(EventType::MOTOR_M_SET_SLOW, true));
	EXPECT_EQ(MotorState::RIGHT_SLOW, fsm->getCurrentState());
}

// FAST -> SLOW
TEST_F(MotorFSM_Test, FastToSlow) {
	// Clear "Stop" flag and set "Fast" flag -> must be in state "right fast"
	fsm->handleEvent(createEvent(EventType::MOTOR_M_SET_STOP, false));
	fsm->handleEvent(createEvent(EventType::MOTOR_M_SET_FAST, true));
	EXPECT_EQ(MotorState::RIGHT_FAST, fsm->getCurrentState());

	// Set "Slow" flag -> must be in state "right slow"
	fsm->handleEvent(createEvent(EventType::MOTOR_M_SET_SLOW, true));
	EXPECT_EQ(MotorState::RIGHT_SLOW, fsm->getCurrentState());
}

// SLOW -> FAST
TEST_F(MotorFSM_Test, SlowToFast) {
	// Clear "Stop" flag and set "Fast" and "Slow" flag -> must be in state "right slow"
	fsm->handleEvent(createEvent(EventType::MOTOR_M_SET_STOP, false));
	fsm->handleEvent(createEvent(EventType::MOTOR_M_SET_FAST, true));
	fsm->handleEvent(createEvent(EventType::MOTOR_M_SET_SLOW, true));
	EXPECT_EQ(MotorState::RIGHT_SLOW, fsm->getCurrentState());

	// Clear "Slow" flag -> must be in state "right fast"
	fsm->handleEvent(createEvent(EventType::MOTOR_M_SET_SLOW, false));
	EXPECT_EQ(MotorState::RIGHT_FAST, fsm->getCurrentState());
}
