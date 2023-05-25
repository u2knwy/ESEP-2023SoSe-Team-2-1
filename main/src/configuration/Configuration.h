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

struct Calibration {
	int calOffset;
	int calRef;
};

class Configuration {
public:
	static Configuration& getInstance() {
		static Configuration instance;
		return instance;
	}
	void readConfigFromFile(const std::string filePath);
	void setMaster(bool isMaster);
	bool systemIsMaster();
	void setPusherMounted(bool pusherIsMounted);
	bool pusherMounted();
	void setDesiredWorkpieceOrder(std::vector<WorkpieceType> order);
	std::vector<WorkpieceType> getDesiredOrder();
	Calibration getCalibration();
private:
	Configuration();
	virtual ~Configuration();
	Configuration(const Configuration&) = delete;
	Configuration& operator=(const Configuration&) = delete;
	std::vector<WorkpieceType> order;
	bool isMaster{true};
	bool hasPusher{false};
	int calOffset{0};
	int calRefHigh{0};
};
