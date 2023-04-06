/* 
 * File:   simshowstates.cpp
 * @author Lehmann
 * @date 29. März 2020
 */

#include "simshowstates.h"
#include "simmasks.h"
#include "simconfquery.h"

#include <iostream>

using namespace std;

void SimShowStates::showFullOut() {
    unsigned short out = state->out;
    unsigned diff = 0x0fff;

    showOut(out, diff);

    last.out = state->out;
};

void SimShowStates::showDiffOut() {
    unsigned short out = state->out;
    unsigned diff = out ^ last.out;

    showOut(out, diff);

    last.out = state->out;
}

void SimShowStates::showFullIn() {
    unsigned short in = state->in;
    unsigned diff = 0x0fff;

    showIn(in, diff);

    last.in = state->in;
};

void SimShowStates::showDiffIn() {
    unsigned short in = state->in;
    unsigned diff = in ^ last.in;

    showIn(in, diff);

    last.in = state->in;
}

void SimShowStates::showOut(unsigned short out, unsigned short diff) {
    unsigned short drive_mask = SIM_DRIVE_DIRECTION_RIGHT | SIM_DRIVE_DIRECTION_LEFT | SIM_DRIVE_SLOW | SIM_DRIVE_STOP;

    if (SIMCONFQUERRY_ISACTIVE(showactuators)) {
    	if ((diff & drive_mask) != 0) {
			if ((out & drive_mask) == 0 || (out & SIM_DRIVE_STOP) == SIM_DRIVE_STOP) {
				cout << "<SIM> Belt stopped" << endl;
			} else {
				if (out & SIM_DRIVE_DIRECTION_RIGHT) {
					cout << "<SIM> Belt moves right";
				}
				if (out & SIM_DRIVE_DIRECTION_LEFT) {
					cout << "<SIM> Belt moves left";
				}
				if (out & SIM_DRIVE_SLOW) {
					cout << ", slow" << endl;
				} else {
					cout << ", fast" << endl;

				}
			}
		}
		if (diff & SIM_FEED_SEPARATOR) {
			if (out & SIM_FEED_SEPARATOR) {
				cout << "<SIM> Feeder: commanded open" << endl;
			} else {
				cout << "<SIM> Feeder: commanded closed" << endl;
			}
		}
		if (diff & SIM_ALARM_LAMP_RED) {
			if (out & SIM_ALARM_LAMP_RED) {
				cout << "<SIM> Red:    on" << endl;
			} else {
				cout << "<SIM> Red:    off" << endl;
			}
		}
		if (diff & SIM_ALARM_LAMP_YELLOW) {
			if (out & SIM_ALARM_LAMP_YELLOW) {
				cout << "<SIM> Yellow: on" << endl;
			} else {
				cout << "<SIM> Yellow: off" << endl;
			}
		}
		if (diff & SIM_ALARM_LAMP_GREEN) {
			if (out & SIM_ALARM_LAMP_GREEN) {
				cout << "<SIM> Green:  on" << endl;
			} else {
				cout << "<SIM> Green:  off" << endl;
			}
		}
		if (diff & SIM_LED_START_BUTTON) {
			if (out & SIM_LED_START_BUTTON) {
				cout << "<SIM> LED start: on" << endl;
			} else {
				cout << "<SIM> LED start: off" << endl;
			}
		}
		if (diff & SIM_LED_RESET_BUTTON) {
			if (out & SIM_LED_RESET_BUTTON) {
				cout << "<SIM> LED Reset: on" << endl;
			} else {
				cout << "<SIM> LED Reset: off" << endl;
			}
		}
		if (diff & SIM_LED_Q1) {
			if (out & SIM_LED_Q1) {
				cout << "<SIM> LED Q1:    on" << endl;
			} else {
				cout << "<SIM> LED Q1:    off" << endl;
			}
		}
		if (diff & SIM_LED_Q2) {
			if (out & SIM_LED_Q2) {
				cout << "<SIM> LED Q2:    on" << endl;
			} else {
				cout << "<SIM> LED Q2:    off" << endl;
			}
		}
    }
};

void SimShowStates::showIn(unsigned short in, unsigned short diff) {

	if (SIMCONFQUERRY_ISACTIVE(showsensors)) {
		if (diff & SIM_ITEM_DETECTED) {
			if (in & SIM_ITEM_DETECTED) {
				cout << "<SIM> LB Begin: closed" << endl;
			} else {
				cout << "<SIM> LB Begin: interrupted" << endl;
			}
		}
		if (diff & SIM_ITEM_AT_HEIGHT_SENSOR) {
			if (in & SIM_ITEM_AT_HEIGHT_SENSOR) {
				cout << "<SIM> LB Height: closed" << endl;
			} else {
				cout << "<SIM> LB Height: interrupted" << endl;
			}
		}
		if (diff & SIM_ITEM_AT_JUNCTION) {
			if (in & SIM_ITEM_AT_JUNCTION) {
				cout << "<SIM> LB Feed Separator: closed" << endl;
			} else {
				cout << "<SIM> LB Feed Separator: interrupted" << endl;
			}
		}
		if (diff & SIM_BUFFER_IS_FULL) {
			if (in & SIM_BUFFER_IS_FULL) {
				cout << "<SIM> LB Slide: closed" << endl;
			} else {
				cout << "<SIM> LB Slide: interrupted" << endl;
			}
		}
		if (diff & SIM_ITEM_AT_END) {
			if (in & SIM_ITEM_AT_END) {
				cout << "<SIM> LB End: closed" << endl;
			} else {
				cout << "<SIM> LB End: interrupted" << endl;
			}
		}
		if (diff & SIM_ITEM_HEIGHT_FITS) {
			if (in & SIM_ITEM_HEIGHT_FITS) {
				cout << "<SIM> Height okay" << endl;
			} else {
				cout << "<SIM> Height not okay" << endl;
			}
		}
		if (diff & SIM_ITEM_IS_METTAL) {
			if (in & SIM_ITEM_IS_METTAL) {
				cout << "<SIM> Metal detected" << endl;
			} else {
				cout << "<SIM> Metal not detected" << endl;
			}
		}
		if (diff & SIM_JUNCTION_IS_OPEN) {
			if (in & SIM_JUNCTION_IS_OPEN) {
				cout << "<SIM> Feed Separator: pass" << endl;
			} else {
				cout << "<SIM> Feed Separator: slide" << endl;
			}
		}

		if (diff & SIM_BUTTON_START) {
			if (in & SIM_BUTTON_START) {
				cout << "<SIM> Button Start: pressed" << endl;
			} else {
				cout << "<SIM> Button Start: released" << endl;
			}
		}
		if (diff & SIM_BUTTON_STOP) {
			if (in & SIM_BUTTON_STOP) {  // active low
				cout << "<SIM> Button Stop: released" << endl;
			} else {
				cout << "<SIM> Button Stop: pressed" << endl;
			}
		}
		if (diff & SIM_BUTTON_RESET) {
			if (in & SIM_BUTTON_RESET) {
				cout << "<SIM> Button Reset: pressed" << endl;
			} else {
				cout << "<SIM> Button Reset: released" << endl;
			}
		}
		if (diff & SIM_EMERGENCY_STOP) {
			if (in & SIM_EMERGENCY_STOP) {  // active low
				cout << "<SIM> Emergency: not active" << endl;
			} else {
				cout << "<SIM> Emergency: active" << endl;
			}
		}
	}
}
