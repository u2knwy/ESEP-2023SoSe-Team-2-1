/*
 * simqnxgpioapi.h
 *
 *  Created on: 23.04.2020
 *      Author: Lehmann
 */

#ifndef SRC_SIMULATION_SIMQNXGPIOAPI_H_
#define SRC_SIMULATION_SIMQNXGPIOAPI_H_

#include <cstdint>
using namespace std;

#define out32(x,y) simqnx_Out32((x),(y))
#define in32(x) simqnx_In32(x)
#define mmap_device_io(x,y) simqnx_mmap_device_io((x),(y))

uint32_t simqnx_In32(uintptr_t addr);

void simqnx_Out32(uintptr_t addr, uint32_t value);

uintptr_t simqnx_mmap_device_io(size_t len, uint64_t io);

#endif /* SRC_SIMULATION_SIMQNXGPIOAPI_H_ */
