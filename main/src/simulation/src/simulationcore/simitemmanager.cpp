
/*
 * File:   itemmanger.cpp
 * @author Lehmann
 * @date 3. April 2020
 */

#include "simitemmanager.h"
#include "simconveyorbelt.h"
#include "simconfquery.h"
#include <sstream>
#include <iostream>
using namespace std;

SimItemManager::SimItemManager(vector<shared_ptr<SimItem>> *items, SimConveyorBelt *conveyor, SimSlide *slide) :
allitems(items), conveyor(conveyor), slide(slide) {

}

void SimItemManager::addItem(ItemKinds kind, bool flip, bool sticky, double x, double y) {
    auto newitem = make_shared<SimItem>(kind, x, y);
    if (flip) {
        newitem->setFlipping();
    }
    if (sticky) {
        newitem->setSticky();
    }

    if (allitems != nullptr) {
    	if(conveyor->positionOnBelt(newitem) && !conveyor->collisions(newitem)) {
			allitems->push_back(newitem);
			//cout << "added item " << endl;
			if (conveyor != nullptr) {
				conveyor->addItem(newitem);
				//cout << "added item to belt" << endl;
			}
    	}
    }
}

void SimItemManager::handleItemAction(const SimItemHandlingAction &action) {
    if (allitems != nullptr) {
        vector<shared_ptr < SimItem>>::iterator it;
        switch (action.actionkind) {
            case SimItemHandlingActionKind::nop:
                break;
            case SimItemHandlingActionKind::add:
                addItem(action.kind, action.flip, action.sticky, action.x, action.y);
                break;

            case SimItemHandlingActionKind::removeall:
            {
                shared_ptr<SimItem> found = nullptr;
                for (it = allitems->begin(); it != allitems->end(); ++it) {
                    if ((*it)->state == ItemState::onBelt || (*it)->state == ItemState::onSlide) {
                        (*it)->state = ItemState::removed;
                    }
                }
                if(SIMCONFQUERRY_ISACTIVE(showactions)){

                	cout << "<SIM> Removed all" << endl;
                }
                break;
            }

            case SimItemHandlingActionKind::removeatend:
            {
                shared_ptr<SimItem> found = nullptr;
                double maxX = 0.0;
                for (it = allitems->begin(); it != allitems->end(); ++it) {
                    if ((*it)->state == ItemState::onBelt) {
                        // is in range end?
                        if ((*it)->x >= 630 && (*it)->x <= 700) {
                            if ((*it)->x > maxX) {
                                found = (*it);
                                maxX = (*it)->x;
                            }
                        }
                    }
                }
                if (found != nullptr) {
                    if(SIMCONFQUERRY_ISACTIVE(showactions)){
                    	cout << "<SIM> Removed at end" << endl;
                    }
                    found->state = ItemState::removed;
                }
                break;
            }

            case SimItemHandlingActionKind::removeatbegin:
            {
                cout << "<SIM> Not Implemented Yet (removeatbegin)" << endl;
                break;
            }
            case SimItemHandlingActionKind::removeallslide:
            {
                if (slide != nullptr) {
                    slide->removeAll();
                }
                break;
            }
            case SimItemHandlingActionKind::removeid:
            {
                shared_ptr<SimItem> found = nullptr;
                for (it = allitems->begin(); it != allitems->end(); ++it) {
					// is given ID?
					if ((*it)->ID == action.ID) {
							found = (*it);
					}
                }
                if (found != nullptr) {
                    if(SIMCONFQUERRY_ISACTIVE(showactions)){
                    	cout << "<SIM> Removed ID: " << found->ID << endl;
                    }
                    found->state = ItemState::removed;
                }
                break;
            }
            default:
                cout << "<SIM> Not Implemented Yet (default)" << endl;
                ;

        }
        housekeeping();
    }
}

void SimItemManager::housekeeping() {
    if (allitems != nullptr) {
        vector<shared_ptr < SimItem>>::iterator it;
        it = allitems->begin();
        while (it != allitems->end()) {
            if (((*it)->state == ItemState::removed) || ((*it)->state == ItemState::droppedLeft) || ((*it)->state == ItemState::droppedRight)) {
                it = allitems->erase(it);
            } else {
                ++it;
            }
        }
    }
}

std::string SimItemManager::toJSONStringFragment() {
    std::stringstream result;
    result << " \"items\": [";
    vector<shared_ptr < SimItem>>::iterator it;

    for (it = allitems->begin(); it != allitems->end(); ++it) {
        if (it != allitems->begin()) {
            result << ", ";
        }
        result << (*it)->toJSONString();
    }

    result << "]";

    return result.str();
}

void SimItemManager::checkRoI() {
    vector<shared_ptr < SimItem>>::iterator it;

    for (it = allitems->begin(); it != allitems->end(); ++it) {
        // begin of belt
        if ((0 <= (*it)->x) && (40 >= (*it)->x)) {
            if ((*it)->roi != RoI::entry) {
                // entered rigion
                (*it)->roi = RoI::entry;
                std::stringstream kind;
                (*it)->typeToShortTypeName(kind);

                if(SIMCONFQUERRY_ISACTIVE(showroi)){
                	cout << "<SIM> Item " << (*it)->ID << "/" << kind.str() << " entered entry-region" << endl;
                }
            }
        } else {
            // in slide
            if (90 <= (*it)->y) {
                if ((*it)->roi != RoI::discarded) {
                    // entered rigion
                    (*it)->roi = RoI::discarded;
                    std::stringstream kind;
                    (*it)->typeToShortTypeName(kind);

                    if(SIMCONFQUERRY_ISACTIVE(showroi)){
                    	cout << "<SIM> Item " << (*it)->ID << "/" << kind.str() << " entered discard-region" << endl;
                    }
               }
            } else {
                // end of belt
                if ((655 - 20 <= (*it)->x) && (655 + 20 >= (*it)->x)) {
                    if ((*it)->roi != RoI::exit) {
                        // entered rigion
                        (*it)->roi = RoI::exit;
                        std::stringstream kind;
                        (*it)->typeToShortTypeName(kind);

                        if(SIMCONFQUERRY_ISACTIVE(showroi)){
                        	cout << "<SIM> Item " << (*it)->ID << "/" << kind.str() << " entered exit-region" << endl;
                        }

                    }
                } else {
                    (*it)->roi = RoI::none;
                }
            }
        }
    }
};
