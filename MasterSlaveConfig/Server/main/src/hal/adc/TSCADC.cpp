/**
 * @file 		TSCADC.cpp
 * @brief
 * @date 		17.02.2016
 * @author: 	T Jaehnichen
 *				HAW-Hamburg
 *          	Labor f�r technische Informatik
 *          	Berliner Tor  7
 *          	D-20099 Hamburg
 * @version 	
 * @details		
 * @copybrief	Based on the StarterWareFree for AM335X provided by Texas Instrument
 *				
 */





#include "TSCADC.h"

#include <hw/inout.h>
#include <sys/mman.h>
#include <sys/neutrino.h>

#include <cstdlib>
#include "tscadc_hw.h"
#include "Util.h"


using namespace std;

TSCADC::TSCADC()
	: 	baseAdd(MAP_DEVICE_FAILED){
	gainAccess();
}

TSCADC::~TSCADC() {
	munmap_device_io(baseAdd, SIZE);
}


void TSCADC::gainAccess(void){
	if(baseAdd ==  MAP_DEVICE_FAILED){
		if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
			DBG_ERROR("ThreadCtl access failed\n");
			exit(EXIT_FAILURE);
		}
		baseAdd = mmap_device_io(SIZE, BASE);
		if(baseAdd ==  MAP_DEVICE_FAILED){
			DBG_ERROR("allocation of GPIO failed");
			exit(EXIT_FAILURE);
		}
	}
}


/** !IMPORTANT: These functions is not in the sources!! */
/*==========*/
void TSCADC::clearBitsInReg(uintptr_t addr,uint32_t value){
	uint32_t prevVal = in32((uintptr_t ) addr);
	out32((uintptr_t ) addr, prevVal & (~value));
}

void TSCADC::setBitsInReg(uintptr_t addr, uint32_t value){
	uint32_t prevVal = in32((uintptr_t ) addr);
	out32((uintptr_t ) addr, prevVal | value);
}

/*==========*/


/**
 * @brief   This API gets the revision information of the Touch Screen Module.
 *
 * @return  returns the revision ID.
 *
 **/
unsigned int TSCADC::getRevision() {
	return in32(baseAdd + REVISION );
}

/**
 * @brief   This API enables the protection of Step Config Registers.
 *
 * @return  None
 *
 **/
void TSCADC::stepConfigProtectionEnable() {
    clearBitsInReg(baseAdd + CTRL, CTRL_STEPCONFIG_WRITEPROTECT_N);
}

/**
 * @brief   This API disables the protection of Step Config Registers.
 *
 * @param   baseAdd    Base Address of the TouchScreen Module Registers.
 *
 * @return  None
 *
 **/
void TSCADC::stepConfigProtectionDisable() {
	setBitsInReg(baseAdd + CTRL, CTRL_STEPCONFIG_WRITEPROTECT_N);
}

/**
 * @brief   This API configures the Touch Screen for 4 Wire/5 Wire mode
 *
 * @param   tsMode     Selects the TS mode to be configured.\n
 *
 *          tsMode can take following macros.\n
 *
 *          FOUR_WIRE_MODE.\n
 *          FIVE_WIRE_MODE.\n
 *          GENERAL_PURPOSE_MODE.\n
 *
 * @return  None
 *
 **/
void TSCADC::tsModeConfig(TSMode tsMode) {
	clearBitsInReg(baseAdd + CTRL, CTRL_AFE_PEN_CTRL);
    setBitsInReg(baseAdd + CTRL, tsMode << CTRL_AFE_PEN_CTRL_SHIFT);
}

/**
 * @brief   This API sets the idle mode of TSCADC Module.
 *
 * @param   idleMode   idle mode to set.\n
 *
 *          idle mode can take following macros.\n
 *
 * 		       0 - FORCE_IDLE
 * 		       1 - NO_IDLE
 * 		       2 - SMART_IDLE
 * 		       3 - SMART_IDLE_WAKEUP
 *
 * @return  None
 *
 **/
void TSCADC::idleModeSet(IdleMode idleMode) {
	 clearBitsInReg(baseAdd + SYSCONFIG, SYSCONFIG_IDLEMODE);
	 setBitsInReg(baseAdd + SYSCONFIG, idleMode << SYSCONFIG_IDLEMODE_SHIFT);
}

/**
 * @brief  This API Configures the Step Config Mode
 *
 * @param   stepSel    Step to be configured
 * @param   mode       Selects the Step Mode.
 *
 *          mode can take following macros.\n
 *
 *          ONE_SHOT_SOFTWARE_ENABLED.\n
 *          CONTINIOUS_SOFTWARE_ENABLED.\n
 *          ONE_SHOT_HARDWARE_SYNC.\n
 *          CONTINIOUS_HARDWARE_SYNC.\n
 *
 * @return  none
 *
 **/
