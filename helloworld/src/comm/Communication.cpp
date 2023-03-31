/*
 * Communication.cpp
 *
 *  Created on: 31.03.2023
 *      Author: Maik
 */

#include "Communication.h"

#include <iostream>
#include <strings.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/neutrino.h>

Communication::Communication() {
	chid = -1;

}

Communication::~Communication() {
	ChannelDestroy(chid);
}
