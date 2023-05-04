/* 
 * File:   simsensors.h
 * @author Lehmann
 * @date 29. März 2020
 */

#ifndef SIMSENSORS_H
#define SIMSENSORS_H

#include "simioimage.h"

class SimSensors {
private:
    SimulationIOImage* shadow;
    int last_action = -1;
    unsigned short actions[16][2];
public:
    SimSensors(SimulationIOImage* shadow);
    void init();
    void evalTime(unsigned int simtime);
};



#endif /* SIMSENSORS_H */

