
/* 
 * File:   simhciaction.cpp
 * @author Lehmann
 * @date 9. Mai 2020
 */

#include "simhciaction.h"
#include "simmasks.h"

#include <string>
#include <sstream>

SimHCIAction::SimHCIAction(unsigned int atTime) : executed(false), atTime(atTime), pattern(SIM_BUTTON_STOP | SIM_EMERGENCY_STOP) {
}

SimHCIAction::SimHCIAction(unsigned int atTime, SimHCIActionKind kind) : executed(false), atTime(atTime), pattern(SIM_BUTTON_STOP | SIM_EMERGENCY_STOP) {
    switch (kind) {
        case SimHCIActionKind::pressStartOnly:
            pressStart();
            break;
        case SimHCIActionKind::pressStopOnly:
            pressStop();
            break;
        case SimHCIActionKind::pressResetOnly:
            pressReset();
            break;
        case SimHCIActionKind::pressEStopOnly:
            pressEStop();
            break;
        case SimHCIActionKind::releaseAll:
            // nothing to be done, default state
            break;
        default:
            ;
    }
}

void SimHCIAction::releaseAll() {
    pattern = SIM_BUTTON_STOP | SIM_EMERGENCY_STOP;
}

void SimHCIAction::pressStart() {
    pattern = pattern | SIM_BUTTON_START;
}

void SimHCIAction::pressStop() {
    pattern = pattern & (~SIM_BUTTON_STOP);
}

void SimHCIAction::pressReset() {
    pattern = pattern | SIM_BUTTON_RESET;
}

void SimHCIAction::pressEStop() {
    pattern = pattern & (~SIM_EMERGENCY_STOP);
}

void SimHCIAction::releaseStart() {
    pattern = pattern & (~SIM_BUTTON_START);
}

void SimHCIAction::releaseStop() {
    pattern = pattern | SIM_BUTTON_STOP;
}

void SimHCIAction::releaseReset() {
    pattern = pattern & (~SIM_BUTTON_RESET);
}

void SimHCIAction::releaseEStop() {
    pattern = pattern | SIM_EMERGENCY_STOP;
}

void SimHCIAction::setToDefault() {
    executed = false;
    atTime = 0;
    pattern = SIM_BUTTON_STOP | SIM_EMERGENCY_STOP;
}

bool SimHCIAction::evalPair(const char* key, const char* value) {
    //cout << "text>" << key << ":" << value << endl;
    bool validPair = false;
    if (std::string("type") == key) {
        if (std::string("hciaction") == value) {
            validPair = true;   // got valid string
        }        
    }
    //cout << "valid:" << (validPair?"T":"F") << endl;
    return validPair;
};

bool SimHCIAction::evalPair(const char* key, double value) {
    //cout << "nummeric>" << key << ":" << value << endl;
    bool validPair = false;
    if (std::string("atTime") == key) {
        atTime = (int) value;
        validPair = true;
    }
    if (std::string("pattern") == key) {
        pattern = SIM_BUTTON_STOP | SIM_EMERGENCY_STOP;;
        if(((int)value) & 0x01){
           pattern = pattern | SIM_BUTTON_START;
        };
        if(((int)value) & 0x02){  // active low;
           pattern = pattern & (~SIM_BUTTON_STOP);
        };
        if(((int)value) & 0x04){
           pattern = pattern | SIM_BUTTON_RESET;
        };
        if(((int)value) & 0x08){  // active low
           pattern = pattern & (~SIM_EMERGENCY_STOP);
        };
        
        validPair = true;
    }
    return validPair;
};

std::string SimHCIAction::toJSONString() {
    std::ostringstream result;
    result << "{\"type\":\"hciaction\", ";
    result << "\"atTime\": " << atTime << ", ";
    result << "\"pattern\": ";

    int patternvalue = 0;
    if ((pattern & SIM_BUTTON_START) == SIM_BUTTON_START) { // Button is active
        patternvalue = 1;
    }

    if ((pattern & SIM_BUTTON_STOP) == 0) { // Button is active low
        patternvalue = patternvalue | 0x02;
    }

    if ((pattern & SIM_BUTTON_RESET) == SIM_BUTTON_RESET) { // Button is active
        patternvalue = patternvalue | 0x04;
    }

    if ((pattern & SIM_EMERGENCY_STOP) == 0) { // Button is active low
        patternvalue = patternvalue | 0x08;
    }

    result << patternvalue << "}";

    return result.str();
}

