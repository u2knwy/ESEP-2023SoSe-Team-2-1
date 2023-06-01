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


#define EVENT_TO_STRING(ev) std::string(EventString[ev])
