/*
 * macros.h
 *
 *  Created on: 15.05.2023
 *      Author: Maik
 */
#pragma once

#include <thread>
#include <chrono>

/* Helper macros */
#define DELAY_S(x) std::this_thread::sleep_for(std::chrono::seconds(x))

#define BIT_MASK(x) (1 << (x))
// Check if bit(s) of a register are set
#define BIT_SET(mask, reg) ((mask & reg) == mask)
#define BIT_NOTSET(mask, reg) ((mask & reg) != mask)
