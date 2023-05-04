
/* 
 * File:   simconveyorbelt.h
 * @author Lehmann
 * @date 3. April 2020
 */

#ifndef SIMCONVEYORBELT_H
#define SIMCONVEYORBELT_H

#include "simbase.h"
#include "isimdrophandler.h"
#include "simslide.h"
#include "simdrive.h"
#include "isimseparator.h"
#include "simitem.h"

#include <vector>
#include <memory>

using namespace std;

class SimConveyorBelt{
private:
    vector<shared_ptr<SimItem>> *items;
    SimSlide *slide;
    SimDrive *drive;
    ISimSeparator *separator;
    double speedX;
    double speedY;
    ISimDropHandler *dh;
    int displayCounter;
    bool previousMode;
public:
    SimConveyorBelt(vector<shared_ptr<SimItem>> *items, SimSlide *slide, SimDrive *drive, ISimSeparator *separator);
    void addItem(const shared_ptr<SimItem> &item);
    void evalTimeStep(unsigned int simTime);
    void setDropHandler(ISimDropHandler* dh);
    bool positionOnBelt(const shared_ptr<SimItem>  &item);
    bool collisions(const shared_ptr<SimItem>  &item);
};

#endif /* SIMCONVEYORBELT_H */

