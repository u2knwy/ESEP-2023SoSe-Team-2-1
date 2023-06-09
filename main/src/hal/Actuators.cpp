/*
 * Actuators.cpp
 *
 *  Created on: 26.05.2023
 *      Author: Maik
 */

#include "Actuators.h"
#include "logger/logger.hpp"
#include "configuration/Configuration.h"
#ifdef SIM_ACTIVE
#include "simqnxgpioapi.h" // must be last include !!!
#include "simqnxirqapi.h"
#endif

Actuators::Actuators(std::shared_ptr<EventManager> mngr) : eventManager(mngr)
{
	isMaster = Configuration::getInstance().systemIsMaster();
	gpio_bank_1 = mmap_device_io(SIZE_4KB, (uint64_t)GPIO_BANK_1);
	gpio_bank_2 = mmap_device_io(SIZE_4KB, (uint64_t)GPIO_BANK_2);

	configurePins();

	// Default: Stop Motor
	setMotorStop(true);
	setMotorSlow(false);
	setMotorRight(false);

	greenBlinking = false;
	yellowBlinking = false;
	redBlinking = false;

	standbyMode();

	subscribeToEvents();

	stopCnt = 0;
	fastCnt = 0;
	slowCnt = 0;
}

Actuators::~Actuators()
{
	munmap_device_io(gpio_bank_1, SIZE_4KB);
	munmap_device_io(gpio_bank_2, SIZE_4KB);
}

void Actuators::configurePins()
{
	ThreadCtl(_NTO_TCTL_IO, 0);

	// Request interrupt and IO abilities.
	int procmgr_status = procmgr_ability(0,
			PROCMGR_ADN_ROOT | PROCMGR_AOP_ALLOW | PROCMGR_AID_INTERRUPT,
			PROCMGR_ADN_NONROOT | PROCMGR_AOP_ALLOW | PROCMGR_AID_INTERRUPT,
			PROCMGR_ADN_ROOT | PROCMGR_AOP_ALLOW | PROCMGR_AID_IO,
			PROCMGR_ADN_NONROOT | PROCMGR_AOP_ALLOW | PROCMGR_AID_IO,
			PROCMGR_AID_EOL);
	if (procmgr_status != EOK)
	{
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

void Actuators::subscribeToEvents()
{

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

	// System-dependent events
	if (isMaster)
	{
		eventManager->subscribe(EventType::ESTOP_M_PRESSED, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));
		eventManager->subscribe(EventType::WARNING_M, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));
		eventManager->subscribe(EventType::LED_M_START, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));
		eventManager->subscribe(EventType::LED_M_RESET, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));
		eventManager->subscribe(EventType::MOTOR_M_STOP, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));
		eventManager->subscribe(EventType::MOTOR_M_FAST, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));
		eventManager->subscribe(EventType::MOTOR_M_SLOW, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));
	}
	else
	{
		eventManager->subscribe(EventType::ESTOP_S_PRESSED, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));
		eventManager->subscribe(EventType::WARNING_S, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));
		eventManager->subscribe(EventType::LED_S_START, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));
		eventManager->subscribe(EventType::LED_S_RESET, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));
		eventManager->subscribe(EventType::MOTOR_S_STOP, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));
		eventManager->subscribe(EventType::MOTOR_S_FAST, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));
		eventManager->subscribe(EventType::MOTOR_S_SLOW, std::bind(&Actuators::handleEvent, this, std::placeholders::_1));
	}
}

void Actuators::handleEvent(Event event)
{
	std::lock_guard<std::mutex> lock(mutex);
	Logger::debug("Actuators handle Event: " + EVENT_TO_STRING(event.type) + " - data: " + std::to_string(event.data));
	switch (event.type)
	{
	case EventType::MOTOR_M_STOP:
	case EventType::MOTOR_S_STOP:
		motorStop();
		break;
	case EventType::MOTOR_M_FAST:
	case EventType::MOTOR_S_FAST:
		motorFast();
		break;
	case EventType::MOTOR_M_SLOW:
	case EventType::MOTOR_S_SLOW:
		motorSlow();
		break;
	case EventType::ESTOP_M_PRESSED:
	case EventType::ESTOP_S_PRESSED:
		motorStop();
		break;
	case EventType::MODE_STANDBY:
		standbyMode();
		break;
	case EventType::MODE_RUNNING:
		runningMode();
		break;
	case EventType::MODE_SERVICE:
		serviceMode();
		break;
	case EventType::MODE_ESTOP:
		estopMode();
		break;
	case EventType::MODE_ERROR:
		errorMode();
		break;
	case EventType::WARNING_M:
	case EventType::WARNING_S:
		setYellowBlinking(event.data == 1);
		break;
	case EventType::LED_M_START:
	case EventType::LED_S_START:
		event.data == 1 ? startLedOn() : startLedOff();
		break;
	case EventType::LED_M_RESET:
	case EventType::LED_S_RESET:
		event.data == 1 ? resetLedOn() : resetLedOff();
		break;
	default:
		Logger::warn(EVENT_TO_STRING(event.type) + " was not handled by actuators");
	}
}

