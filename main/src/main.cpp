#include <iostream>
#include <thread>
#include "hal/hal.h"

#include "demo/demos.h"
#include "logger/logger.hpp"
#include "events/events.h"
#include "configuration/options.hpp"

#ifdef RUN_TESTS
#include <gtest/gtest.h>
#endif

using namespace std;

int main(int argc, char **argv) {
#ifdef RUN_TESTS
	// Run Unit Tests
	::testing::InitGoogleTest(&argc, argv);
	auto result = RUN_ALL_TESTS();
#endif

    Options options{argc, argv};
    options.master ? Logger::info("Program started as MASTER") : Logger::info("Program started as SLAVE");
    options.pusher ? Logger::info("Hardware uses Pusher for sorting out") : Logger::info("Hardware uses Switch for sorting out");

	// Initialize Logger
	const string debug = string(getenv("QNX_DEBUG"));
	if(debug == "TRUE") {
		Logger::set_level(Logger::level::DEBUG);
		Logger::debug("##### Started in DEBUG mode #####");
	} else {
		Logger::set_level(Logger::level::INFO);
	}

	const string demo = string(getenv("DEMO_MODE"));
	if(demo == "TRUE") {
		//actuatorDemo();
		sensorDemo();
		//adcDemo();
	}

	Logger::info("Sorting Machine was terminated.");
	return 0;
}
