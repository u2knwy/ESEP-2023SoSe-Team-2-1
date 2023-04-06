/*
 * simulationimageaccesdummy.h
 *
 *  Created on: 09.04.2020
 *      Author: Lehmann
 */

#ifndef SRC_SIMULATIONIMAGEACCESSDUMMY_H_
#define SRC_SIMULATIONIMAGEACCESSDUMMY_H_

#include "isimulationimage.h"

class SimulationImageAccessDummy:public ISimulationImageAccess{
private:
	unsigned short outreg = 0;
public:
	unsigned short readIn() override {return 0xFFFF;};
	unsigned short readOut() override {return outreg;};
	void writeOut(unsigned int short value) override {cout << hex << "<SimDummy out>" << value << dec << endl; outreg=value;};
};
#endif 
