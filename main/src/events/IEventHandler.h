/*
 * IEventHandler.h
 *
 *  Created on: 04.05.2023
 *      Author: Maik
 */

#pragma once

#include "eventtypes_enum.h"
#include "events.h"

typedef struct Event {
    EventType type;
    int data{-1};
};

class IEventHandler {
public:
	virtual ~IEventHandler() {};
	virtual void handleEvent(Event event) = 0;
};