void TSCADC::tsStepModeConfig(unsigned int stepSel, StepMode mode) {
	clearBitsInReg(baseAdd + STEPCONFIG(stepSel), STEPCONFIG_MODE);
	setBitsInReg(baseAdd + STEPCONFIG(stepSel), mode << STEPCONFIG_MODE_SHIFT);
}

/**
 * @brief   This API clears the status of the specified interrupts.
 *
 * @param   intFlag   It determines which interrupts status need to be
 *                    cleared.\n
 *
 *          intFlag can take following macro values.\n
 *
 *          ASYNC_HW_PEN_EVENT_INT.\n
 *          SYNC_PEN_EVENT_INT.\n
 *          FIFO0_UNDER_FLOW_INT.\n
 *          FIFO1_UNDER_FLOW_INT.\n
 *          END_OF_SEQUENCE_INT.\n
 *          FIFO0_THRESHOLD_INT.\n
 *          FIFO1_THRESHOLD_INT.\n
 *          FIFO0_OVER_RUN_INT.\n
 *          FIFO1_OVER_RUN_INT.\n
 *          OUT_OF_RANGE_INT.\n
 *          PEN_UP_EVENT_INT.\n
 *
 * @return  None
 *
 **/
void TSCADC::intStatusClear(unsigned int intFlag) {
	out32(baseAdd + IRQSTATUS, intFlag & 0x7FF);
}

/**
 * @brief   This API enables the interrupt for the given event.
 *
 * @param   event      event for which the interrupt to be enabled.
 *
 *          event can take following macro values.\n
 *
 *          ASYNC_HW_PEN_EVENT_INT.\n
 *          SYNC_PEN_EVENT_INT.\n
 *          FIFO0_UNDER_FLOW_INT.\n
 *          FIFO1_UNDER_FLOW_INT.\n
 *          END_OF_SEQUENCE_INT.\n
 *          FIFO0_THRESHOLD_INT.\n
 *          FIFO1_THRESHOLD_INT.\n
 *          FIFO0_OVER_RUN_INT.\n
 *          FIFO1_OVER_RUN_INT.\n
 *          OUT_OF_RANGE_INT.\n
 *          PEN_UP_EVENT_INT.\n
 *
 * @return  None
 *
 **/
void TSCADC::eventInterruptEnable(IntFlag event) {
	setBitsInReg(baseAdd + IRQENABLE_SET, event);
}

/**
 * @brief   This API disables the interrupt for the given event.
 *
 * @param   event      Event for which the interrupt to be disabled.
 *
 *          event can take following macro values.\n
 *
 *          ASYNC_HW_PEN_EVENT_INT.\n
 *          SYNC_PEN_EVENT_INT.\n
 *          FIFO0_UNDER_FLOW_INT.\n
 *          FIFO1_UNDER_FLOW_INT.\n
 *          END_OF_SEQUENCE_INT.\n
 *          FIFO0_THRESHOLD_INT.\n
 *          FIFO1_THRESHOLD_INT.\n
 *          FIFO0_OVER_RUN_INT.\n
 *          FIFO1_OVER_RUN_INT.\n
 *          OUT_OF_RANGE_INT.\n
 *          PEN_UP_EVENT_INT.\n
 *
 * @return  None
 *
 **/
void TSCADC::eventInterruptDisable(IntFlag event) {
	 out32(baseAdd + IRQENABLE_CLR, event);
}

/**
 * @brief  This API selects the FIFO to store the ADC data
 *
 * @param   stepSel    Step to be configured
 * @param   FIFOSel    Selects the FIFO.\n
 *
 *          FIFOSel can take following macros.\n
 *
 *          TSCADC_FIFO_0.\n
 *          TSCADC_FIFO_1.\n
 *
 * @return  none
 *
 **/
void TSCADC::tsStepFIFOSelConfig(unsigned int stepSel, Fifo FIFOSel) {
	clearBitsInReg(baseAdd + STEPCONFIG(stepSel), STEPCONFIG_FIFO_SELECT);
	setBitsInReg(baseAdd + STEPCONFIG(stepSel), FIFOSel << STEPCONFIG_FIFO_SELECT_SHIFT);
}

/**
 * @brief  This API Configures the number of samples to average
 *
 * @param   stepSel    Step to be configured
 * @param   average    Number of Samples to average.\n
 *
 *          average can take following values.\n
 *
 *          TWO_SAMPLES_AVG.\n
 *          FOUR_SAMPLES_AVG.\n
 *          EIGHT_SAMPLES_AVG.\n
 *          SIXTEEN_SAMPLES_AVG.\n
 *
 * @return  none
 *
 **/
