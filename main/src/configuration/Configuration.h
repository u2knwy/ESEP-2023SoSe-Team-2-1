/*
 * Configuration.h
 *
 *  Created on: 07.05.2023
 *      Author: Maik
 */
#pragma once

#include <string>
#include <vector>

enum WorkpieceType {
	WS_F,
	WS_BOM,
	WS_BUM,
	UNKNOWN
};

class Configuration {
public:
	Configuration();
	virtual ~Configuration();
	void readConfigFromFile(const std::string filePath);
	bool systemIsMaster();
	bool pusherMounted();
	std::vector<WorkpieceType> getDesiredOrder();
	static Configuration& getInstance() {
		static Configuration instance;
		return instance;
	}
private:
	std::vector<WorkpieceType> order;
	bool isMaster{true};
	bool hasPusher{false};
	int calOffset{0};
	int calAdcPerMillimeter{50};
};
