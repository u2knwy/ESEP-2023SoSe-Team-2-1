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

bool SubEStopTwoPressed::master_EStop_Released() {
	Logger::debug("SubEStopTwoPressed::master_EStop_Released");
	exit();
	new(this) SubEStopOnePressed;
	entry();
	return true;
}

bool SubEStopTwoPressed::slave_EStop_Released() {
	Logger::debug("SubEStopTwoPressed::slave_EStop_Released");
	exit();
	new(this) SubEStopOnePressed;
	entry();
	return true;
}
