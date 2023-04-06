/*
 * simconf.h
 *
 *  Created on: 08.02.2023
 *      Author: Lehmann
 */

#ifndef SIMCONF_H_
#define SIMCONF_H_

#include "simjsonbase.h"

#include <vector>
#include <map>
#include <string>

#define SIMCONF(x) x,

enum class SimConfCodes {
	#include "simconfparameter.h"
	None
};
#undef SIMCONF

class SimConfiguration : public SimJSONBase {
private:
    std::vector<bool> config_parameter;
    std::map<std::string, SimConfCodes> config_names;
    SimConfiguration();
public:
    static const char* SimConfStrings[];
    static int string_table_size();
public:
    ~SimConfiguration(){};
    static SimConfiguration* getInstance();
    bool isactive(SimConfCodes parametercode);
    bool isactive(std::string parametername);
    void activate(SimConfCodes parametercode);
    void deactivate(SimConfCodes parametercode);
    bool configSetTo(std::string parameter, bool value);

    void dumpConfig();
    void writeToFile(std::string configfilename="simconf.init.json");
    void readFromFile(std::string configfilename="simconf.json");
    virtual std::string toJSONString();
protected:    
    virtual bool evalPair(const char* key, const char* value);
    virtual bool evalPair(const char* key, double value);
    virtual void setToDefault();
};

#endif
