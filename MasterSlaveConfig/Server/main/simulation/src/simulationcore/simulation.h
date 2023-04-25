/* 
 * File:   simulation.h
 * @author Lehmann
 * @date 28. März 2020
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include "isimulationimage.h"
#include "isimulationexecution.h"
#include "isimulationcycleendhandler.h"
#include "isimdrophandler.h"
#include "isiminitcompleteobserver.h"
#include "isimulationreporthandler.h"
#include "simshowstates.h"
#include "simhci.h"
#include "simsensors.h"
#include "time.h"
#include "simconveyorbelt.h"
#include "simitemmanager.h"
#include "simlightbarrier.h"
#include "simitemhandling.h"
#include "simconfhandler.h"
#include "simmagneticsensor.h"
#include "simheightsensor.h"
#include "simfeedseparator.h"
#include "simfeedpusher.h"
#include "simbase.h"
#include <chrono>
#include <mutex>

class Simulation : public ISimlationExecution, public ISimulationImageAccess {
private:
#if !defined(SIM_TWIN_B) && !defined(SIM_MANUAL_START) && !defined(SIM_AUTOSTART_ON_WRITE)
    bool simreleased = true;
#else
    bool simreleased = false;
#endif
private:
    unsigned long simTime = 0; // [ms]
    unsigned long simLack = 0; // [ms]
    std::chrono::time_point<std::chrono::high_resolution_clock> last_update_time_high;
    int displayCounter = SimulationBase::displayEachNCycle - 1;
private:
    SimulationIOImage image;
    SimulationIOImage shadow;
#ifdef SIM_PUSHER
    SimFeedPusher feedpusher;
#else
    SimFeedSeparator feedseparator;
#endif 
    SimDrive drive;

    SimShowStates* display;
    SimSlide *slide;
    SimConveyorBelt *conveyor;
    SimItemManager *manager;
    SimLightBarrier *lBBegin;
    SimLightBarrier *lBHight;
    SimLightBarrier *lBFeedseparator;
    SimLightBarrier *lbEnd;
    SimLightBarrierSlide *lbSlide;
    SimMagneticSensor *magnetSensor;
    SimHeightSensor *heightSensor;
    SimItemHandling *itemhandling;
    SimHCI* hci;
    SimConfHandler *confhandler;
    vector<shared_ptr < SimItem>> allitems;
    vector<shared_ptr < SimItem>> conveyoritems;
    vector<shared_ptr < SimItem>> slideitems;
    vector< ISimulationCycleEndHandler*> cylceendhandler;
    vector< ISimulationReportHandler*> reporthandler;
    ISimInitCompleteObserver *siminitobserver;
    std::mutex buffermutex;
    unsigned int instancenumber;
    static unsigned int instancecounter;
    bool oneCycleHasBeenExecuted;
public:
    Simulation(SimItemHandling* itemhandling = nullptr, SimHCI *hci = nullptr, SimConfHandler *confhandler=nullptr, ISimInitCompleteObserver *ich = nullptr);
    Simulation(const Simulation&) = delete;
    Simulation& operator=(const Simulation&) = delete;
    virtual ~Simulation();
    void init(); // inits/resets simulation
    /**
     * Polling this method lets the simulaiton only evaluate if a given duration
     * since last simulation step has been expired.
     */
    void evaluateRealTimeStep(); // check if simulation step is required

    //void evaluateSimulatedTime(unsigned int duration); // simulate given time
    /**
     * Method simulates as many time steps which are requrired to simulate
     * the given duration. Lack is preserved and added to the next call.
     * @param duration
     */
    void simulateTime(unsigned int duration) override; // simulate given time in ms

    // I/O-Interfaces
    void writeOut(unsigned short value) override; // write to output 'register'
    unsigned short readOut() override; // read back from output 'register'
    unsigned short readIn() override; // read from input 'register'

    // Show I/O-State
    void show_out();
    void show_in();

    // Handler
    void addCycleEndHandler(ISimulationCycleEndHandler* edh);
    void addReportHandler(ISimulationReportHandler* rh);
    void setDropHandler(ISimDropHandler* dh);

    // start simulation, if in non-autostart mode
    void releaseSimulation();
    bool simulationHasStarted();   // min. one cycle has been executed

    // current simulation time (last or actual simulating)
    unsigned long currentSimTime() {
        return simTime;
    };
private:
    //void updateSimTime();
    void doSimulationCycle(); // timer expired, so update all elements
    std::string generateJSONStatusReport();
};

#endif /* SIMULATION_H */

