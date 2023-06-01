/*
 * hal.cpp
 *
 *  Created on: 31.03.2023
 *      Author: Maik
 */

#include "Sensors.h"
#include <string>
#include "logger/logger.hpp"
#include "events/events.h"
#include "common/macros.h"
#include "configuration/Configuration.h"

#ifdef SIMULATION
#include "../simulation/simulationadapterqnx/simqnxgpioapi.h"
#include "../simulation/simulationadapterqnx/simqnxirqapi.h"
#endif
static std::chrono::steady_clock::time_point lastStartBtnPressTime;

Sensors::Sensors(std::shared_ptr<EventManager> mngr) : eventManager(mngr)
{
	gpio_bank_0 = mmap_device_io(GPIO_SIZE, (uint64_t)GPIO_BANK_0);

	/* ### Create channel to receive interrupt pulse messages ### */
	chanID = ChannelCreate(0);
	if (chanID < 0)
	{
		perror("Could not create a channel!\n");
	}

	// Connect to channel.
	conID = ConnectAttach(0, 0, chanID, _NTO_SIDE_CHANNEL, 0);
	if (conID < 0)
	{
		perror("Could not connect to channel!");
	}

	receivingRunning = false;

	configurePins();
	initInterrupts();
}

Sensors::~Sensors()
{
	stopEventLoop();

	int detach_status = ConnectDetach(conID);
	if (detach_status != EOK)
	{
		perror("Detaching channel failed!");
		exit(EXIT_FAILURE);
	}

	int destroy_status = ChannelDestroy(chanID);
	if (destroy_status != EOK)
	{
		perror("Destroying channel failed!");
		exit(EXIT_FAILURE);
	}

	munmap_device_io(gpio_bank_0, GPIO_SIZE);
}

void Sensors::configurePins()
{
	ThreadCtl(_NTO_TCTL_IO, 0);

	uint32_t temp, inputs;
	inputs = LB_START_PIN | LB_HEIGHT_PIN | LB_HEIGHT_OK_PIN | LB_SWITCH_PIN | SE_METAL_PIN | SE_SWITCH_PIN | LB_RAMP_PIN | LB_END_PIN;
	inputs = inputs | KEY_START_PIN | KEY_STOP_PIN | KEY_RESET_PIN | ESTOP_PIN;

	// Configure GPIOs as inputs
	temp = in32(GPIO_OE_REGISTER(gpio_bank_0));
	out32(GPIO_OE_REGISTER(gpio_bank_0), temp & inputs);

	// Set debounce value/time
	out32(GPIO_DEBOUNCINGTIME(gpio_bank_0), GPIO_DEBOUNCE_VALUE);
	// Enable debouncing
	out32(GPIO_DEBOUNCENABLE(gpio_bank_0), inputs);
}

void Sensors::initInterrupts()
{
	using namespace std;

	/* ### Setup ### */
	ThreadCtl(_NTO_TCTL_IO, 0); // Request IO privileges for process.

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

	InterruptEnable(); // Enables interrupts

	/* ### Register interrupts by OS. ### */
	struct sigevent intr_event;
	SIGEV_PULSE_INIT(&intr_event, conID, SIGEV_PULSE_PRIO_INHERIT,
					 PULSE_INTR_ON_PORT0, 0);
	interruptID = InterruptAttachEvent(INTR_GPIO_PORT0, &intr_event, 0);
	if (interruptID < 0)
	{
		perror("Interrupt was not able to be attached!");
		exit(EXIT_FAILURE);
	}

	// Enable interrupts on pins.
	uint32_t intEnable = (LB_START_PIN | LB_END_PIN | LB_RAMP_PIN | LB_SWITCH_PIN | LB_HEIGHT_PIN | LB_HEIGHT_OK_PIN);
	intEnable |= (SE_SWITCH_PIN | SE_METAL_PIN | KEY_START_PIN | KEY_STOP_PIN | KEY_RESET_PIN | ESTOP_PIN);
	out32(GPIO_IRQSTATUS_SET_1(gpio_bank_0), intEnable);
	uint32_t enabled = in32(GPIO_IRQSTATUS_SET_1(gpio_bank_0));
	std::bitset<32> enabledBits(enabled);

	// Set irq event types...
	uint32_t temp, rising, falling;

	//	(for rising edge detection)
	rising = (LB_START_PIN | LB_END_PIN | LB_RAMP_PIN | LB_SWITCH_PIN | LB_HEIGHT_PIN | LB_HEIGHT_OK_PIN);
	rising |= (SE_SWITCH_PIN | SE_METAL_PIN | KEY_START_PIN | KEY_STOP_PIN | KEY_RESET_PIN | ESTOP_PIN);
	temp = in32(GPIO_RISINGDETECT(gpio_bank_0));
	out32(GPIO_RISINGDETECT(gpio_bank_0), temp | rising);

	// 	(for falling edge detection)
	falling = (LB_START_PIN | LB_END_PIN | LB_RAMP_PIN | LB_SWITCH_PIN | LB_HEIGHT_PIN | LB_HEIGHT_OK_PIN);
	falling |= (SE_SWITCH_PIN | SE_METAL_PIN | KEY_START_PIN | KEY_STOP_PIN | KEY_RESET_PIN | ESTOP_PIN);
	temp = in32(GPIO_FALLINGDETECT(gpio_bank_0));
	out32(GPIO_FALLINGDETECT(gpio_bank_0), temp | falling);
}

