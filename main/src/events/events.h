/*
 * events.h
 *
 *  Created on: 04.05.2023
 *      Author: Maik
 */

#pragma once

#include "IEventHandler.h"
#include "eventtypes_enum.h"
#include "eventtypes_stringlist.h"

// ENum value to attach to event data for controlling lamps
enum LampState {
	OFF,
	ON,
	FLASHING_SLOW,
	FLASHING_FAST
};

#define EVENT_TO_STRING(ev) std::string(EventString[ev])
