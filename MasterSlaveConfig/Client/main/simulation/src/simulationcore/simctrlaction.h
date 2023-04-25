/* 
 * File:   simctrlaction.h
 * @author Lehmann
 * @date 15. Mai 2020
 */

#ifndef SIMCTRLACTION_H
#define SIMCTRLACTION_H

#include "simjsonbase.h"
#include <string>

using namespace std;

class SimCtrlAction : public SimJSONBase {
private:
    bool start = false;
    bool restart = false;
public:
    SimCtrlAction() = default;
    SimCtrlAction(const std::string& command);
    ~SimCtrlAction(){};
    std::string toJSONString() override;

    bool isCommandStart(){return (start && !restart);};
    bool isCommandRestart(){return (!start && restart);};
private:
    bool evalPair(const char* key, const char* value) override;
    bool evalPair(const char* key, double value)override;
    void setToDefault()override;
};




#endif /* SIMCTRLACTION_H */

