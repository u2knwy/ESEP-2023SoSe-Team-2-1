/*
 * IEventHandler.h
 *
 *  Created on: 04.05.2023
 *      Author: Maik
 */

#pragma once

#include "eventtypes_enum.h"
#include "events.h"

struct Event {
	Event() {}
	Event(EventType evType) : type(evType), data(-1) {}
	Event(EventType evType, int evData) : type(evType), data(evData) {}
    EventType type;
    int data{-1};
};

class IEventHandler {
public:
	virtual ~IEventHandler() {};
	virtual void handleEvent(Event event) = 0;
};
