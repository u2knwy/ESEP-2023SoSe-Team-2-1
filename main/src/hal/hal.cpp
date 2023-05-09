/*
 * hal.cpp
 *
 *  Created on: 31.03.2023
 *      Author: Maik
 */

#include "hal.h"
#include <string>
#include <logger/logger.hpp>
#include <events/events.h>

HAL::HAL() {
	gpio_bank_0 = mmap_device_io(GPIO_SIZE, (uint64_t) GPIO_BANK_0);
	gpio_bank_1 = mmap_device_io(GPIO_SIZE, (uint64_t) GPIO_BANK_1);
	gpio_bank_2 = mmap_device_io(GPIO_SIZE, (uint64_t) GPIO_BANK_2);

	adc = new ADC(tsc);

	/* ### Create channel to receive interrupt pulse messages ### */
	chanID = ChannelCreate(0);
	if (chanID < 0) {
		perror("Could not create a channel!\n");
	}

	//Connect to channel.
	conID = ConnectAttach(0, 0, chanID, _NTO_SIDE_CHANNEL, 0);
	if (conID < 0) {
		perror("Could not connect to channel!");
	}

	/* ### Create channel to receive ADC values ### */
	adcChanID = ChannelCreate(0);
	if (adcChanID < 0) {
		perror("Could not create a channel!\n");
	}

	adcConID = ConnectAttach(0, 0, adcChanID, _NTO_SIDE_CHANNEL, 0); // Connect to channel.
	if (adcConID < 0) {
		perror("Could not connect to channel!");
	}

	receivingRunning = false;

	configurePins();
	initInterrupts();

	// Default: Stop Motor
	motorFast();
	motorStop();
}

HAL::~HAL() {
	stopEventLoop();
	stopHeightMeasurement();

	int detach_status = ConnectDetach(conID);
	if (detach_status != EOK) {
		perror("Detaching channel failed!");
		exit(EXIT_FAILURE);
	}

	int destroy_status = ChannelDestroy(chanID);
	if (destroy_status != EOK) {
		perror("Destroying channel failed!");
		exit(EXIT_FAILURE);
	}

	int adc_detach_status = ConnectDetach(adcConID);
	if (adc_detach_status != EOK) {
		perror("Detaching ADC channel failed!");
		exit(EXIT_FAILURE);
	}

	int adc_destroy_status = ChannelDestroy(adcChanID);
	if (adc_destroy_status != EOK) {
		perror("Destroying ADC channel failed!");
		exit(EXIT_FAILURE);
	}

	munmap_device_io(gpio_bank_0, GPIO_SIZE);
	munmap_device_io(gpio_bank_1, GPIO_SIZE);
	munmap_device_io(gpio_bank_2, GPIO_SIZE);

	delete adc;
}

void HAL::configurePins() {
	ThreadCtl(_NTO_TCTL_IO, 0);

	uint32_t temp, outputs, inputs;
	inputs = LB_START_PIN | LB_HEIGHT_PIN | LB_HEIGHT_OK_PIN | LB_SWITCH_PIN | SE_METAL_PIN | SE_SWITCH_PIN | LB_RAMP_PIN | LB_END_PIN;
	inputs = inputs | KEY_START_PIN | KEY_STOP_PIN | KEY_RESET_PIN | ESTOP_PIN;

	// Configure GPIOs as inputs
	temp = in32((uintptr_t) gpio_bank_0 + GPIO_OE_REGISTER);
	out32((uintptr_t) gpio_bank_0 + GPIO_OE_REGISTER, temp & inputs);

	// Configure GPIOs as outputs
	// Port 1
	outputs = MOTOR_LEFT_PIN | MOTOR_RIGHT_PIN | MOTOR_SLOW_PIN | MOTOR_STOP_PIN | LAMP_RED_PIN | LAMP_YELLOW_PIN | LAMP_GREEN_PIN | SWITCH_PIN;
	temp = in32((uintptr_t) gpio_bank_1 + GPIO_OE_REGISTER);
	out32((uintptr_t) gpio_bank_1 + GPIO_OE_REGISTER, temp & ~outputs);

	// Port 2
	outputs = LED_Q1_PIN | LED_Q2_PIN | LED_RESET_PIN | LED_START_PIN;
	temp = in32((uintptr_t) gpio_bank_2 + GPIO_OE_REGISTER);
	out32((uintptr_t) gpio_bank_2 + GPIO_OE_REGISTER, temp & ~outputs);
}