void Sensors::handleEvent(Event event)
{
	Logger::debug("HAL handle Event: " + EVENT_TO_STRING(event.type));
}

void Sensors::startEventLoop()
{
	/* ### Start thread for handling interrupt messages. */
	eventLoopThread = std::thread(&Sensors::eventLoop, this);
}

void Sensors::stopEventLoop()
{
	if (eventLoopThread.joinable())
	{
		MsgSendPulse(conID, -1, PULSE_STOP_THREAD, 0); // using prio of calling thread.
		eventLoopThread.join();
	}
}

bool Sensors::lbStartBlocked()
{
	return BIT_NOTSET(LB_START_PIN, in32(GPIO_DATAIN(gpio_bank_0)));
}

bool Sensors::lbStartUnblocked()
{
	return BIT_SET(LB_START_PIN, in32(GPIO_DATAIN(gpio_bank_0)));
}

bool Sensors::lbSwitchBlocked()
{
	return BIT_NOTSET(LB_SWITCH_PIN, in32(GPIO_DATAIN(gpio_bank_0)));
}

bool Sensors::lbSwitchUnblocked()
{
	return BIT_SET(LB_SWITCH_PIN, in32(GPIO_DATAIN(gpio_bank_0)));
}

bool Sensors::lbRampBlocked()
{
	return BIT_NOTSET(LB_RAMP_PIN, in32(GPIO_DATAIN(gpio_bank_0)));
}

bool Sensors::lbRampUnblocked()
{
	return BIT_SET(LB_RAMP_PIN, in32(GPIO_DATAIN(gpio_bank_0)));
}

bool Sensors::metalDetected()
{
	return BIT_SET(SE_METAL_PIN, in32(GPIO_DATAIN(gpio_bank_0)));
}

bool Sensors::lbEndBlocked()
{
	return BIT_NOTSET(LB_END_PIN, in32(GPIO_DATAIN(gpio_bank_0)));
}

bool Sensors::lbEndUnblocked()
{
	return BIT_SET(LB_END_PIN, in32(GPIO_DATAIN(gpio_bank_0)));
}

bool Sensors::startPressed()
{
	return BIT_SET(KEY_START_PIN, in32(GPIO_DATAIN(gpio_bank_0)));
}

bool Sensors::stopPressed()
{
	return BIT_NOTSET(KEY_STOP_PIN, in32(GPIO_DATAIN(gpio_bank_0)));
}

bool Sensors::resetPressed()
{
	return BIT_SET(KEY_RESET_PIN, in32(GPIO_DATAIN(gpio_bank_0)));
}

bool Sensors::eStopPressed()
{
	return BIT_NOTSET(ESTOP_PIN, in32(GPIO_DATAIN(gpio_bank_0)));
}

bool Sensors::eStopReleased()
{
	return BIT_SET(ESTOP_PIN, in32(GPIO_DATAIN(gpio_bank_0)));
}

void Sensors::eventLoop()
{
	using namespace std;
	ThreadCtl(_NTO_TCTL_IO, 0); // Request IO privileges for this thread.

	cout << "Receiving routine started." << endl;
	_pulse msg;
	receivingRunning = true;
	while (receivingRunning)
	{
		int recvid = MsgReceivePulse(chanID, &msg, sizeof(_pulse), nullptr);
		if (recvid < 0)
		{
			perror("MsgReceivePulse failed!");
			exit(EXIT_FAILURE);
		}

		if (recvid == 0)
		{ // pulse received.
			// Stop thread while it blocks.
			if (msg.code == PULSE_STOP_THREAD)
			{
				Logger::debug("Thread kill code received!");
				receivingRunning = false;
				continue;
			}

			if (msg.code == PULSE_INTR_ON_PORT0)
			{
				handleGpioInterrupt();
			}

			// Do not ignore OS pulses!
		}
	}

	cout << "Message thread stops..." << endl;
}

