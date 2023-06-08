/* 
 * File:   simmagenticsensor.cpp
 * @author Lehmann
 * @date 3. April 2020
 */
#include "simmagneticsensor.h"
#include <cmath>
#include <iostream>

using namespace std;

SimMagneticSensor::SimMagneticSensor(vector<shared_ptr<SimItem> >* items, double position, SimulationIOImage* regs, unsigned short bitmask) : 
allitems(items), position(position), state(regs), bitmask(bitmask) {

}

void SimMagneticSensor::evalTimeStep(unsigned int simTime){
    bool magnetic = false;  // true if interrupted
    
    for(const auto &it : (*allitems)){
        if(it->state==ItemState::onBelt && it->isMagnetic()){
            double rel_x = it->x - position;
            double rel_y = it->y - 60;
            
            if(abs(rel_x)<=10.0 && abs(rel_y)<=10){
                magnetic = true;
            }
            //cout << rel_x << ", " << rel_y << ", "<< magnetic << endl;
        }
    }
    if(magnetic){
        state->in = state->in | bitmask;
    }else{
        state->in = (state->in & (~bitmask));
    }
    //cout << hex << state->in << dec << endl;  
};