void HAL::initInterrupts() {
	using namespace std;

	/* ### Setup ### */
	ThreadCtl(_NTO_TCTL_IO, 0);	//Request IO privileges for process.

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

	InterruptEnable(); //Enables interrupts

	/* ### Register interrupts by OS. ### */
	struct sigevent intr_event;
	SIGEV_PULSE_INIT(&intr_event, conID, SIGEV_PULSE_PRIO_INHERIT,
			PULSE_INTR_ON_PORT0, 0);
	interruptID = InterruptAttachEvent(INTR_GPIO_PORT0, &intr_event, 0);
	if (interruptID < 0) {
		perror("Interrupt was not able to be attached!");
		exit(EXIT_FAILURE);
	}

	// Enable interrupts on pins.
	uint32_t intEnable = (LB_START_PIN | LB_END_PIN | LB_RAMP_PIN | LB_SWITCH_PIN | LB_HEIGHT_PIN | LB_HEIGHT_OK_PIN);
	intEnable |= (SE_SWITCH_PIN | SE_METAL_PIN | KEY_START_PIN | KEY_STOP_PIN | KEY_RESET_PIN | ESTOP_PIN);
	out32((uintptr_t) gpio_bank_0 + GPIO_IRQSTATUS_SET_1, intEnable);
	uint32_t enabled = in32((uintptr_t) gpio_bank_0 + GPIO_IRQSTATUS_SET_1);
	std::bitset<32> enabledBits(enabled);
	Logger::debug("INT enabled: " + enabledBits.to_string());

	// Set irq event types...
	uint32_t temp, rising, falling;

	//	(for rising edge detection)
	rising = (LB_START_PIN | LB_END_PIN | LB_RAMP_PIN | LB_SWITCH_PIN | LB_HEIGHT_PIN | LB_HEIGHT_OK_PIN);
	rising |= (SE_SWITCH_PIN | SE_METAL_PIN | KEY_START_PIN | KEY_STOP_PIN | KEY_RESET_PIN | ESTOP_PIN);
	temp = in32((uintptr_t) gpio_bank_0 + GPIO_RISINGDETECT);
	out32((uintptr_t) (gpio_bank_0 + GPIO_RISINGDETECT), temp | rising);

	// 	(for falling edge detection)
	falling = (LB_START_PIN | LB_END_PIN | LB_RAMP_PIN | LB_SWITCH_PIN | LB_HEIGHT_PIN | LB_HEIGHT_OK_PIN);
	falling |= (SE_SWITCH_PIN | SE_METAL_PIN | KEY_START_PIN | KEY_STOP_PIN | KEY_RESET_PIN | ESTOP_PIN);
	temp = in32((uintptr_t) gpio_bank_0 + GPIO_FALLINGDETECT);
	out32((uintptr_t) (gpio_bank_0 + GPIO_FALLINGDETECT), temp | falling);
}

void HAL::handleEvent(EventType eventType) {
	Logger::debug("HAL handle Event: " + EVENT_TO_STRING(eventType));
}

void HAL::startEventLoop() {
	/* ### Start thread for handling interrupt messages. */
	eventLoopThread = std::thread(&HAL::eventLoop, this);
}

void HAL::stopEventLoop() {
	if (eventLoopThread.joinable()) {
		MsgSendPulse(conID, -1, PULSE_STOP_THREAD, 0); // using prio of calling thread.
		eventLoopThread.join();
	}
}

