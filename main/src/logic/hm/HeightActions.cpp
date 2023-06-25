/*
 * HeightActions.cpp
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#include "HeightActions.h"
#include "events/IEventSender.h"

#include "logger/logger.hpp"

HeightActions::HeightActions(HeightContextData* data, IEventSender* sender, std::shared_ptr<IEventManager> mngr) {
    this->data = data;
    this->sender = sender;
    this->eventManager = mngr;
    this->isMaster = Configuration::getInstance().systemIsMaster();
    if (sender->connect(mngr)) {
        Logger::debug("[HeightActions] Connected to EventManager");
    } else {
        Logger::error("[HeightActions] Error while connecting to EventManager");
    }
}

HeightActions::~HeightActions() {
	sender->disconnect();
	delete sender;
}

void HeightActions::sendMotorSlowRequest(bool slow) {
    Event ev;
    ev.type =
        isMaster ? EventType::MOTOR_M_SLOW_REQ : EventType::MOTOR_S_SLOW_REQ;
    if (slow) {
        Logger::debug("[HFSM] Send motor slow request");
        ev.data = 1;
    } else {
        Logger::debug("[HFSM] Reset motor slow request");
        ev.data = 0;
    }
    sender->sendEvent(ev);
}

void HeightActions::sendHeightResult() {
    HeightResult result = data->getCurrentResult();
    Event event;
    switch (result.type) {
    case WS_F:
        event.type = isMaster ? EventType::HM_M_WS_F : EventType::HM_S_WS_F;
        break;
    case WS_OB:
        event.type = isMaster ? EventType::HM_M_WS_OB : EventType::HM_S_WS_OB;
        break;
    case WS_BOM:
        event.type = isMaster ? EventType::HM_M_WS_BOM : EventType::HM_S_WS_BOM;
        break;
    default:
        event.type =
            isMaster ? EventType::HM_M_WS_UNKNOWN : EventType::HM_S_WS_UNKNOWN;
    }

    std::stringstream ss;
    ss.precision(1);

    if (isMaster) {
    	// master: send average value (1 decimal place)
        event.data = (int) (result.average * 10);
        ss << "[HM] Height Result at FBM1: " << EVENT_TO_STRING(event.type);
        ss << " - average(mm): " << result.average;
    } else {
    	// slave: send average and max value (1 decimal place)
        event.data = (int) (result.average * 10);
        event.additional_data = (int) (result.max * 10);
        ss << "[HM] Height Result at FBM2: " << EVENT_TO_STRING(event.type);
        ss << " - average(mm): " << result.average << " - max(mm): " << result.max;
    }
    Logger::debug(ss.str());

    sender->sendEvent(event);
}
