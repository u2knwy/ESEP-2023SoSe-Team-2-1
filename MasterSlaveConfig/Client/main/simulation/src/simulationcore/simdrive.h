/* 
 * File:   simdrive.h
 * @author Lehmann
 * @date 3. April 2020
 */

#ifndef SIMDRIVE_H
#define SIMDRIVE_H

#include "simioimage.h"
// adapter for translation of register setting to speed

class SimDrive{
private:
        SimulationIOImage* state;
public:
    SimDrive(SimulationIOImage* regs): state(regs){};
    /**
     * Translate register settings to speed in sense of 
     * +1.0 moving right
     * +0.5 moving right slow
     * 0    stop
     * -0.5 moving left  slow
     * -1.0 moving left
     * @return speedvalue
     */
    double getSpeed();
};

#endif /* SIMDRIVE_H */

