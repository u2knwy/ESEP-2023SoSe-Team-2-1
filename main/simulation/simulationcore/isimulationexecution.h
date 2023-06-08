/* 
 * File:   isimulationexcution.h
 * @author Lehmann
 * @date 6. April 2020
 */

#ifndef ISIMULATIONEXCUTION_H
#define ISIMULATIONEXCUTION_H

class ISimlationExecution {
public:
    virtual ~ISimlationExecution(){};
    virtual void simulateTime(unsigned int duration)=0;
};

#endif /* ISIMULATIONEXCUTION_H */