void Actuators::setGreenBlinking(bool on)
{
	if (th_GreenBlinking.joinable())
	{
		greenBlinking = false;
		th_GreenBlinking.join();
	}

	if (on)
	{
		th_GreenBlinking = std::thread(&Actuators::thGreenLampFlashing, this, false);
	}
}

void Actuators::setYellowBlinking(bool on)
{
	if (th_YellowBlinking.joinable())
	{
		yellowBlinking = false;
		th_YellowBlinking.join();
	}

	if (on)
	{
		th_YellowBlinking = std::thread(&Actuators::thYellowLampFlashing, this, false);
	}
}

void Actuators::setRedBlinking(bool on, bool fast)
{
	if (redBlinking)
	{
		redBlinking = false;
		if (th_RedBlinking.joinable())
			th_RedBlinking.join();
	}

	if (on)
	{
		th_RedBlinking = std::thread(&Actuators::thRedLampFlashing, this, fast);
	}
}

void Actuators::standbyMode()
{
	setGreenBlinking(false);
	setYellowBlinking(false);
	setRedBlinking(false, false);
	greenLampOff();
	yellowLampOff();
	redLampOff();
	q1LedOff();
	q2LedOff();
	startLedOn();
	resetLedOff();
	setMotorStop(true);
	setMotorRight(false);
	setMotorLeft(false);
	setMotorSlow(false);
}

void Actuators::runningMode()
{
	setGreenBlinking(false);
	setYellowBlinking(false);
	setRedBlinking(false, false);
	greenLampOn();
	yellowLampOff();
	redLampOff();
	q1LedOff();
	q2LedOff();
	startLedOff();
	resetLedOff();
	setMotorStop(false);
	setMotorRight(false);
	setMotorLeft(false);
	setMotorSlow(false);
}

void Actuators::serviceMode()
{
	yellowLampOff();
	redLampOff();
	setGreenBlinking(true);
	q1LedOff();
	q2LedOff();
	startLedOff();
	resetLedOff();
	setMotorStop(false);
	setMotorRight(false);
	setMotorLeft(false);
	setMotorSlow(false);
}

void Actuators::errorMode()
{
	greenLampOff();
	yellowLampOff();
	setRedBlinking(true, true);
	setMotorStop(true);
	setMotorRight(false);
	setMotorLeft(false);
	setMotorSlow(false);
}

void Actuators::estopMode()
{
	setGreenBlinking(false);
	setYellowBlinking(false);
	setRedBlinking(false, false);
	greenLampOff();
	yellowLampOff();
	redLampOff();
	setMotorStop(true);
	setMotorRight(false);
	setMotorLeft(false);
	setMotorSlow(false);
}

void Actuators::greenLampOn()
{
	out32(GPIO_SETDATAOUT(gpio_bank_1), LAMP_GREEN_PIN);
}

void Actuators::greenLampOff()
{
	out32(GPIO_CLEARDATAOUT(gpio_bank_1), LAMP_GREEN_PIN);
}

void Actuators::yellowLampOn()
{
	out32(GPIO_SETDATAOUT(gpio_bank_1), LAMP_YELLOW_PIN);
}

void Actuators::yellowLampOff()
{
	out32(GPIO_CLEARDATAOUT(gpio_bank_1), LAMP_YELLOW_PIN);
}

void Actuators::redLampOn()
{
	out32(GPIO_SETDATAOUT(gpio_bank_1), LAMP_RED_PIN);
}

void Actuators::redLampOff()
{
	out32(GPIO_CLEARDATAOUT(gpio_bank_1), LAMP_RED_PIN);
}

void Actuators::thRedLampFlashing(bool fast)
{
	int t_ms;
	t_ms = fast ? ON_TIME_FAST_MS : ON_TIME_SLOW_MS;
	redBlinking = true;
	while (redBlinking)
	{
		redLampOn();
		std::this_thread::sleep_for(std::chrono::milliseconds(t_ms));
		redLampOff();
		std::this_thread::sleep_for(std::chrono::milliseconds(t_ms));
	}
}

