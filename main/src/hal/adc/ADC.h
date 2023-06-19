/**
 * @file 		ADC.h
 * @brief		This module controls and configures the ADC of the
 *BeagleboneBlack
 * @date 		03.11.2017
 * @author: 	T Jaehnichen
 *				HAW-Hamburg
 *          	Labor f�r technische Informatik
 *          	Berliner Tor  7
 *          	D-20099 Hamburg
 * @author:     T Lehmann
 * @version 	2
 * @details
 * @copybrief	Based on the StarterWareFree for AM335X provided by Texas
 *Instrument
 *
 */

#pragma once

#include "TSCADC.h"
#include <sys/siginfo.h>


#define ADC_TYPE 16

class ADC {
  public:
    ADC(void) = delete;
    ADC(TSCADC &tscadc);
    virtual ~ADC();

    void registerAdcISR(int connectionID, char msgType);
    void unregisterAdcISR(void);
    void sample(void);
    void adcDisable(void);

  private:
    static const struct sigevent *adcISR(void *arg, int id);

    void init(void);
    void initInterrupts(void);
    void stepConfigure(unsigned int stepSel, Fifo fifo,
                       PositiveInput positiveInpChannel);
    void cleanUpInterrupts(void);
    void adcEnableSequence(unsigned int steps);

  private:
    TSCADC *tscadc;
    struct sigevent event;

    int interruptID;
};
