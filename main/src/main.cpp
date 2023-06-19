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
#include "hal/Actuators.h"
#include "hal/HeightSensor.h"
#include "logger/logger.hpp"
#include "logic/main_fsm/MainContext.h"
#include "logic/motor_fsm/MotorContext.h"
#include "watchdog/Watchdog.h"
#ifdef SIM_ACTIVE
#include "simqnxgpioapi.h"  // must be last include !!!
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
  Logger::info("Exit code received: " + std::to_string(exitCode));
  //	heightSensor->stop();
  eventManager->stop();
  running = false;
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

  conf.setConfigFilePath("/tmp/esep_conf.txt");
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

  // Create components running on Master AND Slave

  // Create and start EventManager to receive internal Events
  eventManager = std::make_shared<EventManager>();
  Logger::registerEvents(eventManager);
  eventManager->start();

  actuators = std::make_shared<Actuators>(eventManager);
  sensors = std::make_shared<Sensors>(eventManager);
  heightSensor = std::make_shared<HeightSensor>(eventManager);
  heightFSM = std::make_shared<HeightContext>(eventManager, heightSensor);

  // Run FSM's only at Master
  if (options.mode == Mode::MASTER) {
    motorFSM_Master = std::make_shared<MotorContext>(eventManager, true);
    motorFSM_Slave = std::make_shared<MotorContext>(eventManager, false);
    mainFSM = std::make_shared<MainContext>(eventManager);

    // mainFSM->master_btnStart_PressedShort();
    // mainFSM->master_LBA_Blocked();
    // mainFSM->master_LBA_Unblocked();
  } else {
    Logger::info("Program started as SLAVE");
    conf.setMaster(false);
  }

  // Register handler function to be called if the program is not terminated
  // properly
  std::signal(SIGINT, cleanup);
  std::signal(SIGABRT, cleanup);
  std::signal(SIGTERM, cleanup);
  std::signal(SIGKILL, cleanup);

  // Start threads...
  sensors->startEventLoop();

  // Endless loop - wait until termination
  while (running) {
    // Sleep to save CPU resources
    std::this_thread::sleep_for(std::chrono::seconds(10));
  }

  Logger::info("Sorting Machine was terminated.");
  return EXIT_SUCCESS;
}
