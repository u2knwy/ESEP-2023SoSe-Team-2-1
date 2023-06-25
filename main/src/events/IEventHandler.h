/*
 * IEventHandler.h
 *
 *  Created on: 04.05.2023
 *      Author: Maik
 */

#pragma once

#include "events.h"
#include "eventtypes_enum.h"


// ENum value to attach to event data for controlling lamps
enum LampState { OFF, ON, FLASHING_SLOW, FLASHING_FAST };

struct Event {
    Event() {}
    Event(EventType evType) : type(evType), data(-1) {}
    Event(EventType evType, int evData) : type(evType), data(evData) {}
    Event(EventType evType, int evData, int addData) : type(evType), data(evData), additional_data(addData) {}
    EventType type;
    int data{-1};
    int additional_data{-1};
};

class IEventHandler {
  public:
    virtual ~IEventHandler(){};
    virtual void handleEvent(Event event) = 0;
};
