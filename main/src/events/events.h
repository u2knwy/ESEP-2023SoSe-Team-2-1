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

// My pulse codes
#define PULSE_STOP_THREAD 		_PULSE_CODE_MINAVAIL + 1
#define PULSE_ADC_SAMPLING_DONE _PULSE_CODE_MINAVAIL + 2
#define PULSE_INTR_ON_PORT0 	_PULSE_CODE_MINAVAIL + 3
