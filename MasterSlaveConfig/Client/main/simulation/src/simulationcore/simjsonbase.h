
/* 
 * File:   simjsonbase.h
 * @author Lehmann
 * @date 15. Mai 2020
 */

#ifndef SIMJSONBASE_H
#define SIMJSONBASE_H

#include <string>

enum class PJS {
    waitopen, waitkey, key, waitsep, waitvalue, value, text, boolean, waitcomma, end, error
};

class SimJSONBase {
protected:
    PJS pstate = PJS::error;
public:
    virtual ~SimJSONBase(){};
    virtual bool parseJSON(std::string input);
    virtual std::string toJSONString();
protected:
    virtual bool evalPair(const char* key, const char* value);
    virtual bool evalPair(const char* key, double value);
    virtual void setToDefault();
};
#endif /* SIMJSONBASE_H */

