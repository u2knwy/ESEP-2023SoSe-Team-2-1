/* 
 * File:   simconfhandler.h
 * @author Lehmann
 * @date 08. Feb. 2023
 */

#ifndef SIMCONFHANDLER_H
#define SIMCONFHANDLER_H

#include "simconfaction.h"
#include <mutex>
#include <vector>

using namespace std;

class SimConfHandler {
private:
    vector<SimConfAction> actions;
    std::mutex buffermutex;
public:
    SimConfHandler();
    void init();
    void evalTime(unsigned int simTime);
    void addAction(const SimConfAction &action);
};

#endif /* SIMHCI_H */

