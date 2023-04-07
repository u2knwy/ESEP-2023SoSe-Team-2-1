/*
 * hal.cpp
 *
 *  Created on: 31.03.2023
 *      Author: Maik
 */

#include "hal.h"
#include <string>

HAL::HAL() {
	gpio_bank_0 = mmap_device_io(GPIO_SIZE, (uint64_t) GPIO_BANK_0);
	gpio_bank_1 = mmap_device_io(GPIO_SIZE, (uint64_t) GPIO_BANK_1);
	gpio_bank_2 = mmap_device_io(GPIO_SIZE, (uint64_t) GPIO_BANK_2);

	TSCADC tsc;
	adc = new ADC(tsc);

	receivingRunning = false;

	initInterrupts();
}

HAL::~HAL() {
	int destroy_status = ChannelDestroy(chanID);
	if (destroy_status != EOK) {
		perror("Destroying channel failed!");
		exit(EXIT_FAILURE);
	}

	munmap_device_io(gpio_bank_0, GPIO_SIZE);
	munmap_device_io(gpio_bank_1, GPIO_SIZE);
	munmap_device_io(gpio_bank_2, GPIO_SIZE);

	delete adc;
}

void HAL::initInterrupts() {
	// Request interrupt and IO abilities.
	int procmgr_status = procmgr_ability(
		0,
		PROCMGR_ADN_ROOT | PROCMGR_AOP_ALLOW | PROCMGR_AID_INTERRUPT,
		PROCMGR_ADN_NONROOT | PROCMGR_AOP_ALLOW | PROCMGR_AID_INTERRUPT,
		PROCMGR_ADN_ROOT | PROCMGR_AOP_ALLOW | PROCMGR_AID_IO,
		PROCMGR_ADN_NONROOT | PROCMGR_AOP_ALLOW | PROCMGR_AID_IO,
		PROCMGR_AID_EOL
	);
	if(procmgr_status != EOK){
		perror("Requested abilities failed or denied!");
		exit(EXIT_FAILURE);
	}

	InterruptEnable(); //Enables interrupts

	/* ### Create channel ### */
	chanID = ChannelCreate(0);//Create channel to receive interrupt pulse messages.
	if (chanID < 0) {
		perror("Could not create a channel!\n");
	}

	int conID = ConnectAttach(0, 0, chanID, _NTO_SIDE_CHANNEL, 0); //Connect to channel.
	if (conID < 0) {
		perror("Could not connect to channel!");
	}

	/* ### Register interrupts by OS. ### */
	struct sigevent intr_event;
	SIGEV_PULSE_INIT(&intr_event, conID, SIGEV_PULSE_PRIO_INHERIT, PULSE_INTR_ON_PORT0, 0);
	interruptID = InterruptAttachEvent(INTR_GPIO_PORT0, &intr_event, 0);
	if (interruptID < 0) {
		perror("Interrupt was not able to be attached!");
	}

	// Enable interrupts on pins.
	out32((uintptr_t) gpio_bank_0 + GPIO_IRQSTATUS_SET_1, LB_START_PIN);
	out32((uintptr_t) gpio_bank_0 + GPIO_IRQSTATUS_SET_1, LB_END_PIN);

	// Set irq event types.
	unsigned int temp;

	//	(for rising edge detection)
	temp = in32((uintptr_t) (gpio_bank_0 + GPIO_RISINGDETECT));
	std::bitset<32> reg(temp);
	temp |= (BIT_MASK(LB_START_PIN) | BIT_MASK(KEY_START_PIN));
	reg = std::bitset<32>(temp);
	out32((uintptr_t) (gpio_bank_0 + GPIO_RISINGDETECT), temp);

	// 	(for falling edge detection)
	temp = in32((uintptr_t) (gpio_bank_0 + GPIO_FALLINGDETECT));
	std::bitset<32> reg2(temp);
	temp |= (BIT_MASK(LB_START_PIN) | BIT_MASK(KEY_START_PIN));
	reg2 = std::bitset<32>(temp);
	out32((uintptr_t) (gpio_bank_0 + GPIO_FALLINGDETECT), temp);
}

void HAL::GreenLampOn() {
	out32((uintptr_t) (gpio_bank_1 + GPIO_SETDATAOUT), LAMP_GREEN_PIN); // set register
}

