/*
 * SubEStopTwoPressed.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "SubEStopTwoPressed.h"
#include "SubEStopOnePressed.h"
#include "logger/logger.hpp"

#include <iostream>

void SubEStopTwoPressed::entry() {
	Logger::debug("[EStop] Two pressed");
}

bool SubEStopTwoPressed::master_EStop_Released() {
	exit();
	new(this) SubEStopOnePressed;
	entry();
	return true;
}

bool SubEStopTwoPressed::slave_EStop_Released() {
	exit();
	new(this) SubEStopOnePressed;
	entry();
	return true;
}
