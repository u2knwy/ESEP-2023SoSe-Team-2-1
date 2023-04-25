/* 
 * File:   simfeedpusher.cpp
 * @author Lehmann
 * @date 22. Mai 2020
 */

#include "simfeedpusher.h"

#define SIM_BITMASK_FEEDSEPERATOR_ACTOR 0x0080
#define SIM_BITMASK_FEEDSEPERATOR_SENSOR 0x0020

bool SimFeedPusher::isModePassing() {
    bool result = false;
    if (state != nullptr) {
        // if set, pushed out, no passing
        result = !((state->out & SIM_BITMASK_FEEDSEPERATOR_ACTOR) == SIM_BITMASK_FEEDSEPERATOR_ACTOR);  
        if (result) {
            state->in = (state->in | SIM_BITMASK_FEEDSEPERATOR_SENSOR);
        } else {
            state->in = (state->in & (~SIM_BITMASK_FEEDSEPERATOR_SENSOR));
        }
    }
    return result;
}
