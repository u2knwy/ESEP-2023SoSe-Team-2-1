/* 
 * File:   simctrlaction.cpp
 * @author Lehmann
 * @date 15. Mai 2020
 */
#include "simctrlaction.h"
#include <sstream>
#include <iostream>
using namespace std;

SimCtrlAction::SimCtrlAction(const std::string& command) : start(false), restart(false) {
    if (std::string("start") == command) {
        start = true;
    }
    if (std::string("restart") == command) {
        restart = true;
    }
}

bool SimCtrlAction::evalPair(const char* key, const char* value) {
    //cout << "text>" << key << ":" << value << endl;
    bool validPair = false;
    if (string("type") == key) {
        if (string("simctrl") == value) {
            validPair = true; // got valid string
        }
    }
    if (string("action") == key) {
        if (string("nop") == value) {
            validPair = true;
        }
        if (string("start") == value) {
            start = true;
            validPair = true;
        }
        if (string("restart") == value) {
            restart = true;
            validPair = true;
        }
    }
    //cout << "valid:" << (validPair?"T":"F") << endl;
    return validPair;
};

bool SimCtrlAction::evalPair(const char* key, double value) {
    //cout << "nummeric>" << key << ":" << value << endl;
    bool validPair = false;
    // this object has no digits
    return validPair;
};

void SimCtrlAction::setToDefault() {
    start = false;
    restart = false;
};

std::string SimCtrlAction::toJSONString() {
    std::ostringstream result;
    result << "{\"type\":\"simctrl\", ";
    result << "\"action\": ";
    if (!start && !restart) {
        result << "\"nop\"}";
    }
    if (start && !restart) {
        result << "\"start\"}";
    }
    if (!start && restart) {
        result << "\"restart\"}";
    }
    return result.str();
}
