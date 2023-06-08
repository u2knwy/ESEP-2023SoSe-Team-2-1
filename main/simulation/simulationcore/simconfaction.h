/* 
 * File:   simconfaction.h
 * @author Lehmann
 * @date 07. Jan 2023
 */

#ifndef SIMCONFACTION_H
#define SIMCONFACTION_H

#include "simjsonbase.h"
#include "simconf.h"
#include <string>

using namespace std;

class SimConfAction : public SimJSONBase {
public:
    bool executed;
    unsigned int atTime;
    SimConfCodes parameter2configure;
    bool value2configure;
public:
    SimConfAction() = default;
    SimConfAction(unsigned int atTime, const std::string& parameter, bool value);
    SimConfAction(const std::string& parameter, bool value);
    ~SimConfAction(){};
    std::string toJSONString() override;
private:
    bool evalPair(const char* key, const char* value) override;
    bool evalPair(const char* key, double value) override;
    void setToDefault()override;
};

#endif /* SIMCTRLACTION_H */

