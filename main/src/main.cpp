#include <iostream>
#include <thread>
#include "hal/hal.h"

#include "demo/demos.h"
#include "logger/logger.hpp"
#include "events/events.h"
#include "configuration/options.hpp"
#include "configuration/Configuration.h"

#include <gtest/gtest.h>

using namespace std;

int main(int argc, char **argv) {
    Options options{argc, argv};
    if (options.mode == Mode::TESTS) {
    	// Run Unit Tests
    	Logger::info("Running tests...");
    	::testing::InitGoogleTest(&argc, argv);
    	auto result = RUN_ALL_TESTS();
    	return result;
    } else if (options.mode == Mode::DEMO) {
    	Logger::info("Starting in Demo Mode...");
    	// Run Demo programs...
		actuatorDemo();
		sensorDemo();
		adcDemo();
    	return EXIT_SUCCESS;
    }

    options.mode == Mode::MASTER ? Logger::info("Program started as MASTER") : Logger::info("Program started as SLAVE");
    options.pusher ? Logger::info("Hardware uses Pusher for sorting out") : Logger::info("Hardware uses Switch for sorting out");

	// Initialize Logger
	const string debug = string(getenv("QNX_DEBUG"));
	if(debug == "TRUE") {
		Logger::set_level(Logger::level::DEBUG);
		Logger::debug("##### Started in DEBUG mode #####");
	} else {
		Logger::set_level(Logger::level::INFO);
	}

	Configuration conf;
	conf.readConfigFromFile("/usr/tmp/conf.txt");

	Logger::info("Sorting Machine was terminated.");
	return EXIT_SUCCESS;
}
