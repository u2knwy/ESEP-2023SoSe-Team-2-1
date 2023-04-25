#include <iostream>
#include <chrono>
#include <thread>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "events/eventtypes_enum.h"
#include "hal/hal.h"
#include <sys/iofunc.h>
#include <sys/neutrino.h>
#include <sys/dispatch.h>

#define NO_OF_MSGS			6

#define ATTACH_POINT 		"/dev/name/global/myservice"

/* Fist header used by GNS and application */
typedef struct _pulse header_t;

/* header types
 *  0 .. _IO_BASE -1 						: Pulse Messages
 * 	_IO_BASE = 0x100 .. _IO_MAX = 0x1FF 	: Sync Messages of the System
 * 	_IO_MAX + 1 ..							: Sync Messages of our application
 */

// Messages types of our application
#define STR_MSG   (_IO_MAX + 1)
#define DATA_MSG  (_IO_MAX + 2)




/* Second header: used by application - if required */
typedef struct {
	int size; // size of data block
	int count;
	EventType eventnr;// some counter used by the application
	// further data fields required by our application
} app_header_t;


int client() { // Client side of the code

    int server_coid;

    char  *payload0 = "Dies ist ein sehr grosser Datenbereich ...";
    char  *payload1 = "Eine andere Payload ...";
    header_t     header;
    app_header_t app_header;
    iov_t        iov[3]; // multi part msg

    char  r_msg[512]; // buffer for the answer

    // Use GNS for setting up the connection to the server - running somewhere in the network
    if ((server_coid = name_open(ATTACH_POINT, NAME_FLAG_ATTACH_GLOBAL)) == -1) {
    	perror("Client: name_open failed, check server");
        return EXIT_FAILURE;
    }
    printf("connected to server\n");

    // Do whatever work you wanted with server connection
    for (int no = 0; no < NO_OF_MSGS; no++) {
    	printf("Client sending msg %d \n", no);

    	// Construct payload
    	char * payload = ((no % 2) == 0) ? payload0 : payload1; // use dummy values as payload
    	int payload_size = strlen(payload)+1;   // +1 due to \0 at the end of a string
    	EventType eventnr1[NO_OF_MSGS] = {EventType::HALgrueneLampeAn, EventType::HALgelbeLampeAn, EventType::HALroteLampeAn, EventType::HALgrueneLampeAus ,EventType::HALgelbeLampeAus, EventType::HALroteLampeAus};

    	// Compose the msg using an IOV
    	header.type = STR_MSG;    // define msg type
    	header.subtype = 0x00;

    	app_header.size = payload_size;	// fill application header
    	app_header.count = no;
    	app_header.eventnr = eventnr1[no];

    	SETIOV(iov+0, &header, sizeof(header));
    	SETIOV(iov+1, &app_header, sizeof(app_header));
//    	SETIOV(iov+2, payload, payload_size);

        // send msg
    	if (-1 == MsgSendvs(server_coid, iov, 3, r_msg, sizeof(r_msg))){
    		perror("Client: MsgSend failed");
    	    exit(EXIT_FAILURE);
    	}
    	// Answer form server should be structured, too.
    	// Did not check for buffer overflow before printing r_msg
    	printf("Client: Answer from server: %s \n", r_msg);
    	sleep(2);
    }
    // close connection
    name_close(server_coid);
    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
	std::shared_ptr<HAL> hal = std::make_shared<HAL>();
    printf("gns must be running.\n");

        printf("Running Client ... \n");
        int ret;
        ret = client();
    return ret;
}

