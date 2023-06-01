/*
 * Actuators.cpp
 *
 *  Created on: 26.05.2023
 *      Author: Maik
 */

#include "Actuators.h"
#include "logger/logger.hpp"
#ifdef SIMULATION
#include "../simulation/simulationadapterqnx/simqnxgpioapi.h"
#include "../simulation/simulationadapterqnx/simqnxirqapi.h"
#endif

Actuators::Actuators(std::shared_ptr<EventManager> mngr) : eventManager(mngr) {
	gpio_bank_1 = mmap_device_io(GPIO_SIZE, (uint64_t) GPIO_BANK_1);
	gpio_bank_2 = mmap_device_io(GPIO_SIZE, (uint64_t) GPIO_BANK_2);

	// Default: Stop Motor
	motorStop(true);
	motorSlow(false);
	motorFast(false);

	subscribeToEvents();
}

Actuators::~Actuators() {
	munmap_device_io(gpio_bank_1, GPIO_SIZE);
	munmap_device_io(gpio_bank_2, GPIO_SIZE);
}

void Actuators::configurePins() {
	ThreadCtl(_NTO_TCTL_IO, 0);

	// Request interrupt and IO abilities.
	int procmgr_status = procmgr_ability(0,
			PROCMGR_ADN_ROOT | PROCMGR_AOP_ALLOW | PROCMGR_AID_INTERRUPT,
			PROCMGR_ADN_NONROOT | PROCMGR_AOP_ALLOW | PROCMGR_AID_INTERRUPT,
			PROCMGR_ADN_ROOT | PROCMGR_AOP_ALLOW | PROCMGR_AID_IO,
			PROCMGR_ADN_NONROOT | PROCMGR_AOP_ALLOW | PROCMGR_AID_IO,
			PROCMGR_AID_EOL);
	if (procmgr_status != EOK) {
		perror("Requested abilities failed or denied!");
		exit(EXIT_FAILURE);
	}

	uint32_t temp, outputs;

	// Configure GPIOs as outputs
	// Port 1
	outputs = MOTOR_LEFT_PIN | MOTOR_RIGHT_PIN | MOTOR_SLOW_PIN | MOTOR_STOP_PIN | LAMP_RED_PIN | LAMP_YELLOW_PIN | LAMP_GREEN_PIN | SWITCH_PIN;
	temp = in32(GPIO_OE_REGISTER(gpio_bank_1));
	out32(GPIO_OE_REGISTER(gpio_bank_1), temp & ~outputs);

	// Port 2
	outputs = LED_Q1_PIN | LED_Q2_PIN | LED_RESET_PIN | LED_START_PIN;
	temp = in32(GPIO_OE_REGISTER(gpio_bank_2));
	out32(GPIO_OE_REGISTER(gpio_bank_2), temp & ~outputs);
}

void Actuators::handleEvent(Event event) {
	Logger::debug("Actuators handle Event: " + EVENT_TO_STRING(event.type) + " - data: " + std::to_string(event.data));
	switch(event.type) {
	case EventType::HALmotorStop:
		motorStop((bool) event.data); break;
	case EventType::HALmotorFastRight:
		motorFast((bool) event.data); break;
	case EventType::HALmotorSlowRight:
		motorSlow((bool) event.data); break;
	case EventType::MODE_STANDBY:
		standbyMode(); break;
	case EventType::MODE_RUNNING:
		runningMode(); break;
	case EventType::MODE_SERVICE:
		serviceMode(); break;
	case EventType::MODE_ESTOP:
		estopMode(); break;
	case EventType::MODE_ERROR:
		errorMode(); break;
	default:
		Logger::warn(EVENT_TO_STRING(event.type) + " was not handled by actuators");
	}
}

void Actuators::subscribeToEvents() {
	// Subscribe to motor events
	eventManager->subscribe(EventType::HALmotorStop, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));
	eventManager->subscribe(EventType::HALmotorFastRight, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));
	eventManager->subscribe(EventType::HALmotorSlowRight, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));

	// Subscribe to lamp events
	eventManager->subscribe(EventType::HALroteLampeAn, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));
	eventManager->subscribe(EventType::HALroteLampeAus, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));
	eventManager->subscribe(EventType::HALgelbeLampeAn, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));
	eventManager->subscribe(EventType::HALgelbeLampeAus, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));
	eventManager->subscribe(EventType::HALgrueneLampeAn, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));
	eventManager->subscribe(EventType::HALgrueneLampeAus, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));

	// Subscribe to modes
	eventManager->subscribe(EventType::MODE_STANDBY, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));
	eventManager->subscribe(EventType::MODE_RUNNING, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));
	eventManager->subscribe(EventType::MODE_SERVICE, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));
	eventManager->subscribe(EventType::MODE_ESTOP, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));
	eventManager->subscribe(EventType::MODE_ERROR, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));

}

