#include <iostream>
#include <chrono>
#include <thread>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include "hal/hal.h"
#include "eventmanager/eventmanager.h"



int main(int argc, char *argv[])
{

//	hal = std::make_shared<HAL>();
	int ret = -1;
	printf("Running Server 1 ... \n");
	EventManager myEventMgr;
	ret = myEventMgr.start();
    //print out message
    printf("Server stopped. \n");
    return ret;
}
