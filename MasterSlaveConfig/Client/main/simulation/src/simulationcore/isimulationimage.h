/*
 * isimulationimage.h
 *
 *  Created on: 09.04.2020
 *      Author: Lehmann
 */

#ifndef SRC_ISIMULATIONIMAGE_H_
#define SRC_ISIMULATIONIMAGE_H_

class ISimulationImageAccess{
public:
	virtual ~ISimulationImageAccess(){};
	virtual unsigned short readIn() = 0;
	virtual unsigned short readOut() = 0;
	virtual void writeOut(unsigned short) = 0;
};

#endif /* SRC_ISIMULATIONIMAGE_H_ */
