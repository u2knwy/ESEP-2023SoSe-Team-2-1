/*
 * interrupts.hpp
 *
 *  Created on: 17.04.2023
 *      Author: Maik
 */
#pragma once

#include <stdio.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/procmgr.h>
#include <iostream>
#include <thread>
#include <chrono>

#include <sys/mman.h>
#include <hw/inout.h>
#include "hal.h"
#include "macros.h"

/* Code configuration */
#define DEMO true			//true for demo time, false to spin forever.
#define DEMO_DURATION 3		//Duration of demo time in minutes.

/* My pulse codes */
#define PULSE_STOP_THREAD 		_PULSE_CODE_MINAVAIL + 1
#define PULSE_INTR_ON_PORT0 	_PULSE_CODE_MINAVAIL + 2

/* Prototypes*/
int main(int argc, char** args);
void receivingRoutine(int);
void handleInterrupt();

/* Variables */
bool receivingRunning = false;
int interruptID;

/* Implementation */
using namespace std;

int sensorDemo() {

	/* ### Setup ### */
	ThreadCtl(_NTO_TCTL_IO, 0);	//Request IO privileges for process.

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

	InterruptEnable();			//Enables interrupts.

	// Start motor for demo.
	uintptr_t port1BaseAddr = mmap_device_io(GPIO_REGISTER_LENGTH, GPIO_PORT1);
	out32((uintptr_t) port1BaseAddr + GPIO_SETDATAOUT, BIT_MASK(MOTOR_RIGHT_PIN));
	out32((uintptr_t) port1BaseAddr + GPIO_SETDATAOUT, BIT_MASK(MOTOR_SLOW_PIN));
	out32((uintptr_t) port1BaseAddr + GPIO_CLEARDATAOUT, BIT_MASK(MOTOR_STOP_PIN));

	/* ### Create channel ### */
	int chanID = ChannelCreate(0);//Create channel to receive interrupt pulse messages.
	if (chanID < 0) {
		perror("Could not create a channel!\n");
		return EXIT_FAILURE;
	}

	int conID = ConnectAttach(0, 0, chanID, _NTO_SIDE_CHANNEL, 0); //Connect to channel.
	if (conID < 0) {
		perror("Could not connect to channel!");
		return EXIT_FAILURE;
	}

	/* ### Register interrupts by OS. ### */
	struct sigevent intr_event;
	SIGEV_PULSE_INIT(&intr_event, conID, SIGEV_PULSE_PRIO_INHERIT, PULSE_INTR_ON_PORT0, 0);
	interruptID = InterruptAttachEvent(INTR_GPIO_PORT0, &intr_event, 0);
	if (interruptID < 0) {
		perror("Interrupt was not able to be attached!");
		return EXIT_FAILURE;
	}

	/* ### Configure registers to receive irq events. */
	uintptr_t port0BaseAddr = mmap_device_io(GPIO_REGISTER_LENGTH, GPIO_PORT0);

	// Enable interrupts on pins.
	out32((uintptr_t) port0BaseAddr + GPIO_IRQSTATUS_SET_1, BIT_MASK(LIGHTBARRIER_FRONT_PIN));
	out32((uintptr_t) port0BaseAddr + GPIO_IRQSTATUS_SET_1, BIT_MASK(BUTTON_START_PIN));

	// Set irq event types.
	unsigned int temp;

	//	(for rising edge detection)
	temp = in32((uintptr_t) (port0BaseAddr + GPIO_RISINGDETECT));			//Read current config.
	std::cout << "Default rising: " << temp << "\n";
	std::bitset<32> reg(temp);
	cout << "Rising register - pre : "<< reg << "\n";
	temp |= (BIT_MASK(LIGHTBARRIER_FRONT_PIN) | BIT_MASK(BUTTON_START_PIN));//Add desired pins.
	reg = bitset<32>(temp);
	cout << "Rising register - post: "<< reg << "\n";
	out32((uintptr_t) (port0BaseAddr + GPIO_RISINGDETECT), temp);			//Write new config back.

	// 	(for falling edge detection)
	temp = in32((uintptr_t) (port0BaseAddr + GPIO_FALLINGDETECT));			//Read current config.
	std::cout << "Default falling: " << temp << "\n";
	std::bitset<32> reg2(temp);
	cout << "Falling register - pre : "<< reg2 << "\n";
	temp |= (BIT_MASK(LIGHTBARRIER_FRONT_PIN) | BIT_MASK(BUTTON_START_PIN));//Add desired pins.
	reg2 = bitset<32>(temp);
	cout << "Falling register - post: "<< reg2 << "\n";
	out32((uintptr_t) (port0BaseAddr + GPIO_FALLINGDETECT), temp);			//Write new config back.

	/* ### Start thread for handling interrupt messages. */
	thread receivingThread = thread(&receivingRoutine, chanID);

	/* ### Waiting for demo. ### */
	if(DEMO){
		printf("Demo time for %d minutes...\n", DEMO_DURATION);
		this_thread::sleep_for(chrono::minutes(DEMO_DURATION));	//main thread sleep.
		printf("Stopping in...\n");
		printf("3\n");
		this_thread::sleep_for(chrono::seconds(1));
		printf("2\n");
		this_thread::sleep_for(chrono::seconds(1));
		printf("1\n");
		this_thread::sleep_for(chrono::seconds(1));
		printf("NOW!\n");
	} else {
		while(true){
			this_thread::sleep_for(chrono::hours(999999));
		}
	}

	/* ### Cleaning up. */

	// Detach interrupts.
	int intr_detach_status = InterruptDetach(interruptID);
	if(intr_detach_status != EOK){
		perror("Detaching interrupt failed!");
		exit(EXIT_FAILURE);
	}

	// Stop receiving thread.
	MsgSendPulse(conID, -1, PULSE_STOP_THREAD, 0); //using prio of calling thread.
	receivingThread.join();

	// Close channel
	int detach_status = ConnectDetach(conID);
	if(detach_status != EOK){
		perror("Detaching channel failed!");
		exit(EXIT_FAILURE);
	}

	int destroy_status = ChannelDestroy(chanID);
	if(destroy_status != EOK){
		perror("Destroying channel failed!");
		exit(EXIT_FAILURE);
	}

	out32((uintptr_t) port1BaseAddr + GPIO_CLEARDATAOUT, BIT_MASK(MOTOR_RIGHT_PIN));
	out32((uintptr_t) port1BaseAddr + GPIO_SETDATAOUT, BIT_MASK(MOTOR_STOP_PIN));

	return EXIT_SUCCESS;
}

