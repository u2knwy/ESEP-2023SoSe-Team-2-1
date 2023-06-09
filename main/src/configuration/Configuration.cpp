/*
 * Configuration.cpp
 *
 *  Created on: 07.05.2023
 *      Author: Maik
 */

#include "configuration/Configuration.h"
#include "hal/IHeightSensor.h"
#include "logger/logger.hpp"


#include <fstream>
#include <iostream>
#include <string>
#include <vector>


Configuration::Configuration() {
    this->cal = Calibration{.calOffset = ADC_DEFAULT_OFFSET,
                            .calRef = ADC_DEFAULT_HIGH};
    this->configFilePath = std::string(DEFAULT_CONFIG_FILE_PATH);
    this->order = {WS_F, WS_BUM, WS_OB };
}

Configuration::~Configuration() {}

void Configuration::setConfigFilePath(std::string filePath) {
    this->configFilePath = filePath;
}

bool Configuration::readConfigFromFile() {
    bool readResult = false;

    std::vector<std::string> errors;
    std::ifstream fileStream(configFilePath);
    if (fileStream.is_open()) {
        Logger::info("Read config from file: " + configFilePath);
        std::vector<WorkpieceType> workpieceOrder;
        std::string line;
        while (std::getline(fileStream, line)) {
            std::istringstream iss(line);
            std::string key;
            if (std::getline(iss, key, '=')) {
                std::string value;
                if (std::getline(iss, value)) {
                    if (key == "ORDER") {
                        std::string wpType;
                        std::istringstream tokenStream(value);
                        while (std::getline(tokenStream, wpType, ',')) {
                            if (wpType == "F") {
                                workpieceOrder.push_back(WorkpieceType::WS_F);
                            } else if (wpType == "BOM") {
                                workpieceOrder.push_back(WorkpieceType::WS_BOM);
                            } else if (wpType == "BUM") {
                                workpieceOrder.push_back(WorkpieceType::WS_BUM);
                            } else if (wpType == "OB") {
                                workpieceOrder.push_back(WorkpieceType::WS_OB);
                            } else {
                                errors.push_back(
                                    "Unknown workpiece type in config: " +
                                    wpType);
                            }
                        }
                    } else if (key == "CAL_OFFSET") {
                        cal.calOffset = std::stoi(value);
                    } else if (key == "CAL_REF") {
                        cal.calRef = std::stoi(value);
                    }
                }
            }
        }

        if (workpieceOrder.size() != 3) {
            errors.push_back("Configured workpiece order must contain exactly "
                             "3 types (e.g.: F,BOM,OB)");
        } else {
            order = std::move(workpieceOrder);
        }
        Logger::debug("Cal. Offset: " + std::to_string(cal.calOffset));
        Logger::debug("Cal. Ref: " + std::to_string(cal.calRef));
        readResult = true;
    } else {
        Logger::warn("Config file " + configFilePath +
                     " does not exist -> create new and write default values");
        Logger::warn("Please perform a calibration of the HeightSensor!");
        std::ofstream fileStream;
        fileStream.open(configFilePath);
        fileStream << "ORDER=F,BUM,OB\n";
        fileStream << "CAL_OFFSET=" << ADC_DEFAULT_OFFSET << "\n";
        fileStream << "CAL_REF=" << ADC_DEFAULT_HIGH << "\n";
        fileStream.close();
        readResult = true;
    }

    if (!errors.empty()) {
        Logger::error("Config file contains errors. Please fix them:");
        for (const auto &msg : errors) {
            Logger::error("- " + msg);
        }
        return false;
    }

    std::stringstream ss;
    for (size_t i = 0; i < order.size(); ++i) {
        ss << WP_TYPE_TO_STRING(order[i]);
        if (i < order.size() - 1) {
            ss << " -> ";
        }
    }
    Logger::info("Configured workpiece order: " + ss.str());

    return readResult;
}

void Configuration::setMaster(bool isMaster) { this->isMaster = isMaster; }

bool Configuration::systemIsMaster() { return isMaster; }

void Configuration::setPusherMounted(bool pusherIsMounted) {
    this->hasPusher = pusherIsMounted;
}

bool Configuration::pusherMounted() { return hasPusher; }

void Configuration::setDesiredWorkpieceOrder(std::vector<WorkpieceType> order) {
    this->order = order;
}

std::vector<WorkpieceType> Configuration::getDesiredOrder() { return order; }

void Configuration::setOffsetCalibration(int offset) { cal.calOffset = offset; }

void Configuration::setReferenceCalibration(int refHigh) {
    cal.calRef = refHigh;
}

Calibration Configuration::getCalibration() { return cal; }

void Configuration::saveCurrentConfigToFile() {
    std::stringstream ss;
    for (size_t i = 0; i < order.size(); ++i) {
        if (order[i] == WorkpieceType::WS_F) {
            ss << "F";
        } else if (order[i] == WorkpieceType::WS_BOM) {
            ss << "BOM";
        } else if (order[i] == WorkpieceType::WS_BUM) {
            ss << "BUM";
        } else if (order[i] == WorkpieceType::WS_OB) {
            ss << "OB";
        }

        if (i < order.size() - 1) {
            ss << ",";
        }
    }
    const std::string &order = "ORDER=" + ss.str();
    const std::string &offset = "CAL_OFFSET=" + std::to_string(cal.calOffset);
    const std::string &ref = "CAL_REF=" + std::to_string(cal.calRef);

    std::ofstream outputFile(configFilePath);
    if (!outputFile) {
        Logger::error("Error opening config file for writing");
        return;
    }

    outputFile << order << std::endl;
    outputFile << offset << std::endl;
    outputFile << ref << std::endl;
    outputFile.close();

    Logger::info("Config file was saved");
}

bool Configuration::calibrationValid() {
	if(cal.calOffset == ADC_DEFAULT_OFFSET && cal.calRef == ADC_DEFAULT_HIGH)
		return false;
	if(cal.calOffset > 4000 || cal.calOffset < 3000)
		return false;
	if(cal.calRef > 3300 || cal.calRef < 1500)
		return false;
	if((cal.calOffset - cal.calRef) < 500)
		return false;
	return true;
}

void Configuration::writeLineToConfigFile(int lineNumber,
                                          const std::string &newContent) {
    std::ifstream inputFile(configFilePath);
    if (!inputFile) {
        Logger::error("Error opening config file.");
        return;
    }

    const std::string &tempFilePath = configFilePath + ".tmp";

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
        Logger::error("Error renaming temporary file: " + tempFilePath +
                      " -> " + configFilePath);
        perror("Error renaming file");
        return;
    }

    Logger::info("Updated line " + std::to_string(lineNumber) +
                 " in config file -> " + newContent);
}
