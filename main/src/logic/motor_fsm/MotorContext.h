/*
 * MotorContext.h
 *
 *  Created on: 09.06.2023
 *      Author: Maik
 */
#pragma once

#include "MotorActions.h"
#include "MotorBasestate.h"
#include "MotorContextData.h"
#include "events/EventManager.h"
#include "events/IEventHandler.h"
#include "events/events.h"

#include <memory>

class MotorContext : public IEventHandler {
  public:
    MotorContext(std::shared_ptr<EventManager> mngr, bool master);
    virtual ~MotorContext();

    void handleEvent(Event event) override;
    MotorState getCurrentState();

  private:
    MotorActions *actions;
    MotorContextData *data;
    MotorBasestate *state;
    std::shared_ptr<EventManager> eventManager;
    void subscribeToEvents();
    bool isMaster;
};