void receivingRoutine(int channelID) {

	ThreadCtl( _NTO_TCTL_IO, 0);	//Request IO privileges

	_pulse msg;
	receivingRunning = true;

	printf("Message thread started.\n");

	while (receivingRunning) {
		int recvid = MsgReceivePulse(channelID, &msg, sizeof(_pulse), nullptr);

		if (recvid < 0) {
			perror("MsgReceivePulse failed!");
			exit(EXIT_FAILURE);
		}

		if (recvid == 0) {	//pulse received.

			//Stop thread while it blocks.
			if (msg.code == PULSE_STOP_THREAD) {
				printf("Thread kill code received!\n");
				receivingRunning = false;
			}

			if (msg.code == PULSE_INTR_ON_PORT0){
				handleInterrupt();
			}

			// Do not ignore OS pulses!
		}
	}

	printf("Message thread stops...\n");
}

void handleInterrupt(void) {
	uintptr_t gpioBase = mmap_device_io(GPIO_REGISTER_LENGTH, GPIO_PORT0);

	unsigned int intrStatusReg = in32(uintptr_t(gpioBase + GPIO_IRQSTATUS_1));

	out32(uintptr_t(gpioBase + GPIO_IRQSTATUS_1), 0xffffffff);	//clear all interrupts.
	InterruptUnmask(INTR_GPIO_PORT0, interruptID);				//unmask interrupt.

	for (int pin = 0; pin < 32; pin++) {
		unsigned int mask = (uint32_t) BIT_MASK(pin);
		if (intrStatusReg == mask) {
			int current_level = (in32((uintptr_t) gpioBase + GPIO_DATAIN) >> pin) & 0x1;
			printf("Interrupt on pin %d, now %d\n", pin, current_level);
		}
	}
}
