/*
 * IThreadRunner.h
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#pragma once

#include <thread>

using namespace std;

class IThreadRunner {
public:
	virtual ~IThreadRunner() {}
	virtual void start() = 0;
	virtual void stop() = 0;

protected:
    bool running{false};
    std::thread runningThread;

    virtual void runningFunction() = 0;
};
