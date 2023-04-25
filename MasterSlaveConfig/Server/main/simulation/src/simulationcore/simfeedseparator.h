/* 
 * File:   simfeedseparator.h
 * @author Lehmann
 * @date 3. April 2020
 */

#ifndef SIMFEEDSEPARATOR_H
#define SIMFEEDSEPARATOR_H

#include "simioimage.h"
#include "isimseparator.h"

// adapter for translation of register setting to open state

class SimFeedSeparator : public ISimSeparator{
public:
    static constexpr double position = 400.0;    // front of bar!
private:
    SimulationIOImage* state;
public:
    SimFeedSeparator(SimulationIOImage* regs) : state(regs) {
    };
    /**
     * Translate register settings to opening state
     * @return state (true if open)
     */
    bool isModePassing() override;
     double getPosition() override {return position;};   
     bool isPusher() override {return false;};
};

#endif /* SIMFEEDSEPARATOR_H */

