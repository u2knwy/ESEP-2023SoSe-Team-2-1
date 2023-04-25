/* 
 * File:   simitemhandling.h
 * @author Lehmann
 * @date 3. April 2020
 */

#ifndef SIMITEMHANDLING_H
#define SIMITEMHANDLING_H

#include "simitemmanager.h"
#include "simitemhandlingaction.h"
#include <vector>
#include <mutex>
#include "simitem.h"

class SimItemHandling {
private:
    SimItemManager *manager;
    vector<SimItemHandlingAction> actions;
    std::mutex buffermutex;
public:
    SimItemHandling();
    SimItemHandling(SimItemManager *manager);
    void init();
    void setManger(SimItemManager *manager){this->manager = manager;};
    void evalTime(unsigned int simtime);
    void addAction(const SimItemHandlingAction &action);
    unsigned int numberOfActions(){return actions.size();};
};

#endif /* SIMITEMHANDLING_H */