void HAL::GreenLampOn() {
	out32((uintptr_t) (gpio_bank_1 + GPIO_SETDATAOUT), LAMP_GREEN_PIN);
}

void HAL::GreenLampOff() {
	out32((uintptr_t) (gpio_bank_1 + GPIO_CLEARDATAOUT), LAMP_GREEN_PIN);
}

void HAL::YellowLampOn() {
	out32((uintptr_t) (gpio_bank_1 + GPIO_SETDATAOUT), LAMP_YELLOW_PIN);
}

void HAL::YellowLampOff() {
	out32((uintptr_t) (gpio_bank_1 + GPIO_CLEARDATAOUT), LAMP_YELLOW_PIN);
}

void HAL::RedLampOn() {
	out32((uintptr_t) (gpio_bank_1 + GPIO_SETDATAOUT), LAMP_RED_PIN);
}

void HAL::RedLampOff() {
	out32((uintptr_t) (gpio_bank_1 + GPIO_CLEARDATAOUT), LAMP_RED_PIN);
}

void HAL::StartLedOn() {
	out32((uintptr_t) (gpio_bank_2 + GPIO_SETDATAOUT), LED_START_PIN);
}

void HAL::StartLedOff() {
	out32((uintptr_t) (gpio_bank_2 + GPIO_CLEARDATAOUT), LED_START_PIN);
}

void HAL::ResetLedOn() {
	out32((uintptr_t) (gpio_bank_2 + GPIO_SETDATAOUT), LED_RESET_PIN);
}

void HAL::ResetLedOff() {
	out32((uintptr_t) (gpio_bank_2 + GPIO_CLEARDATAOUT), LED_RESET_PIN);
}

void HAL::Q1LedOn() {
	out32((uintptr_t) (gpio_bank_2 + GPIO_SETDATAOUT), LED_Q1_PIN);
}

void HAL::Q1LedOff() {
	out32((uintptr_t) (gpio_bank_2 + GPIO_CLEARDATAOUT), LED_Q1_PIN);
}

void HAL::Q2LedOn() {
	out32((uintptr_t) (gpio_bank_2 + GPIO_SETDATAOUT), LED_Q2_PIN);
}

void HAL::Q2LedOff() {
	out32((uintptr_t) (gpio_bank_2 + GPIO_CLEARDATAOUT), LED_Q2_PIN);
}

void HAL::motorSlow() {
	out32((uintptr_t) (gpio_bank_1 + GPIO_SETDATAOUT), MOTOR_SLOW_PIN);
}

void HAL::motorFast() {
	out32((uintptr_t) (gpio_bank_1 + GPIO_CLEARDATAOUT), MOTOR_SLOW_PIN);
}

void HAL::motorRight() {
	out32((uintptr_t) (gpio_bank_1 + GPIO_CLEARDATAOUT), MOTOR_STOP_PIN);
	out32((uintptr_t) (gpio_bank_1 + GPIO_CLEARDATAOUT), MOTOR_LEFT_PIN);
	out32((uintptr_t) (gpio_bank_1 + GPIO_SETDATAOUT), MOTOR_RIGHT_PIN);
}

void HAL::motorLeft() {
	out32((uintptr_t) (gpio_bank_1 + GPIO_CLEARDATAOUT), MOTOR_STOP_PIN);
	out32((uintptr_t) (gpio_bank_1 + GPIO_SETDATAOUT), MOTOR_LEFT_PIN);
	out32((uintptr_t) (gpio_bank_1 + GPIO_CLEARDATAOUT), MOTOR_RIGHT_PIN);
}

void HAL::motorStop() {
	out32((uintptr_t) (gpio_bank_1 + GPIO_SETDATAOUT), MOTOR_STOP_PIN);
	out32((uintptr_t) (gpio_bank_1 + GPIO_CLEARDATAOUT), MOTOR_LEFT_PIN);
	out32((uintptr_t) (gpio_bank_1 + GPIO_CLEARDATAOUT), MOTOR_RIGHT_PIN);
}

