/* 
 * File:   isimulationreporthandler.h
 * @author Lehmann
 * @date 29. Mai 2020
 */

#ifndef ISIMULATIONREPORTHANDLER_H
#define ISIMULATIONREPORTHANDLER_H

#include <string>

class ISimulationReportHandler{
public:
    virtual ~ISimulationReportHandler(){};
    virtual void handlereport(const std::string jsonreport)=0;
};


#endif /* ISIMULATIONREPORTHANDLER_H */