void TSCADC::tsStepAverageConfig(unsigned int stepSel, AverageSamples average) {
	clearBitsInReg(baseAdd + STEPCONFIG(stepSel), STEPCONFIG_AVERAGING);
	    setBitsInReg(baseAdd + STEPCONFIG(stepSel), average <<  STEPCONFIG_AVERAGING_SHIFT);
}

/**
	 * @brief  This API configures the reference voltage and input for
	 *         given Step (other than Idle and Touch Screen charge Step)
	 *
	 * @param   stepSelect	 	Step to be configured.
	 *
	 * @param   adcNegativeRef	Selects the ADC Negative Reference Voltage.\n
	 *
	 *          adcNegativeRef can take following values.\n
	 *
	 *           NEGATIVE_REF_VSSA.\n
	 *           NEGATIVE_REF_XNUR.\n
	 *           NEGATIVE_REF_YNLR.\n
	 *           NEGATIVE_REF_ADCREFM.\n
	 *
	 * @param   adcPositiveInput 	Selects the Positive Analog Input Source.\n
	 *
	 *           adcPositiveInput can take following values.\n
	 *
	 *           POSITIVE_INP_CHANNEL1.\n
	 *           POSITIVE_INP_CHANNEL2.\n
	 *           POSITIVE_INP_CHANNEL3.\n
	 *           POSITIVE_INP_CHANNEL4.\n
	 *           POSITIVE_INP_CHANNEL5.\n
	 *           POSITIVE_INP_CHANNEL6.\n
	 *           POSITIVE_INP_CHANNEL7.\n
	 *           POSITIVE_INP_CHANNEL8.\n
	 *           POSITIVE_INP_ADCREFM.\n
	 *
	 * @param   adcNegativeInput 	Selects the Negative Analog Input Source.\n
	 *
	 *          adcNegativeInput can take following values.\n
	 *
	 *          NEGATIVE_INP_CHANNEL1.\n
	 *          NEGATIVE_INP_CHANNEL2.\n
	 *          NEGATIVE_INP_CHANNEL3.\n
	 *          NEGATIVE_INP_CHANNEL4.\n
	 *          NEGATIVE_INP_CHANNEL5.\n
	 *          NEGATIVE_INP_CHANNEL6.\n
	 *          NEGATIVE_INP_CHANNEL8.\n
	 *          NEGATIVE_INP_ADCREFM.\n
	 *
	 * @param   adcPositiveRef 	Selects the ADC Positive Reference Voltage.\n
	 *
	 *          adcPositiveRef can take following values.\n
	 *
	 *          POSITIVE_REF_VDDA.\n
	 *          TSCADC_POSITIVE_REF_XNUR.\n
	 *          TSCADC_POSITIVE_REF_YNLR.\n
	 *          POSITIVE_REF_ADCREFP.\n
	 *
	 * @return  none
	 *
	 * Note: stepSelect can take any integer value b/w 0 to 15
	 *
	 **/
void TSCADC::tsStepConfig(unsigned int stepSelect, NegativeRef adcNegativeRef, PositiveInput adcPositiveInp, NegativeInput adcNegativeInp, PositiveRef adcPositiveRef) {
	clearBitsInReg(baseAdd + STEPCONFIG(stepSelect), STEPCONFIG_SEL_RFM_SWC);
	setBitsInReg(baseAdd + STEPCONFIG(stepSelect), adcNegativeRef << STEPCONFIG_SEL_RFM_SWC_SHIFT);

	clearBitsInReg(baseAdd + STEPCONFIG(stepSelect), STEPCONFIG_SEL_INP_SWC);
	setBitsInReg(baseAdd + STEPCONFIG(stepSelect), adcPositiveInp << STEPCONFIG_SEL_INP_SWC_SHIFT);

	clearBitsInReg(baseAdd + STEPCONFIG(stepSelect), STEPCONFIG_SEL_INM_SWM);
	setBitsInReg(baseAdd + STEPCONFIG(stepSelect), adcNegativeInp << STEPCONFIG_SEL_INM_SWM_SHIFT);

	clearBitsInReg(baseAdd + STEPCONFIG(stepSelect), STEPCONFIG_SEL_RFP_SWC);
	setBitsInReg(baseAdd + STEPCONFIG(stepSelect), adcPositiveRef << STEPCONFIG_SEL_RFP_SWC_SHIFT);
}

