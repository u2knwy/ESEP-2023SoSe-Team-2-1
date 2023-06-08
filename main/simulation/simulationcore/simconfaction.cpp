/* 
 * File:   SimConfAction.cpp
 * @author Lehmann
 * @date 15. Mai 2020
 */
#include "simconfaction.h"
#include <sstream>
#include <iostream>

using namespace std;

SimConfAction::SimConfAction(const std::string& parameter, bool value) : executed(false), atTime(0), parameter2configure(SimConfCodes::None), value2configure(value) {
    int table_size = SimConfiguration::string_table_size();
    for(int index=0; index<table_size; index++){
        if(parameter == SimConfiguration::SimConfStrings[index]){
            parameter2configure = static_cast<SimConfCodes>(index);
            value2configure = value;
            // ToDo: break
        }
    }
}

SimConfAction::SimConfAction(unsigned int atTime, const std::string& parameter, bool value) : executed(false), atTime(atTime), parameter2configure(SimConfCodes::None), value2configure(value) {
    int table_size = SimConfiguration::string_table_size();
    for(int index=0; index<table_size; index++){
        if(parameter == SimConfiguration::SimConfStrings[index]){
            parameter2configure = static_cast<SimConfCodes>(index);
            value2configure = value;
            // ToDo: break
        }
    }
}

bool SimConfAction::evalPair(const char* key, const char* value) {
    //cout << "text>" << key << ":" << value << endl;
    bool validPair = false;
    if (string("type") == key) {
        if (string("simconfaction") == value) {
            validPair = true; // got valid string
        }
    }
    if (string("parameter") == key) {
        unsigned int table_size = SimConfiguration::string_table_size();
        for(unsigned int index=0; index<table_size; index++){
            std::string temp = SimConfiguration::SimConfStrings[index];
            if(temp == value){
                parameter2configure = static_cast<SimConfCodes>(index);
                validPair = true; // got valid string
            }
        }
    }
    if (string("value") == key) {
       if (std::string("true") == value) {
            value2configure = true;
            validPair = true; // got valid string
        }
        if (std::string("false") == value) {
            value2configure = false;
            validPair = true; // got valid string
        }
    }
    //cout << "valid:" << (validPair?"T":"F") << endl;
    return validPair;
};

bool SimConfAction::evalPair(const char* key, double value) {
    //cout << "nummeric>" << key << ":" << value << endl;
    bool validPair = false;
    if (std::string("atTime") == key) {
        atTime = (int) value;
        validPair = true;
    }

    return validPair;
};

void SimConfAction::setToDefault() {
    value2configure = false;
    parameter2configure = SimConfCodes::None;
};

std::string SimConfAction::toJSONString() {
    std::ostringstream result;
    result << "{\"type\": \"simconfaction\", ";
    result << "\"atTime\": " << atTime << ", ";
    result << "\"parameter\": \"";
    result << SimConfiguration::SimConfStrings[static_cast<int>(parameter2configure)];
    result << "\", \"value\": ";
    if(value2configure){
        result << "true";
    }
    else {
        result << "false";
    }
    result << "}";

    return result.str();
}
