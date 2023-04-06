/* 
 * File:   simdrive.cpp
 * @author Lehmann
 * @date 3. April 2020
 */

#include "simdrive.h"
#include <iostream>

using namespace std;

double SimDrive::getSpeed() {
    double result = 0.0;
    if(state!=nullptr){
        //cout << "out: 0x" << hex << state->out << dec << endl;
        if((state->out&0x0008) == 0x0008){
            // stop set
            result = 0.0;
        }
        else{
            if((state->out&0x000F) == 0x0001){
                // stop set
                result = 1.0;
            }
            if((state->out&0x000F) == 0x0002){
                // stop set
                result = -1.0;
            }
            if((state->out&0x000F) == 0x0005){
                // stop set
                result = 0.5;
            }
            if((state->out&0x000F) == 0x0006){
                // stop set
                result = -0.5;
            }
        }
    }
    return result;
}

