/*
 * Actuators.cpp
 *
 *  Created on: 26.05.2023
 *      Author: Maik
 */

#include "Actuators.h"
#include "logger/logger.hpp"

Actuators::Actuators(std::shared_ptr<EventManager> mngr) : eventManager(mngr) {
	gpio_bank_1 = mmap_device_io(GPIO_SIZE, (uint64_t) GPIO_BANK_1);
	gpio_bank_2 = mmap_device_io(GPIO_SIZE, (uint64_t) GPIO_BANK_2);

	// Default: Stop Motor
	motorStop();

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

void Actuators::subscribeToEvents() {
	// Subscribe to modes
	eventManager->subscribe(EventType::MODE_STANDBY, std::bind(&Actuators::standbyMode, this));
	eventManager->subscribe(EventType::MODE_RUNNING, std::bind(&Actuators::runningMode, this));
	eventManager->subscribe(EventType::MODE_SERVICE, std::bind(&Actuators::serviceMode, this));
	eventManager->subscribe(EventType::MODE_ESTOP, std::bind(&Actuators::estopMode, this));
	eventManager->subscribe(EventType::MODE_ERROR, std::bind(&Actuators::errorMode, this));

	// Subscribe to lamp events
	eventManager->subscribe(EventType::HALroteLampeAn, std::bind(&Actuators::redLampOn, this));
	eventManager->subscribe(EventType::HALroteLampeAus, std::bind(&Actuators::redLampOff, this));
	eventManager->subscribe(EventType::HALgelbeLampeAn, std::bind(&Actuators::yellowLampOn, this));
	eventManager->subscribe(EventType::HALgelbeLampeAus, std::bind(&Actuators::yellowLampOff, this));
	eventManager->subscribe(EventType::HALgrueneLampeAn, std::bind(&Actuators::greenLampOn, this));
	eventManager->subscribe(EventType::HALgrueneLampeAus, std::bind(&Actuators::greenLampOff, this));

	// Subscribe to motor events
	eventManager->subscribe(EventType::HALmotorFastRight, std::bind(&Actuators::motorFast, this));
	eventManager->subscribe(EventType::HALmotorSlowRight, std::bind(&Actuators::motorSlow, this));
	eventManager->subscribe(EventType::HALmotorStop, std::bind(&Actuators::motorStop, this));
}

void Actuators::handleEvent(EventType eventType) {
	Logger::debug("Actuators handle Event: " + EVENT_TO_STRING(eventType));
}

void Actuators::standbyMode() {
	greenLampOff();
	yellowLampOff();
	redLampOff();
	motorStop();
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
	motorStop();
}

void Actuators::errorMode() {
	greenLampOff();
	yellowLampOff();
	redLampBlinkFast();
	motorStop();
}

void Actuators::estopMode() {
	greenLampOff();
	yellowLampOff();
	redLampOff();
	motorStop();
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

void Actuators::motorSlow() {
	out32(GPIO_SETDATAOUT(gpio_bank_1), MOTOR_SLOW_PIN);
}

void Actuators::motorFast() {
	out32(GPIO_CLEARDATAOUT(gpio_bank_1), MOTOR_SLOW_PIN);
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

void Actuators::motorStop() {
	out32(GPIO_SETDATAOUT(gpio_bank_1), MOTOR_STOP_PIN);
	out32(GPIO_CLEARDATAOUT(gpio_bank_1), MOTOR_LEFT_PIN);
	out32(GPIO_CLEARDATAOUT(gpio_bank_1), MOTOR_RIGHT_PIN);
}

void Actuators::openSwitch() {
	out32(GPIO_SETDATAOUT(gpio_bank_1), SWITCH_PIN);
}

void Actuators::closeSwitch() {
	out32(GPIO_CLEARDATAOUT(gpio_bank_1), SWITCH_PIN);
}
