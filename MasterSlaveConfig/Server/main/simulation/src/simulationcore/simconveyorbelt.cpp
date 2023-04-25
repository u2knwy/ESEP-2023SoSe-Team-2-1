/* 
 * File:   simconveyorbelt.cpp
 * @author Lehmann
 * @date 3. April 2020
 */

#include "simconveyorbelt.h"
#include "simconfquery.h"

#include <iostream>
#include <cmath>

using namespace std;

#define BELT_POS_DROP_LEFT    0.0
#define BELT_POS_DROP_RIGHT 700.0
#define BELT_POS_UPPER_LIMIT 60.0
#define BELT_POS_LOWER_LIMIT 20.0
#define BELT_GUIDANCE_RAMP_BEGIN 200.0
#define BELT_GUIDANCE_LENGTH     300.0

SimConveyorBelt::SimConveyorBelt(vector<shared_ptr<SimItem>> *items,
        SimSlide* slide, SimDrive* drive, ISimSeparator* separator) :
        items(items), slide(slide), drive(drive), separator(separator), 
        speedX(0.0), speedY(0.0), dh(nullptr), 
        displayCounter(SimulationBase::displayEachNCycle - 1), previousMode(false) {
    speedX = SPEED_X_NORMAL * SimulationBase::timeslice / 1000.0; // x mm/s * sim cycle time/step --> mm / step, reduced to improve sampling
    speedY = SPEED_Y_NORMAL * SimulationBase::timeslice / 1000.0; // y mm/s * sim cycle time/step --> mm / step
    if (separator != NULL) {
        previousMode = separator->isModePassing();
    }
}

bool SimConveyorBelt::positionOnBelt(const shared_ptr<SimItem> &item){
	bool result = false;

	result = (item->x >= BELT_POS_DROP_LEFT) && (item->x <= BELT_POS_DROP_RIGHT);
	result = result && ((item->y >= BELT_POS_LOWER_LIMIT) && (item->y <= BELT_POS_UPPER_LIMIT));

	return result;
}

bool SimConveyorBelt::collisions(const shared_ptr<SimItem> &item){
	bool collision = false;

	if((item->y < BELT_POS_LOWER_LIMIT) || (item->y > BELT_POS_UPPER_LIMIT)){
		collision = true;
	}
	if((item->x > BELT_GUIDANCE_RAMP_BEGIN-8) && (item->x <= BELT_GUIDANCE_RAMP_BEGIN+38-8)){
		// Ramp
		if(item->y < 20+38){
			double dx = item->x - (BELT_GUIDANCE_RAMP_BEGIN - 8);
			double dy = item->y - 20;
			//std::cout << item->x << ", " << item->y << std::endl;
			//std::cout << dx << "/" << dy << std::endl;
			if(dy < dx){
				collision = true;
			}
		}
	}
	if((item->x > BELT_GUIDANCE_RAMP_BEGIN+38-8) && (item->x < BELT_GUIDANCE_RAMP_BEGIN+BELT_GUIDANCE_LENGTH+20)){
		//AABB
		if (item->y < 20 + 38) {
			collision = true;
		}
	}

	return collision;
}

void SimConveyorBelt::addItem(const shared_ptr<SimItem> &item) {
    if (items != nullptr) {
        if(positionOnBelt(item) && !collisions(item)) {
            items->push_back(item);
        }
        else {
        	item->state=ItemState::removed;
        }
    }
}

