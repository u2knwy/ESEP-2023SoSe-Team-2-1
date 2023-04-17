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

#include "hal.h"
#include "utils/cxxopts.hpp"

#include "interrupts.hpp"

/* Für die Simulation: */
// #include "../simulationqnx/simqnxgpioapi.h" // must be last include !!!

using namespace std;

void actuatorDemo() {
	std::cout << "Actuator Demo" << std::endl;
	uintptr_t gpio_bank_0 = mmap_device_io(GPIO_SIZE, (uint64_t) GPIO_BANK_0);
	uintptr_t gpio_bank_1 = mmap_device_io(GPIO_SIZE, (uint64_t) GPIO_BANK_1);
	uintptr_t gpio_bank_2 = mmap_device_io(GPIO_SIZE, (uint64_t) GPIO_BANK_2);
	uintptr_t gpio_bank_3 = mmap_device_io(GPIO_SIZE, (uint64_t) GPIO_BANK_3);

	out32((uintptr_t) (gpio_bank_1 + GPIO_SETDATAOUT), LAMP_GREEN_PIN);
	out32((uintptr_t) (gpio_bank_2 + GPIO_SETDATAOUT), (1 << 2));
	std::this_thread::sleep_for(std::chrono::seconds(3));
	out32((uintptr_t) (gpio_bank_1 + GPIO_CLEARDATAOUT), LAMP_GREEN_PIN);
	out32((uintptr_t) (gpio_bank_2 + GPIO_CLEARDATAOUT), (1 << 2));

	munmap_device_io(gpio_bank_0, GPIO_SIZE);
	munmap_device_io(gpio_bank_1, GPIO_SIZE);
	munmap_device_io(gpio_bank_2, GPIO_SIZE);
	munmap_device_io(gpio_bank_3, GPIO_SIZE);
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

	sensorDemo();
	//actuatorDemo();
	//gnsDemo();

	return 0;
}
