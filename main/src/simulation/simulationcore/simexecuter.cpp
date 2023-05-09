
/* 
 * File:   simexecutionthread.cpp
 * @author Lehmann
 * @date 6. April 2020
 */

#include "simexecuter.h"
#include <chrono>
#include <thread>

#include <iostream>
using namespace std;

void SimulationExecuter::operator()() {
    if (simulation != nullptr && timeslice > 0) {
        std::chrono::time_point<std::chrono::high_resolution_clock> lastUpdateTimeHigh;
        std::chrono::time_point<std::chrono::high_resolution_clock> nowHigh;

        lastUpdateTimeHigh = std::chrono::high_resolution_clock::now();
        while (run) {
            std::this_thread::sleep_for(std::chrono::milliseconds(timeslice));
            nowHigh = std::chrono::high_resolution_clock::now();
            int elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds> (nowHigh - lastUpdateTimeHigh).count();
            lastUpdateTimeHigh = nowHigh;
            //cout << "call with:" << elapsed_milliseconds << endl;
            simulation->simulateTime(elapsed_milliseconds);
        }
    }
}
