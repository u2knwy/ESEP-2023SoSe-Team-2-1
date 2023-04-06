/* 
 * File:   simheightsensor.h
 * @author Lehmann
 * @date 3. April 2020
 */

#ifndef SIMHEIGHTSENSOR_H
#define SIMHEIGHTSENSOR_H

#include "simioimage.h"
#include <vector>
#include <memory>
#include "simitem.h"

using namespace std;

class SimHeightSensor {
private:
    vector<shared_ptr<SimItem>> *allitems;
    double position;
    SimulationIOImage* state;
    unsigned short bitmask;
    double lastHeight;
    static short int beltvalues[41];
    unsigned char beltvaluecounter = 0;
public:
    SimHeightSensor(vector<shared_ptr<SimItem>> *items, double position, SimulationIOImage* regs, unsigned short bitmask);
    virtual ~SimHeightSensor(){};
    virtual void evalTimeStep(unsigned int simTime);
    unsigned short getADCHeight();
};



#endif /* SIMHEIGHTSENSOR_H */

