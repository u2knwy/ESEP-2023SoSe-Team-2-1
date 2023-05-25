/*
 * Configuration.cpp
 *
 *  Created on: 07.05.2023
 *      Author: Maik
 */

#include "configuration/Configuration.h"
#include "logger/logger.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

Configuration::Configuration() {
}

Configuration::~Configuration() {
}

void Configuration::readConfigFromFile(const std::string filePath) {
	std::ifstream fileStream(filePath);
    if(fileStream.is_open()) {
    	Logger::info("Read config file: " + filePath);
        std::string line;
        while (std::getline(fileStream, line)) {
            std::istringstream iss(line);
            std::string key;
            if(std::getline(iss, key, '=')) {
                std::string value;
                if(std::getline(iss, value)) {
                    Logger::debug(key + " = " + value);
                    if (key == "ORDER") {
                        std::vector<std::string> orders;
                        std::string wpType;
                        std::istringstream tokenStream(value);
                        while (std::getline(tokenStream, wpType, ',')) {
                            orders.push_back(wpType);
                        }
                        std::stringstream ss;
                        for(auto const& wsType: orders) {
                        	ss << " - " << wsType;
                        }
                        Logger::info("Configured workpiece order: " + ss.str());
                    }
                }
            }
        }
    } else {
        Logger::error("Cannot open file: " + filePath);
    }
}

bool Configuration::systemIsMaster() {
	return isMaster;
}

bool Configuration::pusherMounted() {
	return hasPusher;
}

std::vector<WorkpieceType> Configuration::getDesiredOrder() {
	return order;
}
