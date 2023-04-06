/* 
 * File:   simulation.cpp
 * @author Lehmann
 * @date 28. März 2020
 */

#include "simulation.h"
#include "simitemhandling.h"
#include "simbase.h"
#include "simconfquery.h"
#include <iostream>
#include <sstream>

using namespace std;

unsigned int Simulation::instancecounter = 0;

#ifdef SIM_PUSHER
Simulation::Simulation(SimItemHandling* itemhandling, SimHCI *hci, SimConfHandler *confhandler, ISimInitCompleteObserver *ich) : feedpusher(&shadow), drive(&shadow), siminitobserver(ich) {
#else
Simulation::Simulation(SimItemHandling* itemhandling, SimHCI *hci, SimConfHandler *confhandler, ISimInitCompleteObserver *ich) : feedseparator(&shadow), drive(&shadow), siminitobserver(ich) {
#endif
    instancenumber = instancecounter++;

    this->confhandler = confhandler;

    oneCycleHasBeenExecuted = false;

    display = new SimShowStates(&image);

    slide = new SimSlide(&slideitems);
#ifdef SIM_PUSHER
    conveyor = new SimConveyorBelt(&conveyoritems, slide, &drive, &feedpusher);
#else
    conveyor = new SimConveyorBelt(&conveyoritems, slide, &drive, &feedseparator);
#endif    
    manager = new SimItemManager(&allitems, conveyor, slide);

    lBBegin = new SimLightBarrier(&conveyoritems, 20.0, &shadow, 0x0001);
    lBHight = new SimLightBarrier(&conveyoritems, 290.0, &shadow, 0x0002);
    lBFeedseparator = new SimLightBarrier(&conveyoritems, 415.0, &shadow, 0x0008);
    lbEnd = new SimLightBarrier(&conveyoritems, 655.0, &shadow, 0x0080);
    lbSlide = new SimLightBarrierSlide(&slideitems, 110.0, &shadow, 0x0040);

    magnetSensor = new SimMagneticSensor(&conveyoritems, 395.0, &shadow, 0x0010);
    heightSensor = new SimHeightSensor(&conveyoritems, 270, &shadow, 0x0004);

    this->itemhandling = itemhandling;
    if (this->itemhandling != nullptr) {
        this->itemhandling->setManger(manager);
    }
    this->hci = hci;
    if (this->hci != nullptr) {
        this->hci->setShadow(&shadow); // Works on shadow becaus its part of the simulation cycle
    }

    init();
};

Simulation::~Simulation() {
    delete display;
    delete conveyor;
    delete manager;
    delete lBBegin;
    delete lBHight;
    delete lBFeedseparator;
    delete lbEnd;
    delete lbSlide;
    delete magnetSensor;
    delete heightSensor;
};

void Simulation::init() {
    //cout << "Init instance number:" << instancenumber << std::endl;
    simTime = 0;
    simLack = 0;
    lBBegin->evalTimeStep(simTime);
    lBHight->evalTimeStep(simTime);
    lBFeedseparator->evalTimeStep(simTime);
    lbEnd->evalTimeStep(simTime);
    lbSlide->evalTimeStep(simTime);
    magnetSensor->evalTimeStep(simTime);
#ifdef SIM_PUSHER
    feedpusher.isModePassing();
#else
    feedseparator.isModePassing();
#endif

    if (itemhandling != nullptr) {
        itemhandling->init();
    }
    if (hci != nullptr) {
        hci->init();
    }
    {
        std::lock_guard<std::mutex> lk(buffermutex);
        image.in = shadow.in;
    }

    for (const auto& ceh : cylceendhandler) {
        ceh->cycleCompletedWith(simTime, shadow, shadow.analog);
    }
    manager->checkRoI();
    show_in();

    if (reporthandler.size() > 0) {
        string report = generateJSONStatusReport() + std::string("\n");
        for (const auto& rh : reporthandler) {
            rh->handlereport(report);
        }

        if (SIMCONFQUERRY_ISACTIVE(showreport)){
        	std::cout << "<Sim> state:" << report << std::endl;
        }
    }

    if (simreleased) { // init only completed if released, so maybe called twice
        last_update_time_high = std::chrono::high_resolution_clock::now();

        if (siminitobserver != nullptr) {
            siminitobserver->initCompleted();
        }
    }
};

void Simulation::writeOut(unsigned short value) {
#if defined(SIM_AUTOSTART_ON_WRITE)
    if(!simreleased) {   // not started yet!
        releaseSimulation();
    }
#endif
    {
        std::lock_guard<std::mutex> lk(buffermutex);
        image.out = value;
    }

};

unsigned short Simulation::readOut() {
    unsigned short result = 0;
    {
        std::lock_guard<std::mutex> lk(buffermutex);
        result = image.out;
    }
    return result;
};

unsigned short Simulation::readIn() {
    unsigned short result = 0;
    {
        std::lock_guard<std::mutex> lk(buffermutex);
        result = image.in;
    }
    return result;
};

void Simulation::doSimulationCycle() {
    std::chrono::time_point<std::chrono::high_resolution_clock> now_high_start;
    now_high_start = std::chrono::high_resolution_clock::now();

    if (SIMCONFQUERRY_ISACTIVE(showtimestamp)){
		if (displayCounter == 0) {
			cout << "<SIM> Time: " << simTime / 1000.0 << " s" << endl;
			displayCounter = SimulationBase::displayEachNCycle - 1;
		} else {
			displayCounter--;
		}
    }

    if (simreleased) {
        {
            std::lock_guard<std::mutex> lk(buffermutex);
            shadow.out = image.out;
        }

        if (confhandler != nullptr){
        	confhandler->evalTime(simTime);
        }

        show_out();

        // update human interactions
        if (hci != nullptr) {
            hci->evalTime(simTime);
        }
        if (itemhandling != nullptr) {
            itemhandling->evalTime(simTime);
        }

        // update actuators
        conveyor->evalTimeStep(simTime);
        slide->evalTimeStep(simTime);

        manager->housekeeping();

        // update sensors
        lBBegin->evalTimeStep(simTime);
        lBHight->evalTimeStep(simTime);
        lBFeedseparator->evalTimeStep(simTime);
        lbEnd->evalTimeStep(simTime);
        lbSlide->evalTimeStep(simTime);
        magnetSensor->evalTimeStep(simTime);
        heightSensor->evalTimeStep(simTime);
        shadow.analog = heightSensor->getADCHeight();

        {
            std::lock_guard<std::mutex> lk(buffermutex);
            image.in = shadow.in;
        }

        for (const auto& ceh : cylceendhandler) {
            ceh->cycleCompletedWith(simTime, shadow, shadow.analog);
        }
        manager->checkRoI();
        show_in();

        if (reporthandler.size() > 0) {
            string report = generateJSONStatusReport() + std::string("\n");
            for (const auto& rh : reporthandler) {
                rh->handlereport(report);
            }

            if (SIMCONFQUERRY_ISACTIVE(showreport)){
            	std::cout << "<Sim> state:" << report << std::endl;
            }
        }
        oneCycleHasBeenExecuted = true;

        if(SIMCONFQUERRY_ISACTIVE(showcycleduration)){
	        std::chrono::time_point<std::chrono::high_resolution_clock> now_high_end;
			now_high_end = std::chrono::high_resolution_clock::now();
			std::chrono::microseconds elapsed_microsec = std::chrono::duration_cast<std::chrono::microseconds> (now_high_end - now_high_start);
			cout << "<SIM> cycle duration:" << elapsed_microsec.count() << endl;
        }
    }
};

void Simulation::show_out() {
    display->showDiffOut();
};

void Simulation::show_in() {
    display->showDiffIn();
};

void Simulation::evaluateRealTimeStep() {
    std::chrono::time_point<std::chrono::high_resolution_clock> now_high;
    now_high = std::chrono::high_resolution_clock::now();

    std::chrono::milliseconds elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds> (now_high - last_update_time_high);

    if (elapsed_milliseconds.count() >= SimulationBase::realTimeSlice) {
        last_update_time_high = now_high;
        simulateTime(SimulationBase::realTimeSlice); // should be elapsed_milleseconds to be more sync to real time.
    }
};

/**
 * Simulate as many time slices that are required to gain the given duration.
 * Remaining time will be executed in the next call of the method. 
 */
void Simulation::simulateTime(unsigned int duration) {
    if (simreleased) {
        simLack = simLack + duration;
        while (simLack >= SimulationBase::timeslice) {
            simLack = simLack - SimulationBase::timeslice;
            simTime = simTime + SimulationBase::timeslice;
            doSimulationCycle();
        }
    }
}

void Simulation::addCycleEndHandler(ISimulationCycleEndHandler* ehd) {
    cylceendhandler.push_back(ehd);

    // send initially last calculation
    ehd->cycleCompletedWith(simTime, shadow, shadow.analog);
}

void Simulation::addReportHandler(ISimulationReportHandler* rh) {
    reporthandler.push_back(rh);
}

void Simulation::setDropHandler(ISimDropHandler *dh) {
    conveyor->setDropHandler(dh);
}

void Simulation::releaseSimulation() {
    if (!simreleased) {
        simreleased = true;
        init(); // re-init simulation.
    }
}
bool Simulation::simulationHasStarted(){
	bool result = false;
	if (simreleased && oneCycleHasBeenExecuted) {
		result = true;
	}
	return result;
}

std::string Simulation::generateJSONStatusReport() {
    stringstream json;
    json << "{";
#ifndef SIM_TWIN_B
    json << "\"id\": \"A\", ";
#else
    json << "\"id\": \"B\", ";
#endif
#ifndef SIM_PUSHER
    json << "\"C\": \"F\", ";
#else
    json << "\"C\": \"P\", ";
#endif
    json << "\"T\": ";
    json << simTime;
    json << ", \"actors\": ";
    json << shadow.out;
    json << ", \"sensors\": ";
    json << shadow.in;
    json << ", \"analog\": ";
    json << shadow.analog;
    json << ", ";
    json << manager->toJSONStringFragment();
    json << "}";

    return json.str();
}
