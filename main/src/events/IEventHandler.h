/*
 * IEventHandler.h
 *
 *  Created on: 04.05.2023
 *      Author: Maik
 */

#pragma once

#include "eventtypes_enum.h"

class IEventHandler {
public:
	virtual ~IEventHandler() {};
	virtual void handleEvent(EventType eventType) = 0;
};
