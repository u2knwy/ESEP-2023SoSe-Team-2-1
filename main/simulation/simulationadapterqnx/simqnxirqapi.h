/*
 * simqnxirqapi.h
 *
 *  Created on: 23.04.2020
 *      Author: Lehmann
 */

#ifndef SRC_SIMULATION_SIMQNXIRQAPI_H_
#define SRC_SIMULATION_SIMQNXIRQAPI_H_


#define InterruptAttach(v,w,x,y,z) simInterruptAttach(v,w,x,y,z)
#define InterruptAttachEvent(x,y,z) simInterruptAttachEvent(x,y,z)
#define InterruptMask(x,y) simInterruptMask(x,y)
#define InterruptUnmask(x,y) simInterruptUnmask(x,y)
#define InterruptDetach(x) simInterruptDetach(x)

int simInterruptMask(int intr, int id);

int simInterruptUnmask(int intr, int id);

int simInterruptAttach(int intr,
		const struct sigevent * (*handler)(void *, int), const void * area,
		int size, unsigned flags);

int simInterruptAttachEvent(int intr, const struct sigevent* event,
		unsigned flags);

int simInterruptDetach(int id);

#endif /* SRC_SIMULATION_SIMQNXIRQAPI_H_ */
