/* 
 * File:   simheightsensor.cpp
 * @author Lehmann
 * @date 3. April 2020
 */

#include "simheightsensor.h"
#include <cmath>
#include <iostream>

using namespace std;

short int SimHeightSensor::beltvalues[41] = {0x0e3d, 0x0e3b, 0x0e3e, 0x0e3b, 0x0e3f, 0x0e3f, 0x0e3f, 0x0e3e, 0x0e3b, 0x0e3e, 0x0e3b, 0x0e3d, 0x0e3c, 0x0e3e, 0x0e3c, 0x0e3f, 0x0e3a, 0x0e3d, 0x0e3f, 0x0e3d, 0x0e3d, 0x0e3d, 0x0e3a, 0x0e3a, 0x0e3b, 0x0e3f, 0x0e3a, 0x0e3c, 0x0e3b, 0x0e39, 0x0e3b, 0x0e3b, 0x0e3c, 0x0e3b, 0x0e3f, 0x0e3d, 0x0e3e, 0x0e3c, 0x0e3b, 0x0e3d, 0x0e3f};

SimHeightSensor::SimHeightSensor(vector<shared_ptr<SimItem> >* items, double position, SimulationIOImage* regs, unsigned short bitmask) :
allitems(items), position(position), state(regs), bitmask(bitmask), lastHeight(0.0), beltvaluecounter(0) {

}

void SimHeightSensor::evalTimeStep(unsigned int simTime) {
    /* Height is okay if heigth is between 22mm and 46mm according to
     "between flat and 2 CDs and flat+normal item is okay" 
     */
    bool heightokay = false; 

    for (const auto &it : (*allitems)) {
        if (it->state == ItemState::onBelt) {
            double rel_x = it->x - position;

            if (abs(rel_x) <= 32.0) {
                lastHeight = it->getHeight(rel_x);
                if(lastHeight >= 22 and lastHeight <=46){
                    heightokay = true;
                }
            }
            // cout << rel_x << ", " << heightokay << endl;
        }
    }
    if (heightokay) {
        state->in = state->in | bitmask;
    } else {
        state->in = (state->in & (~bitmask));
    }
    //cout << hex << state->in << dec << endl;  
};

unsigned short SimHeightSensor::getADCHeight() {
    unsigned short height = 0;
    
    // determine ADC value of belt height from sampled ring list
    if(beltvaluecounter >=40){
        beltvaluecounter = 0;
    }
    height = beltvalues[beltvaluecounter++];
    
    // check if item is in range
    unsigned short item_height = 0xFFFF;
    for (const auto &it : (*allitems)) {
        if (it->state == ItemState::onBelt) {
            double rel_x = it->x - position;
            //cout << "rel x:" << rel_x << endl;
            // assuming only one item is in measurement range
            if (abs(rel_x) <= 32.0) {
                item_height = it->getADCHeight(rel_x);
                if(item_height < height){
                	height = item_height + (beltvalues[beltvaluecounter] & 0x01);  // add on bit of noise
                }
            }            
        }
    }
    return height;
};