void Actuators::standbyMode() {
	greenLampOff();
	yellowLampOff();
	redLampOff();
	motorStop(true);
}

void Actuators::runningMode() {
	greenLampOn();
	yellowLampOff();
	redLampOff();
}

void Actuators::serviceMode() {
	greenLampBlinking();
	yellowLampOff();
	redLampOff();
	motorStop(true);
}

void Actuators::errorMode() {
	greenLampOff();
	yellowLampOff();
	redLampBlinkFast();
	motorStop(true);
}

void Actuators::estopMode() {
	greenLampOff();
	yellowLampOff();
	redLampOff();
	motorStop(true);
}

void Actuators::greenLampOn() {
	out32(GPIO_SETDATAOUT(gpio_bank_1), LAMP_GREEN_PIN);
}

void Actuators::greenLampBlinking() {
	// TODO: Let green lamp blink for ServiceMode
}

void Actuators::greenLampOff() {
	out32(GPIO_CLEARDATAOUT(gpio_bank_1), LAMP_GREEN_PIN);
}

void Actuators::yellowLampOn() {
	out32(GPIO_SETDATAOUT(gpio_bank_1), LAMP_YELLOW_PIN);
}

void Actuators::yellowLampOff() {
	out32(GPIO_CLEARDATAOUT(gpio_bank_1), LAMP_YELLOW_PIN);
}

void Actuators::redLampOn() {
	out32(GPIO_SETDATAOUT(gpio_bank_1), LAMP_RED_PIN);
}

void Actuators::redLampBlinkFast() {
	// TODO: Let red lamp blink with 1Hz
}

void Actuators::redLampBlinkSlow() {
	// TODO: Let red lamp blink with 0,5Hz
}

void Actuators::redLampOff() {
	out32(GPIO_CLEARDATAOUT(gpio_bank_1), LAMP_RED_PIN);
}

void Actuators::startLedOn() {
	out32(GPIO_SETDATAOUT(gpio_bank_2), LED_START_PIN);
}

void Actuators::startLedOff() {
	out32(GPIO_CLEARDATAOUT(gpio_bank_2), LED_START_PIN);
}

void Actuators::resetLedOn() {
	out32(GPIO_SETDATAOUT(gpio_bank_2), LED_RESET_PIN);
}

void Actuators::resetLedOff() {
	out32(GPIO_CLEARDATAOUT(gpio_bank_2), LED_RESET_PIN);
}

void Actuators::q1LedOn() {
	out32(GPIO_SETDATAOUT(gpio_bank_2), LED_Q1_PIN);
}

void Actuators::q1LedOff() {
	out32(GPIO_CLEARDATAOUT(gpio_bank_2), LED_Q1_PIN);
}

void Actuators::q2LedOn() {
	out32(GPIO_SETDATAOUT(gpio_bank_2), LED_Q2_PIN);
}

void Actuators::q2LedOff() {
	out32(GPIO_CLEARDATAOUT(gpio_bank_2), LED_Q2_PIN);
}

void Actuators::motorSlow(bool slow) {
	if(slow)
		out32(GPIO_SETDATAOUT(gpio_bank_1), MOTOR_SLOW_PIN);
	else
		out32(GPIO_CLEARDATAOUT(gpio_bank_1), MOTOR_SLOW_PIN);
}

void Actuators::motorFast(bool fast) {
	if(fast) {
		out32(GPIO_SETDATAOUT(gpio_bank_1), MOTOR_RIGHT_PIN);
	} else {
		out32(GPIO_CLEARDATAOUT(gpio_bank_1), MOTOR_RIGHT_PIN);
	}
}

void Actuators::motorStop(bool stop) {
	if(stop)
		out32(GPIO_SETDATAOUT(gpio_bank_1), MOTOR_STOP_PIN);
	else
		out32(GPIO_CLEARDATAOUT(gpio_bank_1), MOTOR_STOP_PIN);
}

void Actuators::motorRight() {
	out32(GPIO_CLEARDATAOUT(gpio_bank_1), MOTOR_STOP_PIN);
	out32(GPIO_CLEARDATAOUT(gpio_bank_1), MOTOR_LEFT_PIN);
	out32(GPIO_SETDATAOUT(gpio_bank_1), MOTOR_RIGHT_PIN);
}

void Actuators::motorLeft() {
	out32(GPIO_CLEARDATAOUT(gpio_bank_1), MOTOR_STOP_PIN);
	out32(GPIO_SETDATAOUT(gpio_bank_1), MOTOR_LEFT_PIN);
	out32(GPIO_CLEARDATAOUT(gpio_bank_1), MOTOR_RIGHT_PIN);
}

void Actuators::openSwitch() {
	out32(GPIO_SETDATAOUT(gpio_bank_1), SWITCH_PIN);
}

void Actuators::closeSwitch() {
	out32(GPIO_CLEARDATAOUT(gpio_bank_1), SWITCH_PIN);
}
