#include <iostream>
#include <thread>
#include "hal/hal.h"

#include "demo/demos.h"
#include "logger/logger.hpp"
#include <events/events.h>

#include <gtest/gtest.h>

int main(int argc, char **argv) {
	// Run Unit Tests
	//::testing::InitGoogleTest(&argc, argv);
	//auto result = RUN_ALL_TESTS();

	// Initialize Logger
	const auto debug = std::string(std::getenv("QNX_DEBUG"));
	if(debug == "TRUE") {
		Logger::set_level(Logger::level::DEBUG);
		Logger::debug("##### Started in DEBUG mode #####");
	} else {
		Logger::set_level(Logger::level::INFO);
	}

	//actuatorDemo();
	//sensorDemo();
	//adcDemo();

	return 0;
}