/**
 * @brief   This API sets the AFE clock

 * @param   clkDiv     Divider Value.
 * @param	inputClk   Vlaue of the input clk.
 *
 * @return  none
 *
 **/
void TSCADC::configureAFEClock(unsigned int moduleClk, unsigned int inputClk) {
	unsigned int clkDiv = 0;

	clkDiv = moduleClk / inputClk;
	clearBitsInReg(baseAdd + ADC_CLKDIV, ADC_CLKDIV_ADC_CLK_DIV);
	out32(baseAdd + ADC_CLKDIV, clkDiv - 1);
}

/**
 * @brief   This API configures ADC to single ended or differential operation
 *          mode.
 *
 * @param   mode       It is the value which determines whether to
 *                     configure ADC to single ended or differential
 *                     operation mode.
 *
 *          mode can take following macros.
 *
 *          SINGLE_ENDED_OPER_MODE.\n
 *          DIFFERENTIAL_OPER_MODE.\n
 *
 *  @param  stepSelect   It is the value which determines which step config
 *                       register to be configured.\n
 *
 *
 *          stepSelect can take any value between 0 to 15.\n
 *
 *
 * @return  none
 *
 **/
void TSCADC::tsStepOperationModeControl(OperationMode mode, unsigned int stepSelect) {
    if(mode == DIFFERENTIAL_OPER_MODE)
    {
         setBitsInReg(baseAdd + STEPCONFIG(stepSelect), STEPCONFIG_DIFF_CNTRL);
    }
    else
    {
         clearBitsInReg(baseAdd + STEPCONFIG(stepSelect), STEPCONFIG_DIFF_CNTRL);
    }
}

/**
 * @brief   This API configure the TouchScreen Step Transistor Biasing for
 *          xnpsw,xppsw and yppsw pins through which analog supply
 *          is provided to the touch screen.
 *
 * @param   stepSelect	Step to be configured
 * @param   xppsw 	XPPSW pin SW configuration. True means on.\n
 * @param   xnpsw 	XNPSW pin SW configuration. True means on.\n
 * @param   yppsw 	YPPSW pin SW configuration. True means on.\n
 *
 * @return  none
 *
 * Note: stepSelect can take any integer value b/w 0 to 15
 **/
void TSCADC::tsStepAnalogSupplyConfig(bool xppsw, bool xnpsw, bool yppsw, unsigned int stepSelect) {
	clearBitsInReg(baseAdd + STEPCONFIG(stepSelect), STEPCONFIG_XPPSW_SWC);
	clearBitsInReg(baseAdd + STEPCONFIG(stepSelect), STEPCONFIG_XNPSW_SWC);
	clearBitsInReg(baseAdd + STEPCONFIG(stepSelect), STEPCONFIG_YPPSW_SWC);
	if(xppsw){
		setBitsInReg(baseAdd + STEPCONFIG(stepSelect), 1 << STEPCONFIG_XPPSW_SWC_SHIFT);
	}
	if(xnpsw){
		setBitsInReg(baseAdd + STEPCONFIG(stepSelect), 1 << STEPCONFIG_XNPSW_SWC_SHIFT);
	}
	if(yppsw){
		setBitsInReg(baseAdd + STEPCONFIG(stepSelect), 1 << STEPCONFIG_YPPSW_SWC_SHIFT);
	}
}


/**
 * @brief   This API configure the TouchScreen Step Transistor Biasing for
 *          xnnsw, ypnsw, ynnsw and wpnsw pins through which analog ground
 *          is provided to the touch screen.
 *
 * @param   stepSelect	Step to be configured.
 * @param   xnnsw 	XNNSW pin SW configuration. True means on.\n
 * @param   ypnsw 	YPNSW pin SW configuration. True means on.\n
 * @param   ynnsw 	YNNSW pin SW configuration. True means on.\n
 * @param   wpnsw 	WPNSW pin SW configuration. True means on.\n
 *
 * @return  none
 *
 * Note: stepSelect can take any integer value b/w 0 to 15
 **/
