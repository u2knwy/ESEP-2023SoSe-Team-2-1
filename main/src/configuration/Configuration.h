/*
 * Configuration.h
 *
 *  Created on: 07.05.2023
 *      Author: Maik
 */
#pragma once

#include <string>
#include <vector>
#include "data/Workpiece.h"

#define DEFAULT_CONFIG_FILE_PATH "conf.txt"

struct Calibration {
	int calOffset;
	int calRef;
};

class Configuration {
public:
	static Configuration &getInstance()
	{
		static Configuration instance;
		return instance;
	}
	void setConfigFilePath(std::string filePath);
	void readConfigFromFile();
	void setMaster(bool isMaster);
	bool systemIsMaster();
	void setPusherMounted(bool pusherIsMounted);
	bool pusherMounted();
	void setDesiredWorkpieceOrder(std::vector<WorkpieceType> order);
	std::vector<WorkpieceType> getDesiredOrder();
	Calibration getCalibration();
	void setOffsetCalibration(int offset);
	void setReferenceCalibration(int refHigh);
	void saveCurrentConfigToFile();
private:
	Configuration();
	virtual ~Configuration();
	Configuration(const Configuration &) = delete;
	Configuration &operator=(const Configuration &) = delete;
	std::string configFilePath;
	std::vector<WorkpieceType> order;
	bool isMaster{true};
	bool hasPusher{false};
	Calibration cal;
	void writeLineToConfigFile(int lineNumber, const std::string& newContent);
};
