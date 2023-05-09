/* 
 * File:   simlationexecutionthread.h
 * @author Lehmann
 * @date 6. April 2020
 */

#ifndef SIMLATIONEXECUTIONTHREAD_H
#define SIMLATIONEXECUTIONTHREAD_H

#include "isimulationexecution.h"

class SimulationExecuter{
private:
    ISimlationExecution *simulation;
    unsigned int timeslice = 200;
    bool run = true;
public:
    SimulationExecuter(ISimlationExecution *simulation, unsigned int timeslice): simulation(simulation), timeslice(timeslice), run(true){};
    void operator()();
};
#endif /* SIMLATIONEXECUTIONTHREAD_H */

