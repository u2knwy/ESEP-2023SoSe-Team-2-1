/*
 * simconf.cpp
 *
 *  Created on: 08.02.2023
 *      Author: Lehmann
 */

#include "simconf.h"

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#define SIMCONF(x) #x,

const char* SimConfiguration::SimConfStrings[] = {
	#include "simconfparameter.h"
	"None"
};
#undef SIMCONF

int SimConfiguration::string_table_size(){
    int len = 0;

#define SIMCONF(x) len++;
#include "simconfparameter.h"
#undef SIMCONF

    return len;
}

SimConfiguration::SimConfiguration(){
    // fast access
#define SIMCONF(X) config_parameter.push_back(false);
#include "simconfparameter.h"
#undef SIMCONF
    // name resolving
#define SIMCONF(X) config_names[#X] = SimConfCodes::X;
#include "simconfparameter.h"
#undef SIMCONF

    setToDefault();
}

SimConfiguration* SimConfiguration::getInstance(){
    static SimConfiguration configuration;
    return &configuration;
}

bool SimConfiguration::isactive(SimConfCodes parametercode){
	int index = static_cast<int>(parametercode);
	bool querryResult = config_parameter[index];
    return querryResult;
}

bool SimConfiguration::isactive(std::string parametername){
    return config_parameter[static_cast<int>(config_names[parametername])];
}

void SimConfiguration::activate(SimConfCodes parametercode){
	int index = static_cast<int>(parametercode);
    config_parameter[index] = true;
}

void SimConfiguration::deactivate(SimConfCodes parametercode){
	int index = static_cast<int>(parametercode);
    config_parameter[index] = false;
}

void SimConfiguration::dumpConfig(){
    for (const auto& element : config_names) {
        std::cout << '[' << element.first << "] : ";
        if (config_parameter[static_cast<int>(element.second)] > 0){
        	std::cout << "on" << std::endl;
        } else {
        	std::cout << "off" << std::endl;
        }
    }
}

std::string SimConfiguration::toJSONString(){
    std::ostringstream result;
    unsigned int counter = 0;

    result << "{\"type\":\"simconfig\", ";

    for (const auto& element : config_names) {
        counter ++;
        result << '\"' << element.first << "\": ";
        if(config_parameter[static_cast<int>(element.second)]){
            result << "true";
        }
        else {
            result << "false";
        }
        if(counter != config_names.size()){
            result << ", ";
        }
    }

    result << "}";

    return result.str();
};

bool SimConfiguration::configSetTo(std::string parameter, bool value){
	bool valid_parameter = false;

	if(config_names.count(parameter)==1) {
		valid_parameter= true; // got valid string
		config_parameter[static_cast<int>(config_names[parameter])] = value;
	}
	return valid_parameter;
}

bool SimConfiguration::evalPair(const char* key, const char* value){
    std::cout << key << " : " << value << std::endl;
    bool validPair = false;

    if (std::string("type") == key) {
        if (std::string("simconfig") == value) {
            validPair = true; // got valid string
        }
    } else {
        if(config_names.count(key)==1) {
            if (std::string("true") == value) {
                config_parameter[static_cast<int>(config_names[key])] = true;
                validPair = true; // got valid string
            }
            if (std::string("false") == value) {
                config_parameter[static_cast<int>(config_names[key])] = false;
                validPair = true; // got valid string
            }
        } else {
            std::cout << "<SimConfig> Error, unknown parameter: " << key << std::endl;
        }
    }
    return validPair;
}

bool SimConfiguration::evalPair(const char* key, double value){
    return false;
}

void SimConfiguration::setToDefault(){
	// following must be hard-coded
#ifdef SIM_SHOW_ACTIONS
	activate(SimConfCodes::showactions);
#endif
#ifdef SIM_SHOW_CYCLE_DURATION
	activate(SimConfCodes::showcycleduration);
#endif
#ifdef SIM_SHOW_SENSORS
	activate(SimConfCodes::showsensors);
#endif
#ifdef SIM_SHOW_ACTUATORS
	activate(SimConfCodes::showactuators);
#endif
#ifdef SIM_SHOW_POSITIONS
	activate(SimConfCodes::showpositions);
#endif
#ifdef SIM_SHOW_TIMESTAMP
	activate(SimConfCodes::showtimestamp);
#endif
#ifdef SIM_SHOW_ROI
	activate(SimConfCodes::showroi);
#endif
#ifdef SIM_SHOW_REPORT
	activate(SimConfCodes::showreport);
#endif

	dumpConfig();
	readFromFile("/simconf.json");
	writeToFile("/simconf.init.json");
}

void  SimConfiguration::writeToFile(std::string configfilename){
    std::ofstream configfile(configfilename, std::ofstream::out);
    if(configfile.is_open()){
        std::string outputdata = toJSONString();
        configfile.write(outputdata.c_str(), outputdata.length());
        configfile.close();
    }
}

void  SimConfiguration::readFromFile(std::string configfilename){
    std::ifstream configfile(configfilename, std::ofstream::in);
    if(configfile.is_open()){
        std::stringstream buffer;
        buffer << configfile.rdbuf();
        configfile.close();
        std::cout << buffer.str() << std::endl;
        parseJSON(buffer.str());
    }
}
