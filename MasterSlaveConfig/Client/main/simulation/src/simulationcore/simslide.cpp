/* 
 * File:   simslide.cpp
 * @author Lehmann
 * @date 3. April 2020
 */
#include "simslide.h"
#include "simconfquery.h"

#include <iostream>

SimSlide::SimSlide(vector<shared_ptr<SimItem>> *items) : items(items), speedY(0.0), displayCounter(0) {
    speedY = 80 * SimulationBase::timeslice / 1000.0;
    ; // 80 mm/s * sim cycle time/step --> mm / step
};

void SimSlide::addItem(const shared_ptr<SimItem> &item) {
    item->state = ItemState::onSlide;
    if (items != nullptr) {
        items->push_back(item);
    }
}

void SimSlide::evalTimeStep(unsigned int simTime) {
    if (items != nullptr) {
        // housekeeping
        vector<shared_ptr<SimItem>>::iterator it = items->begin();
        while (it != items->end()) {
            // check if removed from system
            if ((*it)->state != ItemState::onSlide) {
                it = items->erase(it);
            } else {
                ++it;
            }
        }

        // move remaining items
        for (unsigned int index = 0; index < items->size(); ++index) {
            shared_ptr<SimItem> item = (*items)[index];
            double maxY = 80 + slideDepth - 20 - index * 40;
            item->y = item->y + speedY;
            if (item->y > maxY) {
                item->y = maxY;
            }
        }
        if (SIMCONFQUERRY_ISACTIVE(showpositions)) {
            if (displayCounter <= 0) {
                for(const auto& it : *items){  //it is shared_ptr<item>
                    cout << "<SIM> " << (it)->x << ", " << (it)->y << " (slide)" << endl;
                }
                displayCounter = SimulationBase::displayEachNCycle - 1;
            } else {
                displayCounter--;
            }
        }
    }
}

void SimSlide::removeFirst() {
    if (items != nullptr) {
        vector<shared_ptr<SimItem>>::iterator it = items->begin();
        while (it != items->end()) {
            // check if still in system an not taged as removed yet
            if ((*it)->state == ItemState::onSlide) {
                // housekeeping removes item later
                (*it)->state = ItemState::removed;
                it = items->end();
            } else {
                ++it;
            }
        }
    }
}

void SimSlide::removeAll() {
    if (items != nullptr) {
        vector<shared_ptr<SimItem>>::iterator it = items->begin();
        while (it != items->end()) {
            // housekeeping removes item later
            (*it)->state = ItemState::removed;
            ++it;
        }
    }
}