void Actuators::thYellowLampFlashing(bool fast)
{
	int t_ms;
	t_ms = fast ? ON_TIME_FAST_MS : ON_TIME_SLOW_MS;
	yellowBlinking = true;
	while (yellowBlinking)
	{
		yellowLampOn();
		std::this_thread::sleep_for(std::chrono::milliseconds(t_ms));
		yellowLampOff();
		std::this_thread::sleep_for(std::chrono::milliseconds(t_ms));
	}
}

void Actuators::thGreenLampFlashing(bool fast)
{
	int t_ms;
	t_ms = fast ? ON_TIME_FAST_MS : ON_TIME_SLOW_MS;
	greenBlinking = true;
	while (greenBlinking)
	{
		greenLampOn();
		std::this_thread::sleep_for(std::chrono::milliseconds(t_ms));
		greenLampOff();
		std::this_thread::sleep_for(std::chrono::milliseconds(t_ms));
	}
}

void Actuators::startLedOn()
{
	out32(GPIO_SETDATAOUT(gpio_bank_2), LED_START_PIN);
}

void Actuators::startLedOff()
{
	out32(GPIO_CLEARDATAOUT(gpio_bank_2), LED_START_PIN);
}

void Actuators::resetLedOn()
{
	out32(GPIO_SETDATAOUT(gpio_bank_2), LED_RESET_PIN);
}

void Actuators::resetLedOff()
{
	out32(GPIO_CLEARDATAOUT(gpio_bank_2), LED_RESET_PIN);
}

void Actuators::q1LedOn()
{
	out32(GPIO_SETDATAOUT(gpio_bank_2), LED_Q1_PIN);
}

void Actuators::q1LedOff()
{
	out32(GPIO_CLEARDATAOUT(gpio_bank_2), LED_Q1_PIN);
}

void Actuators::q2LedOn()
{
	out32(GPIO_SETDATAOUT(gpio_bank_2), LED_Q2_PIN);
}

void Actuators::q2LedOff()
{
	out32(GPIO_CLEARDATAOUT(gpio_bank_2), LED_Q2_PIN);
}

void Actuators::setMotorStop(bool stop)
{
	if (stop)
	{
		out32(GPIO_SETDATAOUT(gpio_bank_1), MOTOR_STOP_PIN);
	}
	else
	{
		out32(GPIO_CLEARDATAOUT(gpio_bank_1), MOTOR_STOP_PIN);
	}
	out32(GPIO_CLEARDATAOUT(gpio_bank_1), MOTOR_RIGHT_PIN);
	out32(GPIO_CLEARDATAOUT(gpio_bank_1), MOTOR_LEFT_PIN);
}

void Actuators::setMotorSlow(bool slow)
{
	if (slow)
		out32(GPIO_SETDATAOUT(gpio_bank_1), MOTOR_SLOW_PIN);
	else
		out32(GPIO_CLEARDATAOUT(gpio_bank_1), MOTOR_SLOW_PIN);
}

void Actuators::setMotorRight(bool right)
{
	if (right)
		out32(GPIO_SETDATAOUT(gpio_bank_1), MOTOR_RIGHT_PIN);
	else
		out32(GPIO_CLEARDATAOUT(gpio_bank_1), MOTOR_RIGHT_PIN);
}

void Actuators::setMotorLeft(bool left)
{
	if (left)
		out32(GPIO_SETDATAOUT(gpio_bank_1), MOTOR_LEFT_PIN);
	else
		out32(GPIO_CLEARDATAOUT(gpio_bank_1), MOTOR_LEFT_PIN);
}

void Actuators::openSwitch()
{
	out32(GPIO_SETDATAOUT(gpio_bank_1), SWITCH_PIN);
}

void Actuators::closeSwitch()
{
	out32(GPIO_CLEARDATAOUT(gpio_bank_1), SWITCH_PIN);
}

void Actuators::motorStop()
{
	setMotorStop(true);
	setMotorSlow(false);
	setMotorRight(false);
	setMotorLeft(false);
}

void Actuators::motorSlow()
{
	setMotorStop(false);
	setMotorSlow(true);
	setMotorRight(true);
	setMotorLeft(false);
}

void Actuators::motorFast()
{
	setMotorStop(false);
	setMotorSlow(false);
	setMotorRight(true);
	setMotorLeft(false);
}
