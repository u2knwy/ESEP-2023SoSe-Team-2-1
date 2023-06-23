/**
 * @file 		TSCADC.h
 * @brief		This file contains the function prototypes for the
 *device abstraction layer for Touch Screen. It also contains some related macro
 *definitions and some files to be included.
 * @date 		17.02.2016
 * @author: 	T Jaehnichen
 *				HAW-Hamburg
 *          	Labor f�r technische Informatik
 *          	Berliner Tor  7
 *          	D-20099 Hamburg
 * @version
 * @details
 * @copybrief	Based on the StarterWareFree for AM335X provided by Texas
 *Instrument (copyright notice see below)
 *
 */

#pragma once

#include "tscadc_hw.h"
#include <cstdint>


enum IdleMode : unsigned int {
    FORCE_IDLE = 0,
    NO_IDLE = 1,
    SMART_IDLE = 2,
    SMART_IDLE_WAKEUP = 3
};

enum IntFlag : unsigned int {
    ASYNC_HW_PEN_EVENT_INT = IRQSTATUS_RAW_HW_PEN_EVENT,
    SYNC_PEN_EVENT_INT = IRQSTATUS_RAW_PEN_IRQ,
    FIFO0_UNDER_FLOW_INT = IRQSTATUS_RAW_FIFO0_UNDERFLOW,
    FIFO1_UNDER_FLOW_INT = IRQSTATUS_RAW_FIFO1_UNDERFLOW,
    END_OF_SEQUENCE_INT = IRQSTATUS_RAW_END_OF_SEQUENCE,
    FIFO0_THRESHOLD_INT = IRQSTATUS_RAW_FIFO0_THRESHOLD,
    FIFO1_THRESHOLD_INT = IRQSTATUS_RAW_FIFO1_THRESHOLD,
    FIFO0_OVER_RUN_INT = IRQSTATUS_RAW_FIFO0_OVERRUN,
    FIFO1_OVER_RUN_INT = IRQSTATUS_RAW_FIFO1_OVERRUN,
    OUT_OF_RANGE_INT = IRQSTATUS_RAW_OUT_OF_RANGE,
    PEN_UP_EVENT_INT = IRQSTATUS_RAW_PEN_UP_EVENT
};

enum TSMode : unsigned int {
    FOUR_WIRE_MODE = 1,
    FIVE_WIRE_MODE = 2,
    GENERAL_PURPOSE_MODE = 0
};

enum PenIRQ : unsigned int { PEN_IRQ_0 = 0, PEN_IRQ_1 = 1 };

enum OperationMode : unsigned int {
    SINGLE_ENDED_OPER_MODE = 0,
    DIFFERENTIAL_OPER_MODE = 1
};

enum AverageSamples : unsigned int {
    NO_SAMPLES_AVG = 0,
    TWO_SAMPLES_AVG = 1,
    FOUR_SAMPLES_AVG = 2,
    EIGHT_SAMPLES_AVG = 3,
    SIXTEEN_SAMPLES_AVG = 4
};

enum StepMode : unsigned int {
    ONE_SHOT_SOFTWARE_ENABLED = 0,
    CONTINIOUS_SOFTWARE_ENABLED = 1,
    ONE_SHOT_HARDWARE_SYNC = 2,
    CONTINIOUS_HARDWARE_SYNC = 3
};

enum Wakeup : unsigned int { WAKEUP_DISABLE = 0, WAKEUO_ENABLE = 1 };

enum HWEvent : unsigned int { HW_INPUT_EVENT = 1, PEN_TOUCH = 0 };

enum ACBias : unsigned int { INTERNAL_AC_BIAS = 0, EXTERNAL_AC_BIAS = 1 };

enum NegativeRef : unsigned int {
    NEGATIVE_REF_VSSA = 0,
    NEGATIVE_REF_XNUR = 1,
    NEGATIVE_REF_YNLR = 2,
    NEGATIVE_REF_ADCREFM = 3
};

enum PositiveRef : unsigned int {
    POSITIVE_REF_VDDA = 0,
    POSITIVE_REF_XPUL = 1,
    POSITIVE_REF_YPLL = 2,
    POSITIVE_REF_ADCREFP = 3
};

enum PositiveInput : unsigned int {
    POSITIVE_INP_CHANNEL1 = 0,
    POSITIVE_INP_CHANNEL2 = 1,
    POSITIVE_INP_CHANNEL3 = 2,
    POSITIVE_INP_CHANNEL4 = 3,
    POSITIVE_INP_CHANNEL5 = 4,
    POSITIVE_INP_CHANNEL6 = 5,
    POSITIVE_INP_CHANNEL7 = 6,
    POSITIVE_INP_CHANNEL8 = 7,
    POSITIVE_INP_ADCREFM = 8
};

enum NegativeInput : unsigned int {
    NEGATIVE_INP_CHANNEL1 = 0,
    NEGATIVE_INP_CHANNEL2 = 1,
    NEGATIVE_INP_CHANNEL3 = 2,
    NEGATIVE_INP_CHANNEL4 = 3,
    NEGATIVE_INP_CHANNEL5 = 4,
    NEGATIVE_INP_CHANNEL6 = 5,
    NEGATIVE_INP_CHANNEL7 = 6,
    NEGATIVE_INP_CHANNEL8 = 7,
    NEGATIVE_INP_ADCREFM = 8
};

class TSCADC {
  public:
    TSCADC();
    virtual ~TSCADC();

    void gainAccess();

    void setBitsInReg(uintptr_t addr, uint32_t value);
    void clearBitsInReg(uintptr_t addr, uint32_t value);

    /*************************************************************************
                    API FUNCTION DEFINITIONS
    *************************************************************************/

    unsigned int getRevision();
    void stepConfigProtectionEnable();
    void stepConfigProtectionDisable();
    void tsModeConfig(TSMode tsMode);
    void idleModeSet(IdleMode idleMode);
    void tsStepModeConfig(unsigned int stepSel, StepMode mode);
    void intStatusClear(unsigned int intFlag);
    void eventInterruptEnable(IntFlag event);
    void eventInterruptDisable(IntFlag event);
    void tsStepFIFOSelConfig(unsigned int stepSel, Fifo FIFOSel);
    void tsStepAverageConfig(unsigned int stepSel, AverageSamples average);
    void tsStepConfig(unsigned int stepSelect, NegativeRef adcNegativeRef,
                      PositiveInput adcPositiveInp,
                      NegativeInput adcNegativeInp, PositiveRef adcPositiveRef);
    void configureAFEClock(unsigned int moduleClk, unsigned int inputClk);
    void tsStepOperationModeControl(OperationMode mode,
                                    unsigned int stepSelect);
    void tsStepAnalogSupplyConfig(bool xppsw, bool xnpsw, bool yppsw,
                                  unsigned int stepSelect);
    void tsStepAnalogGroundConfig(bool xnnsw, bool ypnsw, bool ynnsw,
                                  bool wpnsw, unsigned int stepSelect);
    void configureStepEnable(unsigned int stepSel, bool stepEn_Dis);
    void moduleStateSet(bool enableModule);
    void stepIDTagConfig(bool enableStepIDTag);
    unsigned int fifoADCDataRead(Fifo FIFOSel);
    void tsTransistorConfig(bool enableTSTransistor);
    void fifoIRQThresholdLevelConfig(Fifo FIFOSel,
                                     unsigned char numberOfSamples);
    unsigned int intStatus();

  private:
    unsigned int baseAdd;
};
