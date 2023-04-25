/* 
 * File:   simsensors.cpp
 * @author Lehmann
 * @date 29. März 2020
 */
#include "simsensors.h"
#include "simmasks.h"
#include <iostream>

using namespace std;

SimSensors::SimSensors(SimulationIOImage* shadow) : shadow(shadow) {
    // Default
    unsigned short lbDefault = SIM_ITEM_DETECTED |
            SIM_ITEM_AT_HEIGHT_SENSOR | 
            SIM_ITEM_AT_JUNCTION | 
            SIM_BUFFER_IS_FULL |
            SIM_ITEM_AT_END;
    actions[0][0] = 0;   // Init
    actions[0][1] = lbDefault;
            
    actions[1][0] = 20;   // beginning
    actions[1][1] = SIM_ITEM_AT_HEIGHT_SENSOR |
            SIM_ITEM_AT_JUNCTION |
            SIM_BUFFER_IS_FULL |
            SIM_ITEM_AT_END;

    actions[2][0] = 30;   // default
    actions[2][1] = lbDefault;

    actions[3][0] = 40;   // height
    actions[3][1] = SIM_ITEM_DETECTED |
            SIM_ITEM_AT_JUNCTION |
            SIM_BUFFER_IS_FULL |
            SIM_ITEM_AT_END;

    actions[4][0] = 50;    // default
    actions[4][1] = lbDefault;

    actions[5][0] = 70;    // seperator
    actions[5][1] = SIM_ITEM_DETECTED |
            SIM_ITEM_AT_HEIGHT_SENSOR |
            SIM_BUFFER_IS_FULL |
            SIM_ITEM_AT_END;

    actions[6][0] = 80;    // default
    actions[6][1] = lbDefault;

    actions[7][0] = 100;    // end
    actions[7][1] = SIM_ITEM_DETECTED |
            SIM_ITEM_AT_HEIGHT_SENSOR |
            SIM_ITEM_AT_JUNCTION |
            SIM_BUFFER_IS_FULL;

    actions[8][0] = 110;    // default
    actions[8][1] = lbDefault;

    actions[9][0] = 100;    // slide
    actions[9][1] = SIM_ITEM_DETECTED |
            SIM_ITEM_AT_HEIGHT_SENSOR |
            SIM_ITEM_AT_JUNCTION |
            SIM_ITEM_AT_END;

    actions[10][0] = 110;    // default
    actions[10][1] = lbDefault;
    
    actions[11][0] = 350;    // begin
    actions[11][1] = SIM_ITEM_AT_HEIGHT_SENSOR | 
            SIM_ITEM_AT_JUNCTION | 
            SIM_BUFFER_IS_FULL |
            SIM_ITEM_AT_END;

    actions[12][0] = 360;    // begin + height
    actions[12][1] = SIM_ITEM_AT_JUNCTION | 
            SIM_BUFFER_IS_FULL |
            SIM_ITEM_AT_END;

    actions[13][0] = 110;    // default
    actions[13][1] = SIM_ITEM_DETECTED |
            SIM_ITEM_AT_JUNCTION |
            SIM_BUFFER_IS_FULL |
            SIM_ITEM_AT_END;
    
    actions[14][0] = 110;    // default
    actions[14][1] = lbDefault;
    
    actions[15][0] = 10000;
    actions[15][1] = 0;
};

void SimSensors::init() {
    unsigned short sensor_mask = SIM_ITEM_DETECTED |
            SIM_ITEM_AT_HEIGHT_SENSOR | SIM_ITEM_HEIGHT_FITS |
            SIM_ITEM_AT_JUNCTION | SIM_ITEM_IS_METTAL |
            SIM_JUNCTION_IS_OPEN | SIM_BUFFER_IS_FULL |
            SIM_ITEM_AT_END;

    unsigned short action = SIM_ITEM_DETECTED |
            SIM_ITEM_AT_HEIGHT_SENSOR |
            SIM_ITEM_AT_JUNCTION |
            SIM_BUFFER_IS_FULL |
            SIM_ITEM_AT_END;
    
    shadow->in = (shadow->in & ~sensor_mask) | action;
}

void SimSensors::evalTime(unsigned int simtime) {
    unsigned short sensor_mask = SIM_ITEM_DETECTED |
            SIM_ITEM_AT_HEIGHT_SENSOR | SIM_ITEM_HEIGHT_FITS |
            SIM_ITEM_AT_JUNCTION | SIM_ITEM_IS_METTAL |
            SIM_JUNCTION_IS_OPEN | SIM_BUFFER_IS_FULL |
            SIM_ITEM_AT_END;

    int found_index = -1;

    //cout << "simtime" << simtime << endl;

    for (int i = 0; i < 5; i++) {
        if (actions[i][0] <= simtime) {
            if (actions[i + 1][0] > simtime) {
                found_index = i;
                break;
            }
        }
    }
    if (found_index != -1 && found_index != last_action) {
        shadow->in = (shadow->in & ~sensor_mask) | actions[found_index][1];
        cout << "<SIM> sensor changed: " << hex << "0x" << actions[found_index][1] << dec << endl;
        last_action = found_index;
    }
};
