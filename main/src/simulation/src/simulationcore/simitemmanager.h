
/* 
 * File:   itemmanger.h
 * @author Lehmann
 * @date 3. April 2020
 */

#ifndef ITEMMANGER_H
#define ITEMMANGER_H

#include "simbase.h"
#include "simconveyorbelt.h"
#include "simitemhandlingaction.h"
#include "simitem.h"

#include <vector>
#include <memory>

using namespace std;

class SimItemManager{
private:
    vector<shared_ptr<SimItem>> *allitems;
    SimConveyorBelt *conveyor;
    SimSlide *slide;
public:
    SimItemManager(vector<shared_ptr<SimItem>> *items, SimConveyorBelt *conveyor, SimSlide *slide);
    void addItem(ItemKinds kind=ItemKinds::flat, bool flip=false, bool sticky=false, double x=0, double y=60);
    void handleItemAction(const SimItemHandlingAction &action);
    void housekeeping();
    int getNumberOfItems(){return allitems->size();};
    void checkRoI();
    std::string toJSONStringFragment();
};

#endif /* ITEMMANGER_H */

