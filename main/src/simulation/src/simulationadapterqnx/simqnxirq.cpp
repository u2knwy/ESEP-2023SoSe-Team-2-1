/*
 * simqnxirq.cpp
 *
 *  Created on: 23.04.2020
 *      Author: Lehmann
 */
#include "simqnxirq.h"
#include "simqnxgpio.h"
#include "simconfquery.h"
#include <sys/neutrino.h>
#include <iostream>
#include <sys/siginfo.h>
#include <sys/platform.h>
using namespace std;

SimQNXIRQ::SimQNXIRQ() :
		timestamp(0), lastADCRawValue(0),
		gpio(nullptr),
		run(0), pending(0),
		irqmasked_gpio(false), irqmasked_adc(false),
		isr_handler_gpio(nullptr), isr_handler_adc(nullptr),
		isr_area_gpio(nullptr),  isr_area_adc(nullptr),
		isr_flags_gpio(0), isr_flags_adc(0),
		isr_id_gpio(-1), isr_id_adc(-1),
		event_flags_gpio(0), event_flags_adc(0) {
	gpio = SimQNXGPIO::getGPIO();

	event_gpio.sigev_notify = 0;
	event_adc.sigev_notify = 0;
}

int SimQNXIRQ::simInterruptMask(int intr, int id) {
	int result = -1;
	if (intr == GPIO_IRQ_NUMBER) {
		if (id == isr_id_gpio) {
			irqmasked_gpio = true;
			result = 1;
		}
	}
	if (intr == ADC_IRQ_NUMBER) {
		if (id == isr_id_adc) {
			irqmasked_adc = true;
			result = 1;
		}
	}
	return result;
}

int SimQNXIRQ::simInterruptUnmask(int intr, int id) {
	int result = -1;
	if (intr == GPIO_IRQ_NUMBER) {
		if (id == isr_id_gpio) {
			irqmasked_gpio = false;
			result = 0;
		}
	}
	if (intr == ADC_IRQ_NUMBER) {
		if (id == isr_id_adc) {
			irqmasked_adc = false;
			result = 0;
		}
	}
	if(result == 0){
		unmask_called();
	}

	return result;
}

bool SimQNXIRQ::isGPIOMasked() {return irqmasked_gpio;}
bool SimQNXIRQ::isADCMasked() {return irqmasked_adc;};

int SimQNXIRQ::simInterruptAttach(int intr,
		const struct sigevent * (*handler)(void *, int), const void * area,
		int size, unsigned flags) {
	int result = -1;
	if (intr == GPIO_IRQ_NUMBER) {
		irqmasked_gpio = true;    // avoid raise during initialisation
		isr_handler_gpio = handler;
		isr_area_gpio = area;
		// size is ignored according to QNX documentation.
		isr_flags_gpio = flags;
		isr_id_gpio = 1;
		result = isr_id_gpio;
		irqmasked_gpio = false;
	}
	if (intr == ADC_IRQ_NUMBER) {
		irqmasked_adc = true;     // avoid raise during initialisation
		isr_handler_adc = handler;
		isr_area_adc = area;
		// size is ignored according to QNX documentation.
		isr_flags_adc = flags;
		isr_id_adc = 2;
		result = isr_id_adc;
		irqmasked_adc = false;
	}
	return result;
}

void SimQNXIRQ::raiseISRGPIO() {
	const struct sigevent * isr_result;
	if (isr_handler_gpio != nullptr && !irqmasked_gpio) {
		isr_result = isr_handler_gpio((void*) isr_area_gpio, isr_id_gpio);
		if (isr_result != NULL) {
			if ((isr_result->sigev_notify == SIGEV_PULSE)
					|| (isr_result->sigev_notify == (SIGEV_PULSE & 0x0f))) {
				MsgSendPulse(isr_result->sigev_coid, isr_result->sigev_priority,
						isr_result->sigev_code,
						isr_result->sigev_value.sival_int);

			}
		}
	}
}

void SimQNXIRQ::raiseISRADC() {
	const struct sigevent * isr_result;
	if (isr_handler_adc != nullptr && !irqmasked_adc) {
		isr_result = isr_handler_adc((void*) isr_area_adc, isr_id_adc);
		if (isr_result != NULL) {
			if ((isr_result->sigev_notify == SIGEV_PULSE)
					|| (isr_result->sigev_notify == (SIGEV_PULSE & 0x0f))) {
				MsgSendPulse(isr_result->sigev_coid, isr_result->sigev_priority,
						isr_result->sigev_code,
						isr_result->sigev_value.sival_int);

			}
		}
	}
}

void SimQNXIRQ::raiseEventGPIO() {
	if(!irqmasked_gpio){
		if ((event_gpio.sigev_notify == SIGEV_PULSE)
				|| (event_gpio.sigev_notify == (SIGEV_PULSE & 0x0f))) {
			irqmasked_gpio = true;
			MsgSendPulse(event_gpio.sigev_coid, event_gpio.sigev_priority,
					event_gpio.sigev_code, event_gpio.sigev_value.sival_int);
		}
	}
}

void SimQNXIRQ::raiseEventADC() {
	if(!irqmasked_adc){
		if ((event_adc.sigev_notify == (SIGEV_PULSE))
				|| (event_adc.sigev_notify == (SIGEV_PULSE & 0x0f))) {
			irqmasked_adc = true;
			MsgSendPulse(event_adc.sigev_coid, event_adc.sigev_priority,
					event_adc.sigev_code, event_adc.sigev_value.sival_int);
		}
	}
}

