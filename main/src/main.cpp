#include <iostream>
#include <thread>
#include "hal/hal.h"

#include "demo/demos.h"
#include "logger/logger.hpp"

int main() {
	Logger::set_level(Logger::level::DEBUG);
	Logger::debug("main started");

	//actuatorDemo();
	//sensorDemo();
	adcDemo();

	return 0;
}
