/* 
 * File:   simitemhandlingaction.h
 * @author Lehmann
 * @date 4. April 2020
 */

#ifndef SIMITEMHANDLINGACTION_H
#define SIMITEMHANDLINGACTION_H

#include "simitem.h"
#include "simjsonbase.h"
#include <string>
using namespace std;

enum class SimItemHandlingActionKind {
    nop, add, removeatend, removeallslide, removeatbegin, removeall, removeid
};

class SimItemHandlingAction : public SimJSONBase {
public:
    bool executed;
    unsigned int atTime;
    SimItemHandlingActionKind actionkind;
    ItemKinds kind;
    unsigned int ID;
    double x;
    double y;
    bool flip;
    bool sticky;

    SimItemHandlingAction() : executed(false), atTime(0), actionkind(SimItemHandlingActionKind::nop), kind(ItemKinds::flat), ID(0), x(0.0), y(60.0), flip(false), sticky(false) {
    };

    SimItemHandlingAction(unsigned int atTime, ItemKinds kind) : executed(false), atTime(atTime), actionkind(SimItemHandlingActionKind::add), kind(kind), ID(0), x(0.0), y(60.0), flip(false), sticky(false) {
    };

    SimItemHandlingAction(unsigned int atTime, ItemKinds kind, bool flip, bool sticky) : executed(false), atTime(atTime), actionkind(SimItemHandlingActionKind::add), kind(kind), ID(0),  x(0.0), y(60.0), flip(flip), sticky(sticky) {
    };

    SimItemHandlingAction(unsigned int atTime, SimItemHandlingActionKind actionkind) : executed(false), atTime(atTime), actionkind(actionkind), kind(ItemKinds::flat), ID(0),  x(0.0), y(60.0), flip(false), sticky(false) {
    };
	
    SimItemHandlingAction(unsigned int atTime, SimItemHandlingActionKind actionkind, unsigned int id) : executed(false), atTime(atTime), actionkind(actionkind), kind(ItemKinds::flat), ID(id),  x(0.0), y(60.0), flip(false), sticky(false) {
    };
    virtual ~SimItemHandlingAction(){};
    std::string toJSONString() override;
private:
    bool evalPair(const char* key, const char* value) override;
    bool evalPair(const char* key, double value) override;
    void setToDefault() override;
};

#endif /* SIMITEMHANDLINGACTION_H */

