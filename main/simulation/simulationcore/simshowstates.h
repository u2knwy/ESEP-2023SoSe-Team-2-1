
/* 
 * File:   simshowstates.h
 * @author Lehmann
 * @date 28. März 2020
 */

#ifndef SIMSHOWSTATES_H
#define SIMSHOWSTATES_H

#include "simbase.h"
#include "simioimage.h"

class SimShowStates{
private:
    SimulationIOImage* state;
    SimulationIOImage last;
public:
    SimShowStates(SimulationIOImage* regs): state(regs){};
    void showFullOut();
    void showDiffOut();
    void showFullIn();
    void showDiffIn();
private:
    void showOut(unsigned short out, unsigned short diff);
    void showIn(unsigned short in, unsigned short diff);
};

#endif /* SIMSHOWSTATES_H */

