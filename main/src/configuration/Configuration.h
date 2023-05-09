/*
 * Configuration.h
 *
 *  Created on: 07.05.2023
 *      Author: Maik
 */

#ifndef SRC_CONFIGURATION_CONFIGURATION_H_
#define SRC_CONFIGURATION_CONFIGURATION_H_

#import <string>
#import <vector>

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
	WorkpieceType getNextExpectedWorkpiece();
private:
	Configuration* instance;
	bool isMaster;
	bool hasPusher;
	int calOffset;
	int calAdcPerMillimeter;
};

#endif /* SRC_CONFIGURATION_CONFIGURATION_H_ */
