#include <gtest/gtest.h>
#include <hal/Sensors.h>
#include <logic/hm/HeightContext.h>
#include <sys/neutrino.h>

#include <atomic>
#include <csignal>
#include <iostream>
#include <thread>

#include "common/macros.h"
#include "configuration/Configuration.h"
#include "configuration/options.hpp"
#include "events/EventManager.h"
#include "events/events.h"
#include "events/EventSender.h"
#include "hal/Actuators.h"
#include "hal/HeightSensor.h"
#include "logger/logger.hpp"
#include "logic/main_fsm/MainContext.h"
#include "logic/motor_fsm/MotorContext.h"
#include "watchdog/Watchdog.h"
#ifdef SIM_ACTIVE
#include "simqnxgpioapi.h"   // must be last include !!!
#include "simqnxirqapi.h"
#endif

using namespace std;

// Components which will be launched in main-function and cleaned up if program
// is terminated
std::shared_ptr<HeightContext> heightFSM;
std::shared_ptr<IHeightSensor> heightSensor;
std::shared_ptr<Sensors> sensors;
std::shared_ptr<Actuators> actuators;
std::shared_ptr<EventManager> eventManager;
std::shared_ptr<MainContext> mainFSM;
std::shared_ptr<MotorContext> motorFSM_Master;
std::shared_ptr<MotorContext> motorFSM_Slave;

// Set this variable to false to stop main function from executing...
std::atomic<bool> running(true);

/**
 * Signal Handler which must be called if the program is terminated.
 * Does all necessary stuff for cleanup to avoid memory leaks.
 */
void cleanup(int exitCode) {
	if(exitCode == EXIT_SUCCESS) {
		return;
	}
    Logger::info("Exit code received: " + std::to_string(exitCode));
    //	heightSensor->stop();
    //eventManager->stop();
    bool master = Configuration::getInstance().systemIsMaster();
    if(master) {
		eventManager->sendExternalEvent(Event{ERROR_M_MAN_SOLVABLE});
    } else {
    	eventManager->sendExternalEvent(Event{ERROR_S_MAN_SOLVABLE});
    }
    actuators->allOff();
    running = false;
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {

    // Initialize Logger
    const char *debugValue = getenv("QNX_DEBUG");
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

    Configuration &conf = Configuration::getInstance();
    conf.setMaster(options.mode == MASTER);
    if (conf.systemIsMaster()) {
        Logger::info("Program started as MASTER");
    } else {
        Logger::info("Program started as SLAVE");
    }

    conf.setConfigFilePath("/tmp/esep_2.1/conf.txt");
    if (!conf.readConfigFromFile()) {
        Logger::error("Error reading config file - terminating...");
        return EXIT_FAILURE;
    }

    if (options.pusher) {
        Logger::info("Configured hardware: Use 'Pusher' for sorting out");
        conf.setPusherMounted(true);
    } else {
        Logger::info("Configured hardware: Use 'Switch' for sorting out");
        conf.setPusherMounted(false);
    }

    //starting GNS
    int gnsExitCode;
    system("slay gns");
    if(options.mode == MASTER) {
        gnsExitCode = system("gns -c");
    } else {
        gnsExitCode = system("gns -s");
    }
    Logger::info("Started GNS -> exited with " + to_string(gnsExitCode));

    eventManager = std::make_shared<EventManager>();
    // Create components running on Master AND Slave
    actuators = std::make_shared<Actuators>(eventManager);

    // Register handler function to be called if the program is not
    // terminated properly
    std::signal(SIGINT, cleanup);
    std::signal(SIGQUIT, cleanup);
    std::signal(SIGTERM, cleanup);
    std::signal(SIGSEGV, cleanup);

    actuators->standbyMode();

    //Logger::registerEvents(eventManager);
    eventManager->start();
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Start Watchdog -> send and receive heartbeats via EventManager
    Watchdog wd(eventManager);
    wd.start();

    // Run FSM's only at Master
    if (options.mode == Mode::MASTER) {
    	MotorActions* actionsM = new MotorActions(eventManager, new EventSender(), true);
        motorFSM_Master = std::make_shared<MotorContext>(actionsM, true);
    	MotorActions* actionsS = new MotorActions(eventManager, new EventSender(), false);
        motorFSM_Slave = std::make_shared<MotorContext>(actionsS, false);
        MainActions* mainActions = new MainActions(eventManager, new EventSender());
        mainFSM = std::make_shared<MainContext>(mainActions);
    } else {
        Logger::info("Program started as SLAVE");
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));

    sensors = std::make_shared<Sensors>(eventManager);
    sensors->startEventLoop();

    heightSensor = std::make_shared<HeightSensor>(eventManager);
    HeightContextData* heightData = new HeightContextData();
    HeightActions* heightActions = new HeightActions(heightData, new EventSender(), eventManager);
    heightFSM = std::make_shared<HeightContext>(heightActions, heightData, heightSensor);

    // do nothing until termination...
    //std::cin.get();
    while (running) {
        // Sleep to save CPU resources
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    cleanup(EXIT_SUCCESS);

    Logger::info("Sorting Machine was terminated.");
    return EXIT_SUCCESS;
}
