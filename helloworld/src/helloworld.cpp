/* Simple demostration of the simulator usage
 * @author: Franz Korf
 * @author: Thomas Lehmann
 * @date: 2020-04-24
 */
#include <iostream>
#include <chrono>
#include <thread>
#include <strings.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/neutrino.h>

#include "hal/hal.h"
#include "comm/Communication.h"
#include "utils/cxxopts.hpp"

/* Für die Simulation: */
// #include "../simulationqnx/simqnxgpioapi.h" // must be last include !!!

using namespace std;

void gpioDemo() {
	cout << "Switch traffic light on / off" << endl;

	// Create a shared pointer which holds a reference to the HAL.
	// This can be shared across multiple classes and the destructor will automatically be called,
	// if there are no more references to it.
	shared_ptr<HAL> hal = std::make_shared<HAL>();

#define RUNS 10
	for(int i = 0; i < RUNS; i++){
		cout << "loop " << i+1 << "/" << RUNS << endl;
		hal->GreenLampOn();
        usleep(1000*500);

        hal->GreenLampOff();
        usleep(1000*500);
	}
}

int chid = -1;

void server() {
	int rcvid;
	char msg[256];

	chid = ChannelCreate(0);

	using namespace std;
	std::cout << "Server started - channel ID " << chid << endl;
	while(1) {
		rcvid = MsgReceive(chid, msg, sizeof(msg), NULL);
		cout << "Server: got the message: " << msg << endl;
		strcpy(msg, "Got the message");
		MsgReply(rcvid, EOK, msg, sizeof(msg));
	}
}

void client() {
	int node_id = 0;
	pid_t pid = 0;
	int coid;
	char *s_msg = "My first message";
	char r_msg[512];

	using namespace std;
	cout << "Client started - connecting to channel " << chid << endl;
	do {
		coid = ConnectAttach(node_id, pid, chid, 0, 0);
		if (coid == -1) {
			cout << "Client: ConnectAttach failed" << endl;
			exit(EXIT_FAILURE);
		}
	} while (coid == -1);

	cout << "Client connected to channel" << endl;

	if (MsgSend(coid, s_msg, strlen(s_msg)+1, r_msg, sizeof(r_msg)) == -1) {
		cout << "Error during MsgSend" << endl;
		ConnectDetach(coid);
		exit(EXIT_FAILURE);
	}
	cout << "Client: Reply from Server: " << r_msg << endl;
}

void gnsDemo() {
	std::thread th_server(server);
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	std::thread th_client(client);
	th_client.join();
	th_server.join();
}

int main(int argc, char *argv[]) {
	cxxopts::Options options("Sorting Machine", "Demo Project for ESEP project");
	options.add_options()
			("m,mode", "Mode master or slave", cxxopts::value<std::string>());
	auto result = options.parse(argc, argv);
	cout << result["m"].as<std::string>() << endl;

	//gpioDemo();
	gnsDemo();

	return 0;
}
