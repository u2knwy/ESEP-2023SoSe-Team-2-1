/*
 * isimcycleendhandler.h
 *
 *  Created on: 11.04.2020
 *      Author: Lehmann
 */

#ifndef SRC_SIMULATION_ISIMULATIONCYCLEENDHANDLER_H_
#define SRC_SIMULATION_ISIMULATIONCYCLEENDHANDLER_H_

#include "simioimage.h"

class ISimulationCycleEndHandler{
public:
	virtual ~ISimulationCycleEndHandler(){};
	virtual void cycleCompletedWith(unsigned long simulationtime, const SimulationIOImage &result, unsigned short ADCRaw) = 0;
};

#endif /* SRC_SIMULATION_ISIMULATIONCYCLEENDHANDLER_H_ */
