/*
 * Configuration.cpp
 *
 *  Created on: 07.05.2023
 *      Author: Maik
 */

#include "configuration/Configuration.h"
#include "logger/logger.hpp"
#include "hal/IHeightSensor.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

Configuration::Configuration() {
	this->cal = Calibration{ .calOffset = ADC_DEFAULT_OFFSET, .calRef = ADC_DEFAULT_HIGH };
	this->configFilePath = std::string(DEFAULT_CONFIG_FILE_PATH);
}

Configuration::~Configuration() {
}

void Configuration::setConfigFilePath(std::string filePath) {
	this->configFilePath = filePath;
}

void Configuration::readConfigFromFile() {
	std::ifstream fileStream(configFilePath);
	if(fileStream.is_open()) {
		Logger::info("Read config file: " + configFilePath);
		std::string line;
		while (std::getline(fileStream, line)) {
			std::istringstream iss(line);
			std::string key;
			if(std::getline(iss, key, '=')) {
				std::string value;
				if(std::getline(iss, value)) {
					Logger::debug(key + " = " + value);
					if (key == "ORDER") {
						std::vector<WorkpieceType> workpieceOrder;
						std::string wpType;
						std::istringstream tokenStream(value);
						while (std::getline(tokenStream, wpType, ',')) {
							if(wpType == "F") {
								workpieceOrder.push_back(WorkpieceType::WS_F);
							} else if(wpType == "BOM") {
								workpieceOrder.push_back(WorkpieceType::WS_BOM);
							}
							else if(wpType == "BUM") {
								workpieceOrder.push_back(WorkpieceType::WS_BUM);
							}
							else if(wpType == "OB") {
								workpieceOrder.push_back(WorkpieceType::WS_OB);
							}
						}
						std::stringstream ss;
						for(auto const& wsType: workpieceOrder) {
							ss << " - " << wsType;
						}
						Logger::info("Configured workpiece order: " + ss.str());
					} else if(key == "CAL_OFFSET") {
						cal.calOffset = std::stoi(value);
					} else if(key == "CAL_REF") {
						cal.calRef = std::stoi(value);
					}
				}
			}
		}
		Logger::info("Cal. Offset: " + std::to_string(cal.calOffset));
		Logger::info("Cal. Ref: " + std::to_string(cal.calRef));
	} else {
		Logger::warn("Config file " + configFilePath + " does not exist -> create new and write default values");
		std::ofstream fileStream;
		fileStream.open(configFilePath);
		fileStream << "ORDER=F,OB,BOM\n";
		fileStream << "CAL_OFFSET=" << ADC_DEFAULT_OFFSET << "\n";
		fileStream << "CAL_REF=" << ADC_DEFAULT_HIGH << "\n";
		fileStream.close();
	}
}

void Configuration::setMaster(bool isMaster) {
	this->isMaster = isMaster;
}

bool Configuration::systemIsMaster() {
	return isMaster;
}

void Configuration::setPusherMounted(bool pusherIsMounted) {
	this->hasPusher = pusherIsMounted;
}

bool Configuration::pusherMounted() {
	return hasPusher;
}

void Configuration::setDesiredWorkpieceOrder(std::vector<WorkpieceType> order) {
	this->order = order;
}

std::vector<WorkpieceType> Configuration::getDesiredOrder() {
	return order;
}

void Configuration::setOffsetCalibration(int offset) {
	cal.calOffset = offset;
}

void Configuration::setReferenceCalibration(int refHigh) {
	cal.calRef = refHigh;
}

Calibration Configuration::getCalibration() {
	return cal;
}

void Configuration::saveCalibrationToFile() {
	// TODO: Save current config to file
	writeLineToConfigFile(2, "CAL_OFFSET=" + std::to_string(cal.calOffset));
	writeLineToConfigFile(3, "CAL_REF=" + std::to_string(cal.calRef));
}

void Configuration::writeLineToConfigFile(int lineNumber, const std::string& newContent) {
	std::ifstream inputFile(configFilePath);
	if (!inputFile) {
		Logger::error("Error opening config file.");
		return;
	}

	const std::string& tempFilePath = configFilePath + ".tmp";

	std::ofstream tempFile(tempFilePath);
	if (!tempFile) {
		Logger::error("Error opening temporary file.");
		inputFile.close();
		return;
	}

	std::string line;
	int currentLine = 1;
	while (std::getline(inputFile, line)) {
		if (currentLine == lineNumber) {
			tempFile << newContent << std::endl;
		} else {
			tempFile << line << std::endl;
		}
		currentLine++;
	}

	inputFile.close();
	tempFile.close();

    if (std::remove(configFilePath.c_str()) != 0) {
    	Logger::error("Error deleting input file.");
        return;
    }

    if (std::rename(tempFilePath.c_str(), configFilePath.c_str()) != 0) {
    	Logger::error("Error renaming temporary file.");
        return;
    }

    Logger::info("Updated line " + std::to_string(lineNumber) + " in config file -> " + newContent);
}
