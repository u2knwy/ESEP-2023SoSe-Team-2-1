/** 
 * File:   simfeedseparator.cpp
 * @author Lehmann
 * @date 3. April 2020
 */

#include "simfeedseparator.h"
#include "simioimage.h"

#define SIM_BITMASK_FEEDSEPERATOR_ACTOR 0x0080
#define SIM_BITMASK_FEEDSEPERATOR_SENSOR 0x0020

bool SimFeedSeparator::isModePassing() {
    bool result = false;
    if (state != nullptr) {
        result = (state->out & SIM_BITMASK_FEEDSEPERATOR_ACTOR) == SIM_BITMASK_FEEDSEPERATOR_ACTOR;
        if (result) {
            state->in = (state->in | SIM_BITMASK_FEEDSEPERATOR_SENSOR);
        } else {
            state->in = (state->in & (~SIM_BITMASK_FEEDSEPERATOR_SENSOR));
        }
    }
    return result;
}
