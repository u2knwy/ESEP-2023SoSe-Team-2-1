
/* 
 * File:   simctrlhandler.h
 * @author Lehmann
 * @date 16. Mai 2020
 */

#ifndef SIMCTRLHANDLER_H
#define SIMCTRLHANDLER_H

#include "simulation.h"
#include "simctrlaction.h"
#include <mutex>

class SimCtrlHandler{
private:
    Simulation *sim = nullptr;
    std::mutex buffermutex;
public:
    SimCtrlHandler(Simulation *sim):sim(sim){};
    void addAction(SimCtrlAction action);
};

#endif /* SIMCTRLHANDLER_H */

