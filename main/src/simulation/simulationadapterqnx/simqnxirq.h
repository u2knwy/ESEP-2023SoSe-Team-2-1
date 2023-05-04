/*
 * simqnxirq.h
 *
 *  Created on: 23.04.2020
 *      Author: Lehmann
 */

#ifndef SRC_SIMULATION_SIMQNXIRQ_H_
#define SRC_SIMULATION_SIMQNXIRQ_H_

#include "simioimage.h"
#include <mutex>
#include <condition_variable>
#include <sys/siginfo.h>

static constexpr int GPIO_IRQ_NUMBER = 97;
static constexpr int ADC_IRQ_NUMBER = 16;

class SimQNXGPIO;

class SimQNXIRQ {
private:
	// IRQ-Handling
	unsigned long timestamp;
	SimulationIOImage lastImage;
	unsigned short lastADCRawValue;
	std::mutex buffermutex;
	std::mutex runmutex;
	std::condition_variable cv;
	SimQNXGPIO* gpio;
	bool run = true;
	bool pending = false;
	// QNX-API
	bool irqmasked_gpio;
	bool irqmasked_adc;
	const struct sigevent * (*isr_handler_gpio)(void *, int);
	const struct sigevent * (*isr_handler_adc)(void *, int);
	const void *isr_area_gpio;
	const void *isr_area_adc;
	unsigned int isr_flags_gpio;
	unsigned int isr_flags_adc;
	int isr_id_gpio;
	int isr_id_adc;
	struct sigevent event_gpio;
	struct sigevent event_adc;
	unsigned int event_flags_gpio;
	unsigned int event_flags_adc;
private:
	SimQNXIRQ();
public:
	// QNX-API simulation
	int simInterruptMask(int intr, int id);
	int simInterruptUnmask(int intr, int id);
	int simInterruptAttach(int intr,
			const struct sigevent * (*handler)(void *, int), const void * area,
			int size, unsigned flags);
	int simInterruptAttachEvent(int intr, const struct sigevent* event,
			unsigned flags);
	int simInterruptDetach(int id);

	//
	bool isGPIOMasked();
	bool isADCMasked();

	// Thread operations
	void operator()();
	void unmask_called();
	void irqPending();
	void kill() {run = false;};

	// IRQ handling
public:
	void raiseISRGPIO();
	void raiseISRADC();
	void raiseEventGPIO();
	void raiseEventADC();

	// singleton
public:
	static SimQNXIRQ* getSimIRQ() {
		static SimQNXIRQ instance;
		return &instance;
	}
};

#endif /* SRC_SIMULATION_SIMQNXIRQ_H_ */
