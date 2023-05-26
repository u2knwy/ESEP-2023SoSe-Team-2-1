#include <iostream>
#include <thread>
#include <csignal>

#include "demo/demos.h"
#include "logger/logger.hpp"
#include "common/macros.h"

#include "hal/hal.h"
#include "hal/HeightSensor.h"
#include "configuration/options.hpp"
#include "logic/hm/HeightSensorFSM.h"
#include "events/events.h"
#include "events/EventManager.h"
#include "configuration/Configuration.h"

#include <gtest/gtest.h>

using namespace std;

// Components which will be launched in main-function and cleaned up if program is terminated
std::shared_ptr<HeightSensorFSM> heightFSM;
std::shared_ptr<HeightSensor> heightSensor;
std::shared_ptr<HAL> hal;
std::shared_ptr<EventManager> eventManager;

// Set this variable to false to stop main function from executing...
bool running = true;

/**
 * Signal Handler which must be called if the program is terminated.
 * Does all necessary stuff for cleanup to avoid memory leaks.
 */
void cleanup(int exitCode)
{
	Logger::info("Exit code received: " + std::to_string(exitCode));
	heightSensor->stop();
	running = false;
}

int main(int argc, char **argv)
{
	Options options{argc, argv};
	if (options.mode == Mode::TESTS)
	{
		// Run Unit Tests
		Logger::info("Running tests...");
		::testing::InitGoogleTest(&argc, argv);
		auto result = RUN_ALL_TESTS();
		return result;
	}

	options.mode == Mode::MASTER ? Logger::info("Program started as MASTER") : Logger::info("Program started as SLAVE");
	options.pusher ? Logger::info("Hardware uses Pusher for sorting out") : Logger::info("Hardware uses Switch for sorting out");

	Configuration &conf = Configuration::getInstance();
	conf.readConfigFromFile("/usr/tmp/conf.txt");
	conf.setMaster(options.mode == Mode::MASTER);
	conf.setPusherMounted(options.pusher);

	// Initialize Logger
	const string debug = string(getenv("QNX_DEBUG"));
	if (debug == "TRUE")
	{
		Logger::set_level(Logger::level::DEBUG);
		Logger::debug("##### Started in DEBUG mode #####");
	}
	else
	{
		Logger::set_level(Logger::level::INFO);
	}

	if (options.mode == Mode::DEMO)
	{
		Logger::info("Starting in Demo Mode...");
		DELAY_S(1);
		// Run Demo programs...
		// actuatorDemo();
		// sensorDemo();
		// adcDemo();
		fsmDemo();
		return EXIT_SUCCESS;
	}

	eventManager = std::make_shared<EventManager>();
	hal = std::make_shared<HAL>(eventManager);
	hal->startEventLoop();

	heightFSM = std::make_shared<HeightSensorFSM>();
	heightSensor = std::make_shared<HeightSensor>(heightFSM);
	heightSensor->start();

	// Register handler function to be called if the program is not terminated properly
	std::signal(SIGINT, cleanup);
	std::signal(SIGABRT, cleanup);
	std::signal(SIGTERM, cleanup);

	// Endless loop - wait until termination
	while (running)
	{
		// Sleep to save CPU resources
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	Logger::info("Sorting Machine was terminated.");
	return EXIT_SUCCESS;
}
