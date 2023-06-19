/*
 * HeightActions.cpp
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#include "HeightActions.h"

#include "logger/logger.hpp"

HeightActions::HeightActions(HeightContextData* data,
                             std::shared_ptr<EventManager> mngr) {
  this->data = data;
  this->eventManager = mngr;
  this->isMaster = Configuration::getInstance().systemIsMaster();
  if (connect(mngr)) {
    Logger::debug("[HeightActions] Connected to EventManager");
  } else {
    Logger::error("[HeightActions] Error while connecting to EventManager");
  }
}

HeightActions::~HeightActions() { disconnect(); }

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
  sendEvent(ev);
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

  if (isMaster) {
    event.data = (int)(result.average *
                       10);  // master: send average value (1 decimal place)
    Logger::debug("[HM] Height Result at FBM1: " + EVENT_TO_STRING(event.type) +
                  ", average (mm): " + std::to_string(result.average));
  } else {
    event.data =
        (int)(result.max * 10);  // slave: send max value (1 decimal place)
    Logger::debug("[HM] Height Result at FBM2: " + EVENT_TO_STRING(event.type) +
                  ", max (mm): " + std::to_string(result.max));
  }

  sendEvent(event);
}
