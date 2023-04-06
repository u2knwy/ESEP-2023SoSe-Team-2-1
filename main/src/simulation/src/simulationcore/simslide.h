/* 
 * File:   simslide.h
 * @author Lehmann
 * @date 3. April 2020
 */

#ifndef SIMSLIDE_H
#define SIMSLIDE_H

#include "simbase.h"
#include "simitem.h"

#include <vector>
#include <memory>

using namespace std;

class SimSlide{
public:
    static constexpr double slideDepth = 130;    // three items of 40 mm
private:
    vector<shared_ptr<SimItem>> *items;
    double speedY;
    int displayCounter;
public:
    SimSlide(vector<shared_ptr<SimItem>> *items);
    void addItem(const shared_ptr<SimItem> &item);
    void evalTimeStep(unsigned int simTime); 
    void removeFirst();
    void removeAll();
};

#endif /* SIMSLIDE_H */