void HAL::openSwitch() {
	out32((uintptr_t) (gpio_bank_1 + GPIO_SETDATAOUT), SWITCH_PIN);
}

void HAL::closeSwitch() {
	out32((uintptr_t) (gpio_bank_1 + GPIO_CLEARDATAOUT), SWITCH_PIN);
}

void HAL::startHeightMeasurement() {
	using namespace std;
	std::cout << "Start height measurement" << std::endl;

	adc->registerAdcISR(adcConID, PULSE_ADC_SAMPLING_DONE);

	// ### Start thread for handling interrupt messages.
	adcReceivingThread = std::thread(&HAL::adcReceivingRoutine, this);

	adc->sample();
}

void HAL::stopHeightMeasurement() {

	// Detach interrupts.
	adc->adcDisable();
	adc->unregisterAdcISR();

	if (adcReceivingThread.joinable()) {
		Logger::debug("Stop height measurement");
		// Stop receiving thread.
		MsgSendPulse(adcConID, -1, PULSE_STOP_THREAD, 0);
		adcReceivingThread.join();
	}
}

void HAL::eventLoop() {
	using namespace std;
	ThreadCtl(_NTO_TCTL_IO, 0); // Request IO privileges for this thread.

	cout << "Receiving routine started." << endl;
	_pulse msg;
	receivingRunning = true;
	while (receivingRunning) {
		int recvid = MsgReceivePulse(chanID, &msg, sizeof(_pulse), nullptr);
		if (recvid < 0) {
			perror("MsgReceivePulse failed!");
			exit(EXIT_FAILURE);
		}

		if (recvid == 0) { // pulse received.
			// Stop thread while it blocks.
			if (msg.code == PULSE_STOP_THREAD) {
				Logger::debug("Thread kill code received!");
				receivingRunning = false;
				continue;
			}

			if (msg.code == PULSE_INTR_ON_PORT0) {
				Logger::debug("Received GPIO interrupt on port 0");
				handleGpioInterrupt();
			}

			// Do not ignore OS pulses!
		}
	}

	cout << "Message thread stops..." << endl;
}

void HAL::handleGpioInterrupt() {
	unsigned int intrStatusReg = in32(
			uintptr_t(gpio_bank_0 + GPIO_IRQSTATUS_1));

	out32(uintptr_t(gpio_bank_0 + GPIO_IRQSTATUS_1), 0xffffffff);//clear all interrupts
	InterruptUnmask(INTR_GPIO_PORT0, interruptID);			//unmask interrupt

	for (int pin = 0; pin < 32; pin++) {
		unsigned int mask = (uint32_t) BIT_MASK(pin);
		if (intrStatusReg == mask) {
			int current_level = (in32((uintptr_t) gpio_bank_0 + GPIO_DATAIN)
					>> pin) & 0x1;
			printf("Interrupt on pin %d, now %d\n", pin, current_level);
		}
	}
}

void HAL::adcReceivingRoutine() {
	ThreadCtl(_NTO_TCTL_IO, 0); // Request IO privileges for this thread.

	Logger::debug("ADC receiving routine started...");

	using namespace std;
	_pulse msg;
	receivingRunning = true;
	while (receivingRunning) {
		int recvid = MsgReceivePulse(adcChanID, &msg, sizeof(_pulse), nullptr);

		if (recvid < 0) {
			perror("MsgReceivePulse failed!");
			exit(EXIT_FAILURE);
		}

		if (recvid == 0) { // pulse received.

			// Stop thread while it blocks.
			if (msg.code == PULSE_STOP_THREAD) {
				std::cout << "ADC thread kill code received" << std::endl;
				receivingRunning = false;
				continue;
			}

			// ADC interrupt value.
			if (msg.code == PULSE_ADC_SAMPLING_DONE) {
				printf("Value from ADC: %d\n", msg.value);
				this_thread::sleep_for(chrono::milliseconds(100));
				adc->sample();
			}

			// Do not ignore OS pulses!
		}
	}

	std::cout << "ADC thread stops..." << std::endl;
}
