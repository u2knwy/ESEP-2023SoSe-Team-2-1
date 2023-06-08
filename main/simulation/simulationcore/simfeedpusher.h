/* 
 * File:   simfeedpusher.h
 * @author Lehmann
 * @date 22. Mai 2020
 */

#ifndef SIMFEEDPUSHER_H
#define SIMFEEDPUSHER_H

#include "simioimage.h"
#include "isimseparator.h"

// adapter for translation of register setting to open state

class SimFeedPusher : public ISimSeparator {
public:
    static constexpr double position = 395.0;  // center of pusher
private:
    SimulationIOImage* state;
public:

    SimFeedPusher(SimulationIOImage* regs) : state(regs) {
    };
    /**
     * Translate register settings to opening state
     * @return state (true if open)
     */
    bool isModePassing() override;

    double getPosition() override {
        return position;              
    };

    bool isPusher() override {
        return true;
    };
};


#endif /* SIMFEEDPUSHER_H */

