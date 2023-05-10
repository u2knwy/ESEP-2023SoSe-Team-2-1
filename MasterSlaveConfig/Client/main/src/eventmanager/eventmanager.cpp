/*
 * eventmanager.cpp
 *
 *  Created on: 10.05.2023
 *      Author: domxs
 */

#include "eventmanager.h"


int eventmanager::sendMessagePayload(EventType event, char* payload) { // Client side of the cod


    header_t     header;
    app_header_t app_header;
    iov_t        iov[3]; // multi part msg

    char  *payload1 = "intern Payload ...";

    char  r_msg[512]; // buffer for the answer
    	printf("Client sending msg + channel %i \n", server_coid);
    	int payload_size = strlen(payload)+1;   // +1 due to \0 at the end of a string
    	// Compose the msg using an IOV
    	header.type = STR_MSG;    // define msg type
    	header.subtype = 0x00;

    	app_header.size = payload_size;	// fill application header
    	app_header.count = 777;
    	app_header.eventnr = event;

    	SETIOV(iov+0, &header, sizeof(header));
    	SETIOV(iov+1, &app_header, sizeof(app_header));
    	SETIOV(iov+2, payload, payload_size);

        // send msg
    	if (-1 == MsgSendvs(server_coid, iov, 3, r_msg, sizeof(r_msg))){
    		perror("Client: MsgSend failed");
    		printf("potatoid %i \n", server_coid);
    	    exit(EXIT_FAILURE);
    	}
    	// Answer form server should be structured, too.
    	// Did not check for buffer overflow before printing r_msg
    	printf("Client: Answer from server: %s \n", r_msg);
    	sleep(2);

    return EXIT_SUCCESS;
}