void TSCADC::tsStepAnalogGroundConfig(bool xnnsw, bool ypnsw, bool ynnsw, bool wpnsw, unsigned int stepSelect) {
	clearBitsInReg(baseAdd + STEPCONFIG(stepSelect), STEPCONFIG_XNNSW_SWC);
	clearBitsInReg(baseAdd + STEPCONFIG(stepSelect), STEPCONFIG_YPNSW_SWC);
	clearBitsInReg(baseAdd + STEPCONFIG(stepSelect), STEPCONFIG_YNNSW_SWC);
	clearBitsInReg(baseAdd + STEPCONFIG(stepSelect), STEPCONFIG_WPNSW_SWC);

	if(xnnsw){
		setBitsInReg(baseAdd + STEPCONFIG(stepSelect), 1 << STEPCONFIG_XNNSW_SWC_SHIFT);
	}
	if(ypnsw){
		setBitsInReg(baseAdd + STEPCONFIG(stepSelect), 1 << STEPCONFIG_YPNSW_SWC_SHIFT);
	}
	if(ynnsw){
		setBitsInReg(baseAdd + STEPCONFIG(stepSelect), 1 << STEPCONFIG_YNNSW_SWC_SHIFT);
	}
	if(wpnsw){
		setBitsInReg(baseAdd + STEPCONFIG(stepSelect), 1 << STEPCONFIG_WPNSW_SWC_SHIFT);
	}

}

/**
 * @brief   This API Enables/Disables the given step.

 * @param   stepEn_Dis   Enables/Disables the Step.\n
 *
 *          false - Disables the step.\n
 *          true - Enables the step.\n
 *
 * @param   stepSel      Select the step.\n
 *
 *          stepSel can take any value bteween 1 to 16.\n
 *
 * @return  none
 *
 **/
void TSCADC::configureStepEnable(unsigned int stepSel, bool stepEn_Dis) {
	clearBitsInReg(baseAdd + STEPENABLE, 1 << stepSel);
	setBitsInReg(baseAdd + STEPENABLE, stepEn_Dis << stepSel);
}

/**
 * @brief   This API Enables/Disables the TSCADC Module
 *
 * @param   enableModule   Whether the module is to be enabled.
 *
 *          enableModule can take following values.\n
 *
 *          false - Disables  module.\n
 *          true  - Enables the module.\n
 *
 * @return  None
 *
 **/
void TSCADC::moduleStateSet(bool enableModule) {
		if(enableModule){
			setBitsInReg(baseAdd + CTRL, CTRL_ENABLE);
		}else{
			clearBitsInReg(baseAdd + CTRL, CTRL_ENABLE);
		}
}

/**
 * @brief   This API enables/disables the channel ID store along with the
 * 	     captured ADC data
 *
 * @param   enableStepIDTag   Whether or not step ID is to be stored.
 *
 * @return  None
 *
 **/
void TSCADC::stepIDTagConfig(bool enableStepIDTag) {
	if(enableStepIDTag){
		setBitsInReg(baseAdd + CTRL, CTRL_STERP_ID_TAG);
	}else{
		clearBitsInReg(baseAdd + CTRL, CTRL_STERP_ID_TAG);
	}
}

/**
 * @brief   This API gets the ADC Sampled data
 *
 * @param   FIFOSel    Selects the FIFO.\n
 *
 * @return  returns the channel ID
 *
 **/
unsigned int TSCADC::fifoADCDataRead(Fifo FIFOSel) {
	return (in32(baseAdd + FIFODATA(FIFOSel)) & FIFODATA_ADC_DATA);
}

/**
 * @brief   This API Enables/Disables the Touch Screen Transistors
 *
 * @param   enableTSTransistor   whether the Touch Screen Transistors are to be
 * 			         Enabled.\n
 *
 *          enableTSTransistor can take following values.\n
 *
 *          false - Disables the transistor.\n
 *          true  - Enables the transistor.\n
 *
 * @return  None
 *
 **/
void TSCADC::tsTransistorConfig(bool enableTSTransistor) {
	 if(enableTSTransistor){
		setBitsInReg(baseAdd + CTRL, CTRL_TOUCH_SCREEN_ENABLE);
	}else{
		clearBitsInReg(baseAdd + CTRL, CTRL_TOUCH_SCREEN_ENABLE);
	}
}

/**
 * @brief  This API sets the FIFO threshold Level to generate interrupt request.
 *
 * @param   FIFOSel           It is the value which determines the whether to
 *                            to configure the threshold level for FIFO0 or
 *                            FIFO1.\n
 *
 *          FIFOSel can take following macros.\n
 *
 *          TSCADC_FIFO_0.\n
 *          TSCADC_FIFO_1.\n
 *
 * @param   numberOfSamples   It is the threshold level to be configured.
 *
 * @return  none
 *
 **/
void TSCADC::fifoIRQThresholdLevelConfig(Fifo FIFOSel, unsigned char numberOfSamples) {
	out32(baseAdd + FIFOTHRESHOLD(FIFOSel), numberOfSamples - 1);
}

/**
 * @brief   This API returns the interrupt status.
 *
 * @return  The status of the interrupts.
 *
 **/
unsigned int TSCADC::intStatus() {
	return (in32(baseAdd + IRQSTATUS));
}

