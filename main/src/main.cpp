#include <iostream>
#include <thread>
#include <csignal>

#include "logger/logger.hpp"
#include "common/macros.h"

#include "configuration/options.hpp"
#include "hal/HeightSensor.h"
#include "hal/Actuators.h"
#include "logic/main_fsm/MainContext.h"
#include "logic/motor_fsm/MotorContext.h"
#include "events/events.h"
#include "events/EventManager.h"
#include "configuration/Configuration.h"

#include <gtest/gtest.h>
#include <hal/Sensors.h>
#include <logic/hm/HeightContext.h>

#ifdef SIMULATION
#include "simulation/simulationadapterqnx/simqnxgpioapi.h" // must be last include !!!
#endif

using namespace std;

// Components which will be launched in main-function and cleaned up if program is terminated
std::shared_ptr<HeightContext> heightFSM;
//std::shared_ptr<HeightSensor> heightSensor;
std::shared_ptr<Sensors> sensors;
std::shared_ptr<Actuators> actuators;
std::shared_ptr<EventManager> eventManager;
std::shared_ptr<MainContext> mainFSM;
std::shared_ptr<MotorContext> motorFSM_Master;
std::shared_ptr<MotorContext> motorFSM_Slave;

// Set this variable to false to stop main function from executing...
bool running = true;

/**
 * Signal Handler which must be called if the program is terminated.
 * Does all necessary stuff for cleanup to avoid memory leaks.
 */
void cleanup(int exitCode)
{
	Logger::info("Exit code received: " + std::to_string(exitCode));
//	heightSensor->stop();
	running = false;
}

int main(int argc, char **argv)
{
	// Initialize Logger
	const char* debugValue = getenv("QNX_DEBUG");
	const std::string debug = debugValue ? debugValue : "";
	if (debug == "TRUE") {
		Logger::set_level(Logger::level::DEBUG);
		Logger::debug("##### Started in DEBUG mode #####");
	} else {
		Logger::set_level(Logger::level::INFO);
	}

	Options options{argc, argv};
	if (options.mode == Mode::TESTS) {
		// Run Unit Tests
		Logger::info("Running tests...");
		::testing::InitGoogleTest(&argc, argv);
		auto result = RUN_ALL_TESTS();
		return result;
	}

	options.mode == Mode::MASTER ? Logger::info("Program started as MASTER") : Logger::info("Program started as SLAVE");
	options.pusher ? Logger::info("Hardware uses Pusher for sorting out") : Logger::info("Hardware uses Switch for sorting out");

	Configuration &conf = Configuration::getInstance();
	conf.setConfigFilePath("/usr/tmp/conf.txt");
	if(!conf.readConfigFromFile()) {
		return EXIT_FAILURE;
	}
	conf.setMaster(options.mode == Mode::MASTER);
	conf.setPusherMounted(options.pusher);

	eventManager = std::make_shared<EventManager>();
	sensors = std::make_shared<Sensors>(eventManager);
	actuators = std::make_shared<Actuators>(eventManager);
	actuators->setMotorStop(true);

	// Run FSM's
	mainFSM = std::make_shared<MainContext>(eventManager);
	motorFSM_Master = std::make_shared<MotorContext>(eventManager, true);
	motorFSM_Slave = std::make_shared<MotorContext>(eventManager, false);

	// Register handler function to be called if the program is not terminated properly
	std::signal(SIGINT, cleanup);
	std::signal(SIGABRT, cleanup);
	std::signal(SIGTERM, cleanup);


	std::shared_ptr<IHeightSensor> heightSensor = std::make_shared<HeightSensor>();

	// ###########################################
	// TEMPORARY: Calibrate HeightSensor
/*	heightSensor->start();
	std::string line;
	Logger::info("Press ENTER to calibrate Conveyor");
	bool ok = false;
	int offset, refHigh;
	while(!ok) {
		std::getline(std::cin, line);
		offset = heightSensor->getLastRawValue();
		Logger::info("Value: " + std::to_string(offset) + " - OK? [y/N]");
		ok = line == "y";
	}
	Logger::info("Press ENTER to calibrate HIGH");
	ok = false;
	while(!ok) {
		std::getline(std::cin, line);
		refHigh = heightSensor->getLastRawValue();
		Logger::info("Value: " + std::to_string(refHigh) + " - OK? [y/N]");
		ok = line == "y";
	}
	heightSensor->stop();
	conf.setOffsetCalibration(offset);
	conf.setReferenceCalibration(refHigh);*/
	//conf.saveCurrentConfigToFile();
	// Calibrate HeightSensor END
	// ###########################################
	conf.setOffsetCalibration(3644);
	conf.setReferenceCalibration(2302);

	sensors->startEventLoop();
	heightFSM = std::make_shared<HeightContext>(eventManager, heightSensor);

	// Endless loop - wait until termination
	while (running) {
		// Sleep to save CPU resources
		std::this_thread::sleep_for(std::chrono::seconds(1));

	}

	Logger::info("Sorting Machine was terminated.");
	return EXIT_SUCCESS;
}
