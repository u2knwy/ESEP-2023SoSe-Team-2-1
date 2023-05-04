/*
 * simgpio.h
 *
 *  Created on: 09.04.2020
 *      Author: Lehmann
 */

#ifndef SRC_SIMGPIO_H_
#define SRC_SIMGPIO_H_

#include "stdint.h"
#include "isimulationimage.h"
#include "isimulationcycleendhandler.h"
#include "simioimage.h"
#include "simqnxirq.h"
#include <mutex>

static constexpr uint32_t SIMGPIO_BASE_BANK0 = 0x44E07000;
static constexpr uint32_t SIMGPIO_BASE_BANK1 = 0x4804C000;
static constexpr uint32_t SIMGPIO_BASE_BANK2 = 0x481AC000;
static constexpr uint32_t SIMGPIO_OE = 0x134;
static constexpr uint32_t SIMGPIO_DATAIN = 0x138;
static constexpr uint32_t SIMGPIO_SETDATAOUT = 0x194;
static constexpr uint32_t SIMGPIO_CLEARDATAOUT = 0x190;
static constexpr uint32_t SIMGPIO_BANK0_MASK = 0x0CD0C0BC;
static constexpr uint32_t SIMGPIO_BANK1_MASK = 0x000FF000;
static constexpr uint32_t SIMGPIO_BANK2_MASK = 0x0000003C;

static constexpr uint32_t SIMGPIO_IRQSTATUS_0 = 0x002C;
static constexpr uint32_t SIMGPIO_IRQSTATUS_1 = 0x0030;
static constexpr uint32_t SIMGPIO_IRQSTATUS_SET_0 = 0x0034;
static constexpr uint32_t SIMGPIO_IRQSTATUS_SET_1 = 0x0038;
static constexpr uint32_t SIMGPIO_IRQSTATUS_CLR_0 = 0x003C;//
static constexpr uint32_t SIMGPIO_IRQSTATUS_CLR_1 = 0x0040;  //
static constexpr uint32_t SIMGPIO_LEVELDETECT0 = 0x0140;
static constexpr uint32_t SIMGPIO_LEVELDETECT1 = 0x0144;
static constexpr uint32_t SIMGPIO_RAISINGDETECT = 0x0148;
static constexpr uint32_t SIMGPIO_FALLINGDETECT = 0x14C;
static constexpr uint32_t SIMGPIO_DEBOUNCENABLE = 0x150;
static constexpr uint32_t SIMGPIO_DEBOUNCINGTIME = 0x154;

static constexpr uint32_t SIMADC_BASE = 0x44E0D000;
static constexpr uint32_t SIMADC_IRQ_ENABLE_SET= 0x2c;
static constexpr uint32_t SIMADC_IRQ_ENABLE_CLR= 0x30;
static constexpr uint32_t SIMADC_IRQ_STATUS= 0x28;
static constexpr uint32_t SIMADC_CTRL = 0x40;
static constexpr uint32_t SIMADC_DATA = 0x100;

class SimQNXGPIO : public ISimulationCycleEndHandler {
private:
	ISimulationImageAccess *simulation = nullptr;
	SimQNXIRQ *irqhandler = nullptr;
	unsigned int bank0_OE = 0xFFFFFFFF;   // all pin are input, spruhl73l, p4893
	unsigned int bank1_OE = 0xFFF00FFF;   // pin are already set to output by system.
	unsigned int bank2_OE = 0xFFFFFFFF;
	unsigned int bank0_IRQSTATUS_0 = 0x00000000;         // mask pending interrupt, so no ISR call until cleared
	unsigned int bank0_IRQSTATUS_1 = 0x00000000;
	unsigned int bank0_IRQSTATUS_ENABLE_0 = 0x00000000;    // Enable IRQ (SET / CLR Register)
	unsigned int bank0_IRQSTATUS_ENABLE_1 = 0x00000000;
	unsigned int bank0_LEVELDETECT0 = 0x00000000;
	unsigned int bank0_LEVELDETECT1 = 0x00000000;
	unsigned int bank0_RAISINGDETECT = 0x00000000;
	unsigned int bank0_FALLINGDETECT = 0x00000000;
	SimulationIOImage lastImage;
	std::mutex imagebuffermutex;

	unsigned int lastSensorIn = 0x00000000;
	unsigned long timestamp = 0;
	std::mutex irqregGPIOmutex;
	std::mutex irqregADCmutex;
	unsigned int adc_irq_enable = 0x00000000;
	unsigned int adc_irq_status = 0x00000000;
	unsigned int adc_ctrl = 0x00000000;
	unsigned int adc_data = 0x00000000;
public:
	void setSimulation(ISimulationImageAccess *sim) {
		simulation = sim;
	}
	void setIRQHandler(SimQNXIRQ *handler){
		irqhandler = handler;
	}

	unsigned int simIn32(unsigned int addr);
	void simOut32(unsigned int addr, unsigned int value);

    void cycleCompletedWith(unsigned long simulationtime, const SimulationIOImage &result, unsigned short int ADCRaw) override;
private:
	bool checkRasingIRQGPIO(unsigned int sensorin);
public:
	bool checkPendingIRQGPIO();
	bool checkPendingIRQADC();
private:
	void updateDataADC(unsigned int rawvalue);
public:
	static SimQNXGPIO* getGPIO() {
		static SimQNXGPIO instance;
		return &instance;
	}
};

#endif /* SRC_SIMGPIO_H_ */
