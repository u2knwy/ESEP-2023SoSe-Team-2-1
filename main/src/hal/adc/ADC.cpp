/**
 * @file 		ADC.cpp
 * @brief		This module controls and configures the ADC of the BeagleboneBlack
 * @date 		03.11.2017
 * @author: 	T Jaehnichen
 *				HAW-Hamburg
 *          	Labor f�r technische Informatik
 *          	Berliner Tor  7
 *          	D-20099 Hamburg
 * @version 	1
 * @details		
 * @copybrief	Based on the StarterWareFree for AM335X provided by Texas Instrument
 *				
 */

#include "ADC.h"

#include <sys/neutrino.h>
#include <stdlib.h>
#include "Util.h"

#define ADC_FIFO0_VALUE 		6
#define ADC_TSC_GENINT 	 		16

const struct sigevent* ADC::adcISR(void* arg, int id){
	ADC* adc = (ADC*)arg;
	unsigned int status = adc->tscadc->intStatus();
	adc->tscadc->intStatusClear(status);
	if(status & END_OF_SEQUENCE_INT){
		adc->event.sigev_value.sival_int = adc->tscadc->fifoADCDataRead(Fifo::FIFO_0);
	}

	return &(adc->event);
}

ADC::ADC(TSCADC& tscadc)
	:	tscadc(&tscadc)
{
	if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
		DBG_ERROR("ThreadCtl access failed\n");
		exit(EXIT_FAILURE);
	}
	init();
}

ADC::~ADC() {
	// This should call disable method...
}

void ADC::registerAdcISR(int connectionID, char msgType){
//	__asm(" dsb");

	/* Clear the status of all interrupts */
	cleanUpInterrupts();

	SIGEV_PULSE_INIT(&event, connectionID, SIGEV_PULSE_PRIO_INHERIT, (int)msgType, 0);

	interruptID = InterruptAttach(ADC_TSC_GENINT, adcISR, this, sizeof(ADC), 0);
	if(interruptID == -1){
		DBG_ERROR("InterruptAttach failed");
		exit(EXIT_FAILURE);
	}

	/* Clear the status of all interrupts */
	cleanUpInterrupts();

	/* End of sequence interrupt is enable */
	tscadc->eventInterruptEnable(END_OF_SEQUENCE_INT);
}

void ADC::unregisterAdcISR(void){
	if( InterruptDetach(interruptID) < 0){
		DBG_ERROR("could not detach adc interrupt handler");
		exit(EXIT_FAILURE);
	}
	tscadc->eventInterruptDisable(END_OF_SEQUENCE_INT);

	cleanUpInterrupts();
}

void ADC::init(void) {
	/* Configures ADC to 3Mhz */
	tscadc->configureAFEClock(24000000, 3000000);

	/* Disable Transistor bias */
	tscadc->tsTransistorConfig(false);

	tscadc->stepIDTagConfig(true);

	/* Disable Write Protection of Step Configuration register*/
	tscadc->stepConfigProtectionDisable();

	/* Configure step 1 for channel 1(AN0)*/
	stepConfigure(0, Fifo::FIFO_0, POSITIVE_INP_CHANNEL1);

	/* General purpose inputs */
	tscadc->tsModeConfig(GENERAL_PURPOSE_MODE);
	tscadc->fifoIRQThresholdLevelConfig(Fifo::FIFO_0, 1);

	/* Clear the status of all interrupts */
	cleanUpInterrupts();

	/* End of sequence interrupt is enable */
	tscadc->eventInterruptEnable(END_OF_SEQUENCE_INT);

}

void ADC::stepConfigure(unsigned int stepSel, Fifo fifo,
		PositiveInput positiveInpChannel) {
    /* Configure ADC to Single ended operation mode */
    tscadc->tsStepOperationModeControl(SINGLE_ENDED_OPER_MODE, stepSel);

    /* Configure step to select Channel, refernce voltages */
    tscadc->tsStepConfig(stepSel, NEGATIVE_REF_VSSA,
                    positiveInpChannel, NEGATIVE_INP_CHANNEL1, POSITIVE_REF_VDDA);

    /* XPPSW Pin is on, Which pull up the AN0 line*/
    tscadc->tsStepAnalogSupplyConfig(true, false, false, stepSel);

    /* XNNSW Pin is on, Which pull down the AN1 line*/
    tscadc->tsStepAnalogGroundConfig(true, false, false, false, stepSel);

    /* select fifo 0 or 1*/
    tscadc->tsStepFIFOSelConfig(stepSel, fifo);

    /* Configure ADC to one shot mode */
    tscadc->tsStepModeConfig(stepSel,  ONE_SHOT_SOFTWARE_ENABLED);

    tscadc->tsStepAverageConfig(stepSel, NO_SAMPLES_AVG);
}

/**
 * @brief	This function enables a single sampling.
 *
 * @return 	None
 */
void ADC::sample(void){
	adcEnableSequence(1);
}

/**
 * @brief	This function enables the first given number of steps.
 *
 * @param 	steps	Number (between 1 and 16) of steps which shall be enabled.
 *
 * @return 	None
 */
void ADC::adcEnableSequence(unsigned int steps) {
	tscadc->moduleStateSet(false);
	int maxSteps = steps<16?steps:16;
	for(int i=1; i<=maxSteps; i++){
		tscadc->configureStepEnable(i, true);
	}
	tscadc->moduleStateSet(true);
}

// this has wrong semantic in comparison with adcEnable
void ADC::adcDisable(void){
	tscadc->moduleStateSet(false);
}

void ADC::cleanUpInterrupts(void){
	tscadc->intStatusClear(0x7FF);
	tscadc->intStatusClear(0x7FF);
	tscadc->intStatusClear(0x7FF);
}