void Sensors::handleGpioInterrupt()
{
	uint32_t intrStatusReg = in32(GPIO_IRQSTATUS_1(gpio_bank_0));

	// clear interrupts and unmask
	out32(GPIO_IRQSTATUS_1(gpio_bank_0), intrStatusReg);
	InterruptUnmask(INTR_GPIO_PORT0, interruptID);

	bool master = Configuration::getInstance().systemIsMaster();
	Event event;
	event.type = (EventType)-1;

	if (BIT_SET(ESTOP_PIN, intrStatusReg))
	{
		if (eStopPressed())
		{
			Logger::debug("ESTOP pressed");
			event.type = master ? EventType::ESTOP_M_PRESSED : EventType::ESTOP_S_PRESSED;
		}
		else
		{
			Logger::debug("ESTOP released");
			event.type = master ? EventType::ESTOP_M_RELEASED : EventType::ESTOP_S_RELEASED;
		}
	}
	else if (BIT_SET(KEY_START_PIN, intrStatusReg))
	{
		// TODO: Check if button is pressed short or long
		using namespace std::chrono;
		const auto now = steady_clock::now();
		int elapsed_ms = duration_cast<milliseconds>(now - lastStartBtnPressTime).count();
		if (startPressed())
		{
			Logger::debug("START pressed");
			event.type = master ? EventType::START_M_SHORT : EventType::START_S_SHORT;
		}
		else
		{
			Logger::debug("START released");
		}
	}
	else if (BIT_SET(KEY_STOP_PIN, intrStatusReg))
	{
		if (stopPressed())
		{
			Logger::debug("STOP pressed");
			event.type = master ? EventType::STOP_M_SHORT : EventType::STOP_S_SHORT;
		}
		else
		{
			Logger::debug("STOP released");
		}
	}
	else if (BIT_SET(KEY_RESET_PIN, intrStatusReg))
	{
		if (resetPressed())
		{
			Logger::debug("RESET pressed");
			event.type = master ? EventType::RESET_M_SHORT : EventType::RESET_S_SHORT;
		}
		else
		{
			Logger::debug("RESET released");
		}
	}
	else if (BIT_SET(LB_START_PIN, intrStatusReg))
	{
		if (lbStartBlocked())
		{
			Logger::debug("LBA blocked");
			event.type = master ? EventType::LBA_M_BLOCKED : EventType::LBA_S_BLOCKED;
		}
		else
		{
			Logger::debug("LBA unblocked");
			event.type = master ? EventType::LBA_M_UNBLOCKED : EventType::LBA_S_UNBLOCKED;
		}
	}
	else if (BIT_SET(LB_SWITCH_PIN, intrStatusReg))
	{
		if (lbSwitchBlocked())
		{
			Logger::debug("LBW blocked");
			event.type = master ? EventType::LBW_M_BLOCKED : EventType::LBW_S_BLOCKED;
		}
		else
		{
			Logger::debug("LBW unblocked");
			event.type = master ? EventType::LBW_M_UNBLOCKED : EventType::LBW_S_UNBLOCKED;
		}
	}
	else if (BIT_SET(LB_END_PIN, intrStatusReg))
	{
		if (lbEndBlocked())
		{
			Logger::debug("LBE blocked");
			event.type = master ? EventType::LBE_M_BLOCKED : EventType::LBE_S_BLOCKED;
		}
		else
		{
			Logger::debug("LBE unblocked");
			event.type = master ? EventType::LBE_M_UNBLOCKED : EventType::LBE_S_UNBLOCKED;
		}
	}
	else if (BIT_SET(LB_RAMP_PIN, intrStatusReg))
	{
		if (lbRampBlocked())
		{
			Logger::debug("LBR blocked");
			event.type = master ? EventType::LBR_M_BLOCKED : EventType::LBR_S_BLOCKED;
		}
		else
		{
			Logger::debug("LBR unblocked");
			event.type = master ? EventType::LBR_M_UNBLOCKED : EventType::LBR_S_UNBLOCKED;
		}
	}
	else if (BIT_SET(SE_METAL_PIN, intrStatusReg))
	{
		if (metalDetected())
		{
			Logger::debug("Metal detected");
			event.type = master ? EventType::MD_M_PAYLOAD : EventType::MD_S_PAYLOAD;
			event.data = 1;
		}
		else
		{
			event.type = master ? EventType::MD_M_PAYLOAD : EventType::MD_S_PAYLOAD;
			event.data = 0;
		}
	}
	// If IRQ is associated to an event, we send it!
	if ((int)event.type != -1)
	{
		eventManager->sendEvent(event);
	}
}