void SimConveyorBelt::evalTimeStep(unsigned int simTime) {
    vector<shared_ptr<SimItem>>::iterator it;
    double currentSpeed = 0.0;
    double stepX = 0.0;
    double stepY = 0.0;

    if (drive != nullptr) {
        currentSpeed = drive->getSpeed();
#ifdef SPEED_X_ALTERNATES
        double phi1 = (2 * M_PI) * ((double) (simTime - (simTime / 8000)*8000) / 8000.0);
        double phi2 = (2 * M_PI) * ((double) (simTime - (simTime / 17000)*17000) / 17000.0);
        //cout << "phi:" << phi1 << " Factor:" << 1+sin(phi1)*0.1 << endl;
        stepX = speedX * currentSpeed * (1 + sin(phi1)*0.1 + sin(phi2)*0.05);
#else
        stepX = speedX * currentSpeed;
#endif
        stepY = speedY * currentSpeed;
    }
    //cout << "x increment:" << stepX << endl;
    if (items != nullptr) {
        for (it = items->begin(); it != items->end(); ++it) {
            double current_x = (*it)->x;
            if (current_x >= BELT_POS_DROP_LEFT && current_x <= BELT_POS_DROP_RIGHT) {
                if (separator->isPusher()) {
                    if (!separator->isModePassing()) { // errected
                        if (separator->isModePassing() != previousMode) {
                            // push all.
                            //std::cout << "push out " << (*it)->x << std::endl;
                            if (abs(current_x - separator->getPosition()) <= 10) { // in front of pusher
                                (*it)->y = 80;
                            } else {
                                if (abs(current_x - (separator->getPosition() - 20)) <= 10) {// only close to pusher
                                    //std::cout << "in front kick range" << std::endl;
                                    (*it)->x = separator->getPosition() - 30;
                                } else {
                                    if (abs(current_x - (separator->getPosition() + 20)) <= 10) {// only close to pusher
                                        //std::cout << "in back kick range" << std::endl;
                                        (*it)->x = separator->getPosition() + 30;
                                    }
                                }
                            }
                        }
                        // blocking
                        if (abs((*it)->x - (separator->getPosition() - 30)) <= stepX) {
                            (*it)->x = separator->getPosition() - 30;
                        } else {
                            (*it)->x = (*it)->x + stepX;
                        }
                    } else { // simply pass
                        (*it)->x = current_x + stepX;
                    }
                } else {
                    // in feedseparator (switch) range ?
                    if (abs(current_x - separator->getPosition()) <= stepX) {
                        // in feedseparator range
                        if (!separator->isModePassing()) {
                            // close, move to slide
                            (*it)->x = separator->getPosition();
                            // are we sticky?
                            if((*it)->sticky){
                                (*it)->y = (*it)->y + stepY/2;   // reduce speed to a half.
                            } else {
                                (*it)->y = (*it)->y + stepY;
                            }
                        } else {
                            // open, go on
                            (*it)->x = current_x + stepX;
                            (*it)->y = 60.0;
                        }
                    } else {
                        (*it)->x = current_x + stepX;
                    }
                }
            }
            // Ramp collisions
            if(((*it)->x > BELT_GUIDANCE_RAMP_BEGIN-8) && ((*it)->x <= BELT_GUIDANCE_RAMP_BEGIN-8+38)){
            	// in Ramp section
            	if((*it)->y < (20+38)) {
        			double dx = (*it)->x - (BELT_GUIDANCE_RAMP_BEGIN - 8);
        			double dy = (*it)->y - 20;
        			if(dy < dx){
        				double diff = dx-dy;
        				double rdiff = sqrt(diff);
        				(*it)->x = (*it)->x - diff + rdiff;
        				(*it)->y = (*it)->y + rdiff;
        			}
            	}
            }

            if (SIMCONFQUERRY_ISACTIVE(showpositions)) {

				if (displayCounter <= 0) {
					cout << "<SIM> " << (*it)->x << ", " << (*it)->y << endl;
					displayCounter = SimulationBase::displayEachNCycle - 1;
				} else {
					displayCounter--;
				}
            }
        }
        previousMode = separator->isModePassing();

        it = items->begin();
        while (it != items->end()) {
            // check dropped
            bool erase = false;
            if ((*it)->state == ItemState::removed) {
                erase = true;
            } else {
                if ((*it)->x < BELT_POS_DROP_LEFT) {
                    (*it)->state = ItemState::droppedLeft;
                    erase = true;
                } else {
                    if ((*it)->x > BELT_POS_DROP_RIGHT) {
                        (*it)->state = ItemState::droppedRight;
                        if (dh != nullptr) {
                            dh->dropEnd(*it);
                        }
                        erase = true;
                    } else {
                        if ((*it)->y >= 80) {
                            (*it)->state = ItemState::onSlide;
                            if (slide != nullptr) {
                                slide->addItem(*it);
                            }
                            erase = true;
                        } else {
                            if ((*it)->state != ItemState::onBelt) {
                                (*it)->state = ItemState::removed; // TODO: needed, maybe not, but in case?
                                erase = true;
                            }
                        }
                    }
                }
            }
            if (erase) {
            	//(*it)->state = ItemState::removed;
                it = items->erase(it);
            } else {
                it++;
            }
        }
    }
}

void SimConveyorBelt::setDropHandler(ISimDropHandler *dh) {
    this->dh = dh;
}
