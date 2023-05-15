/*
 * macros.h
 *
 *  Created on: 15.05.2023
 *      Author: Maik
 */
#pragma once

#include <thread>
#include <chrono>

#define DELAY_S(x) std::this_thread::sleep_for(std::chrono::seconds(x))
