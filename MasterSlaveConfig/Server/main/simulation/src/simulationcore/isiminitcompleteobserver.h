/*
 * isiminitcompleteobserver.h
 *
 *  Created on: 16.05.2020
 *      Author: Lehmann
 */

#ifndef SRC_SIMULATIONCORE_ISIMINITCOMPLETEOBSERVER_H_
#define SRC_SIMULATIONCORE_ISIMINITCOMPLETEOBSERVER_H_

class ISimInitCompleteObserver {
public:
	virtual ~ISimInitCompleteObserver(){};
	virtual void initCompleted() = 0;
};

#endif /* SRC_SIMULATIONCORE_ISIMINITCOMPLETEOBSERVER_H_ */
