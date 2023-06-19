/*
 * MainContext.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "MainContext.h"
#include "MainActions.h"
#include "MainContextData.h"
#include "logger/logger.hpp"
#include "states/Standby.h"

#include <memory>

MainContext::MainContext(std::shared_ptr<EventManager> mngr)
    : eventManager(mngr) {
    this->actions = new MainActions(mngr);
    this->data = new MainContextData();
    this->state = new Standby();
    state->setAction(actions);
    state->setData(data);
    state->entry();
    subscribeToEvents();
}

MainContext::~MainContext() {
    delete state;
    delete data;
    delete actions;
}

void MainContext::subscribeToEvents() {
    eventManager->subscribe(
        EventType::START_M_SHORT,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::START_M_LONG,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::STOP_M_SHORT,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::RESET_M_SHORT,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::ESTOP_M_PRESSED,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::ESTOP_M_RELEASED,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));

    eventManager->subscribe(
        EventType::START_S_SHORT,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::START_S_LONG,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::STOP_S_SHORT,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::RESET_S_SHORT,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::ESTOP_S_PRESSED,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::ESTOP_S_RELEASED,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));

    eventManager->subscribe(
        EventType::LBA_M_BLOCKED,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::LBA_M_UNBLOCKED,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::LBW_M_BLOCKED,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::LBW_M_UNBLOCKED,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::LBE_M_BLOCKED,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::LBE_M_UNBLOCKED,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::LBR_M_BLOCKED,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::LBR_M_UNBLOCKED,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));

    eventManager->subscribe(
        EventType::LBA_S_BLOCKED,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::LBA_S_UNBLOCKED,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::LBW_S_BLOCKED,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::LBW_S_UNBLOCKED,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::LBE_S_BLOCKED,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::LBE_S_UNBLOCKED,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::LBR_S_BLOCKED,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::LBR_S_UNBLOCKED,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));

    // Height Sensor
    eventManager->subscribe(
        EventType::HM_M_WS_UNKNOWN,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::HM_M_WS_F,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::HM_M_WS_OB,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::HM_M_WS_BOM,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::HM_S_WS_UNKNOWN,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::HM_S_WS_F,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::HM_S_WS_OB,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::HM_S_WS_BOM,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));

    // Subscribe to errors and error-solved event
    eventManager->subscribe(
        EventType::ERROR_M_SELF_SOLVABLE,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::ERROR_M_MAN_SOLVABLE,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::ERROR_M_SELF_SOLVED,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::ERROR_S_SELF_SOLVABLE,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::ERROR_S_MAN_SOLVABLE,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::ERROR_S_SELF_SOLVED,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));

    eventManager->subscribe(
        EventType::WD_M_CONN_LOST,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::WD_M_CONN_REESTABLISHED,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::WD_S_CONN_LOST,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
    eventManager->subscribe(
        EventType::WD_S_CONN_REESTABLISHED,
        std::bind(&MainContext::handleEvent, this, std::placeholders::_1));
}

void MainContext::handleEvent(Event event) {
    Logger::debug("MainFSM handle Event: " + EVENT_TO_STRING(event.type));
    switch (event.type) {
    case EventType::START_M_SHORT:
        state->master_btnStart_PressedShort();
        break;
    case EventType::START_M_LONG:
        state->master_btnStart_PressedLong();
        break;
    case EventType::STOP_M_SHORT:
        state->master_btnStop_Pressed();
        break;
    case EventType::RESET_M_SHORT:
        state->master_btnReset_Pressed();
        break;
    case EventType::ESTOP_M_PRESSED:
        state->master_EStop_Pressed();
        break;
    case EventType::ESTOP_M_RELEASED:
        state->master_EStop_Released();
        break;
    case EventType::START_S_SHORT:
        state->slave_btnStart_PressedShort();
        break;
    case EventType::START_S_LONG:
        state->slave_btnStart_PressedLong();
        break;
    case EventType::STOP_S_SHORT:
        state->slave_btnStop_Pressed();
        break;
    case EventType::RESET_S_SHORT:
        state->slave_btnReset_Pressed();
        break;
    case EventType::ESTOP_S_PRESSED:
        state->slave_EStop_Pressed();
        break;
    case EventType::ESTOP_S_RELEASED:
        state->slave_EStop_Released();
        break;
    case EventType::LBA_M_BLOCKED:
        state->master_LBA_Blocked();
        break;
    case EventType::LBA_M_UNBLOCKED:
        state->master_LBA_Unblocked();
        break;
    case EventType::LBW_M_BLOCKED:
        state->master_LBW_Blocked();
        break;
    case EventType::LBW_M_UNBLOCKED:
        state->master_LBW_Unblocked();
        break;
    case EventType::LBE_M_BLOCKED:
        state->master_LBE_Blocked();
        break;
    case EventType::LBE_M_UNBLOCKED:
        state->master_LBE_Unblocked();
        break;
    case EventType::LBR_M_BLOCKED:
        state->master_LBR_Blocked();
        break;
    case EventType::LBR_M_UNBLOCKED:
        state->master_LBR_Unblocked();
        break;
    case EventType::LBA_S_BLOCKED:
        state->slave_LBA_Blocked();
        break;
    case EventType::LBA_S_UNBLOCKED:
        state->slave_LBA_Unblocked();
        break;
    case EventType::LBW_S_BLOCKED:
        state->slave_LBW_Blocked();
        break;
    case EventType::LBW_S_UNBLOCKED:
        state->slave_LBW_Unblocked();
        break;
    case EventType::LBE_S_BLOCKED:
        state->slave_LBE_Blocked();
        break;
    case EventType::LBE_S_UNBLOCKED:
        state->slave_LBE_Unblocked();
        break;
    case EventType::LBR_S_BLOCKED:
        state->slave_LBR_Blocked();
        break;
    case EventType::LBR_S_UNBLOCKED:
        state->slave_LBR_Unblocked();
        break;
    case EventType::HM_M_WS_F:
    case EventType::HM_M_WS_OB:
    case EventType::HM_M_WS_BOM:
    case EventType::HM_M_WS_UNKNOWN: {
        state->master_heightResultReceived(event.type,
                                           ((float) event.data) / 10);
        break;
    }
    case EventType::HM_S_WS_F:
    case EventType::HM_S_WS_OB:
    case EventType::HM_S_WS_BOM:
    case EventType::HM_S_WS_UNKNOWN: {
        state->slave_heightResultReceived(event.type,
                                          ((float) event.data) / 10);
        break;
    }
    case EventType::MD_M_PAYLOAD:
        state->master_metalDetected();
        break;
    case EventType::MD_S_PAYLOAD:
        state->slave_metalDetected();
        break;
    case EventType::ERROR_M_SELF_SOLVABLE:
    case EventType::ERROR_S_SELF_SOLVABLE:
    case EventType::WD_M_CONN_LOST:
    case EventType::WD_S_CONN_LOST:
        selfSolvableErrorOccurred();
        break;
    case EventType::ERROR_M_MAN_SOLVABLE:
    case EventType::ERROR_S_MAN_SOLVABLE:
        nonSelfSolvableErrorOccurred();
        break;
    case EventType::ERROR_M_SELF_SOLVED:
    case EventType::ERROR_S_SELF_SOLVED:
    case EventType::WD_M_CONN_REESTABLISHED:
    case EventType::WD_S_CONN_REESTABLISHED:
        errorSelfSolved();
        break;
    default:
        Logger::warn("[MainFSM] Event was not handled: " +
                     EVENT_TO_STRING(event.type));
        break;
    }
}

MainState MainContext::getCurrentState() { return state->getCurrentState(); }

void MainContext::master_LBA_Blocked() { state->master_LBA_Blocked(); }

void MainContext::master_LBA_Unblocked() { state->master_LBA_Unblocked(); }

void MainContext::master_LBW_Blocked() { state->master_LBW_Blocked(); }

void MainContext::master_LBW_Unblocked() { state->master_LBW_Unblocked(); }

void MainContext::master_LBE_Blocked() { state->master_LBE_Blocked(); }

void MainContext::master_LBE_Unblocked() { state->master_LBE_Unblocked(); }

void MainContext::master_LBR_Blocked() { state->master_LBR_Blocked(); }

void MainContext::master_LBR_Unblocked() { state->master_LBR_Unblocked(); }

void MainContext::slave_LBA_Blocked() { state->slave_LBA_Blocked(); }

void MainContext::slave_LBA_Unblocked() { state->slave_LBA_Unblocked(); }

void MainContext::slave_LBW_Blocked() { state->slave_LBW_Blocked(); }

void MainContext::slave_LBW_Unblocked() { state->slave_LBW_Unblocked(); }

void MainContext::slave_LBE_Blocked() { state->slave_LBE_Blocked(); }

void MainContext::slave_LBE_Unblocked() { state->slave_LBE_Unblocked(); }

void MainContext::slave_LBR_Blocked() { state->slave_LBR_Blocked(); }

void MainContext::slave_LBR_Unblocked() { state->slave_LBR_Unblocked(); }

void MainContext::master_btnStart_PressedShort() {
    state->master_btnStart_PressedShort();
}

void MainContext::master_btnStart_PressedLong() {
    state->master_btnStart_PressedLong();
}

void MainContext::master_btnStop_Pressed() { state->master_btnStop_Pressed(); }

void MainContext::master_btnReset_Pressed() {
    state->master_btnReset_Pressed();
}

void MainContext::slave_btnStart_PressedShort() {
    state->slave_btnStart_PressedShort();
}

void MainContext::slave_btnStart_PressedLong() {
    state->slave_btnStart_PressedLong();
}

void MainContext::slave_btnStop_Pressed() { state->slave_btnStop_Pressed(); }

void MainContext::slave_btnReset_Pressed() { state->slave_btnReset_Pressed(); }

void MainContext::master_EStop_Pressed() { state->master_EStop_Pressed(); }

void MainContext::master_EStop_Released() { state->master_EStop_Released(); }

void MainContext::slave_EStop_Pressed() { state->slave_EStop_Pressed(); }

void MainContext::slave_EStop_Released() { state->slave_EStop_Released(); }

void MainContext::selfSolvableErrorOccurred() {
    Logger::error("Error occurred (self-solvable)");
    state->selfSolvableErrorOccurred();
}

void MainContext::errorSelfSolved() {
    Logger::info("Error was solved");
    state->errorSelfSolved();
}

void MainContext::nonSelfSolvableErrorOccurred() {
    Logger::error("Error occurred (not self-solvable)");
    state->nonSelfSolvableErrorOccurred();
}
