/* 
 * File:   simhci.h
 * @author Lehmann
 * @date 29. März 2020
 */

#ifndef SIMHCI_H
#define SIMHCI_H

#include "simbase.h"
#include "simioimage.h"
#include "simhciaction.h"
#include <mutex>
#include <vector>

using namespace std;

class SimHCI {
private:
    SimulationIOImage* shadow;
    vector<SimHCIAction> actions;
    std::mutex buffermutex;
public:
    SimHCI(SimulationIOImage* shadow = nullptr);
    void init();
    void evalTime(unsigned int simTime);
    void addAction(const SimHCIAction &action);

    void setShadow(SimulationIOImage* shadow) {
        this->shadow = shadow;
    };
};


#endif /* SIMHCI_H */