int SimQNXIRQ::simInterruptAttachEvent(int intr, const struct sigevent* event,
		unsigned flags) {
	int result = -1;
	if (intr == GPIO_IRQ_NUMBER) {
		memcpy(&(this->event_gpio), event, sizeof(struct sigevent));
		//this->event = event;
		event_flags_gpio = flags;
		isr_id_gpio = 1;
		// Event type check...
		//if ((SIGEV_GET_TYPE(event)!= (SIGEV_PULSE))
	    if ((event->sigev_notify != SIGEV_PULSE)
				&& (event->sigev_notify != (SIGEV_PULSE & 0x0f))) { // magic due to 32-bit system
			cout << "No Pulse Signal given " << endl;
			isr_id_gpio = -1;
		}
		result = isr_id_gpio;
		irqmasked_gpio = false;
	}
	if (intr == ADC_IRQ_NUMBER) {
		memcpy(&(this->event_adc), event, sizeof(struct sigevent));
		//this->event = event;
		event_flags_adc = flags;
		isr_id_adc = 2;
		// Event type check...
		if ((event->sigev_notify != (SIGEV_PULSE))
				&& (event->sigev_notify != (SIGEV_PULSE & 0x0f))) { // magic due to 32-bit system
			cout << "No Pulse Signal given " << endl;
			isr_id_adc = -1;
		}
		result = isr_id_adc;
		irqmasked_adc = false;
	}
	return result;
}

int SimQNXIRQ::simInterruptDetach(int id) {
	int result = -1;
	if (id == isr_id_gpio) {
		isr_id_gpio = -1;
		isr_handler_gpio = nullptr;
		isr_area_gpio = nullptr;
		isr_flags_gpio = 0;
		event_gpio.sigev_notify = 0;
		result = 0;
	}
	if (id == isr_id_adc) {
		isr_id_adc = -1;
		isr_handler_adc = nullptr;
		isr_area_adc = nullptr;
		isr_flags_adc = 0;
		event_adc.sigev_notify = 0;
		result = 0;
	}
	return result;
}

#define MAX_ISR_LOOPS 20

void SimQNXIRQ::operator()() {
	run = true;
	// thread super-loop
	while (run) {
		{
			std::unique_lock<std::mutex> lock(runmutex);
			cv.wait(lock, [this]{return pending;}); // wait on semaphore
			pending = false;
		}
		//cout << "unlocked " << endl;

		// call ISR and Event as both could be set up.
		if (gpio->checkPendingIRQGPIO() && !isGPIOMasked()) {
			raiseEventGPIO();
            if(SIMCONFQUERRY_ISACTIVE(showactions)){
            	cout << "<SIM> raise GPIO Event" << endl;
            }
		}
		int runawaycounter = 0;
		while(gpio->checkPendingIRQGPIO() && !isGPIOMasked() && (runawaycounter++ <MAX_ISR_LOOPS)) {
			raiseISRGPIO();
            if(SIMCONFQUERRY_ISACTIVE(showactions)){
				if(runawaycounter < 2){
					cout << "<SIM> raise GPIO ISR" << endl;
				}
            }
		}
		if(runawaycounter>=MAX_ISR_LOOPS){
			cout << "<SIM> Error ISR locked by pending level IRQ GPIO" << endl;
		}

		if (gpio->checkPendingIRQADC() && !isADCMasked()) {
			raiseEventADC();
            if(SIMCONFQUERRY_ISACTIVE(showactions)){
            	cout << "<SIM> raise ADC Event" << endl;
            }
		}
		runawaycounter = 0;
		while(gpio->checkPendingIRQADC() && !isADCMasked() && (runawaycounter++ <MAX_ISR_LOOPS)) {
			raiseISRADC();
            if(SIMCONFQUERRY_ISACTIVE(showactions)){
				if(runawaycounter < 2){
					cout << "<SIM> raise ADC ISR" << endl;
				}
            }
		}
		if(runawaycounter>=MAX_ISR_LOOPS){
			cout << "<SIM> Error ISR locked by pending level IRQ ADC" << endl;
		}

	}
}

void SimQNXIRQ::unmask_called(){
	irqPending();
}

void SimQNXIRQ::irqPending(){
	{
		std::unique_lock<std::mutex> lock(runmutex);
		pending = true;
		cv.notify_one();
	}
}

// QNX-API simulation
int simInterruptMask(int intr, int id) {
	return SimQNXIRQ::getSimIRQ()->simInterruptMask(intr, id);
}

int simInterruptUnmask(int intr, int id) {
	return SimQNXIRQ::getSimIRQ()->simInterruptUnmask(intr, id);
}

int simInterruptAttach(int intr,
		const struct sigevent * (*handler)(void *, int), const void * area,
		int size, unsigned flags) {
	return SimQNXIRQ::getSimIRQ()->simInterruptAttach(intr, handler, area, size,
			flags);
}

int simInterruptAttachEvent(int intr, const struct sigevent* event,
		unsigned flags) {
	return SimQNXIRQ::getSimIRQ()->simInterruptAttachEvent(intr, event, flags);
}

int simInterruptDetach(int id) {
	return SimQNXIRQ::getSimIRQ()->simInterruptDetach(id);
}
