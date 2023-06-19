/*
 * macros.h
 *
 *  Created on: 15.05.2023
 *      Author: Maik
 */
#pragma once

#include "data/workpiecetype_stringlist.h"
#include <chrono>
#include <thread>


#define EVENT_TO_STRING(ev)   std::string(EventString[ev])
#define WP_TYPE_TO_STRING(wp) std::string(WorkpieceTypeString[wp])

/* Helper macros */
#define DELAY_S(x) std::this_thread::sleep_for(std::chrono::seconds(x))

#define BIT_MASK(x) (1 << (x))
// Check if bit(s) of a register are set
#define BIT_SET(mask, reg)    ((mask & reg) == mask)
#define BIT_NOTSET(mask, reg) ((mask & reg) != mask)