void HAL::GreenLampOff() {
	out32((uintptr_t) (gpio_bank_1 + GPIO_CLEARDATAOUT), LAMP_GREEN_PIN); // clear register
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

void HAL::receivingRoutine() {
	ThreadCtl(_NTO_TCTL_IO, 0); // Request IO privileges for this thread.

	_pulse msg;
	receivingRunning = true;

	using namespace std;

	cout << "Receiving routine started." << endl;

	while (receivingRunning) {
		int recvid = MsgReceivePulse(chanID, &msg, sizeof(_pulse), nullptr);

		if (recvid < 0) {
			perror("MsgReceivePulse failed!");
			exit(EXIT_FAILURE);
		}

		if (recvid == 0) { // pulse received.

			// Stop thread while it blocks.
			if (msg.code == PULSE_STOP_THREAD) {
				cout << "Thread kill code received!" << endl;
				receivingRunning = false;
				continue;
			}

			// ADC interrupt value.
			if (msg.code == PULSE_ADC_SAMPLING_DONE) {
				cout << "Value from adc with value " << msg.value.sival_int << endl;
				adc->sample();
			}

			if (msg.code == PULSE_INTR_ON_PORT0){
				cout << "Received GPIO interrupt on port 0" << endl;
				handleGpioInterrupt();
			}

			// Do not ignore OS pulses!
		}
	}

	cout << "Message thread stops..." << endl;
}

void HAL::handleGpioInterrupt() {
	unsigned int intrStatusReg = in32(uintptr_t(gpio_bank_0 + GPIO_IRQSTATUS_1));

	out32(uintptr_t(gpio_bank_0 + GPIO_IRQSTATUS_1), 0xffffffff);	//clear all interrupts
	InterruptUnmask(INTR_GPIO_PORT0, interruptID);					//unmask interrupt

	for (int pin = 0; pin < 32; pin++) {
		unsigned int mask = (uint32_t) BIT_MASK(pin);
		if (intrStatusReg == mask) {
			int current_level = (in32((uintptr_t) gpio_bank_0 + GPIO_DATAIN) >> pin) & 0x1;
			printf("Interrupt on pin %d, now %d\n", pin, current_level);
		}
	}
}

void HAL::adcDemo() {
	using namespace std;

	// Start motor slow for demo
	motorSlow();

	int conID = ConnectAttach(0, 0, chanID, _NTO_SIDE_CHANNEL, 0); // Connect to channel.
	if (conID < 0) {
		perror("Could not connect to channel!");
	}

	adc->registerAdcISR(conID, PULSE_ADC_SAMPLING_DONE);

	// ### Start thread for handling interrupt messages.
	thread receivingThread(&HAL::receivingRoutine, this);

	adc->sample();

#define DEMO_DURATION 120
	cout << "Demo time for " << DEMO_DURATION << " minutes..." << endl;
	this_thread::sleep_for(chrono::seconds(DEMO_DURATION));
	cout << "Stopping in..." << endl;
	cout << "3" << endl;
	this_thread::sleep_for(chrono::seconds(1));
	cout << "2" << endl;
	this_thread::sleep_for(chrono::seconds(1));
	cout << "1" << endl;
	this_thread::sleep_for(chrono::seconds(1));
	cout << "NOW!" << endl;

	// ### Cleaning up ###

	// Detach interrupts.
	adc->adcDisable();
	adc->unregisterAdcISR();

	// Stop receiving thread.
	MsgSendPulse(conID, -1, PULSE_STOP_THREAD, 0); // using prio of calling thread.
	receivingThread.join();

	// Close channel
	int detach_status = ConnectDetach(conID);
	if (detach_status != EOK) {
		perror("Detaching channel failed!");
		exit(EXIT_FAILURE);
	}

	// Stop motor for demo
	motorStop();
	// ### END ADC TEST
}

void HAL::gpioDemo() {
	using namespace std;

	initInterrupts();

	int conID = ConnectAttach(0, 0, chanID, _NTO_SIDE_CHANNEL, 0); // Connect to channel.
	if (conID < 0) {
		perror("Could not connect to channel!");
	}

	/* ### Start thread for handling interrupt messages. */
	std::thread receivingThread(&HAL::receivingRoutine, this);

#define DEMO_DURATION 120
	cout << "Demo time for " << DEMO_DURATION << " minutes..." << endl;
	this_thread::sleep_for(chrono::seconds(DEMO_DURATION));
	cout << "Stopping in..." << endl;
	cout << "3" << endl;
	this_thread::sleep_for(chrono::seconds(1));
	cout << "2" << endl;
	this_thread::sleep_for(chrono::seconds(1));
	cout << "1" << endl;
	this_thread::sleep_for(chrono::seconds(1));
	cout << "NOW!" << endl;

	// Stop receiving thread.
	MsgSendPulse(conID, -1, PULSE_STOP_THREAD, 0); // using prio of calling thread.
	receivingThread.join();

	// Close channel
	int detach_status = ConnectDetach(conID);
	if (detach_status != EOK) {
		perror("Detaching channel failed!");
		exit(EXIT_FAILURE);
	}
}
