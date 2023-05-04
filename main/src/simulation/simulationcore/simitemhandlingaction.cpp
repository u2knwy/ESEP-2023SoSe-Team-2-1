
/* 
 * File:   simitemhandlingaction.cpp
 * @author Lehmann
 * @date 29. April 2020
 */

#include "simitemhandlingaction.h"
#include <sstream>
#include <iostream>
using namespace std;

bool SimItemHandlingAction::evalPair(const char* key, const char* value) {
    //cout << "text>" << key << ":" << value << endl;
    bool validPair = false;
    if (string("type") == key) {
        if (string("itemaction") == value) {
            validPair = true; // got valid string
        }
    }
    if (string("action") == key) {
        if (string("add") == value) {
            actionkind = SimItemHandlingActionKind::add;
            validPair = true;
        }
        if (string("removeatend") == value) {
            actionkind = SimItemHandlingActionKind::removeatend;
            validPair = true;
        }
        if (string("removeallslide") == value) {
            actionkind = SimItemHandlingActionKind::removeallslide;
            validPair = true;
        }
        if (string("removeatbegin") == value) {
            actionkind = SimItemHandlingActionKind::removeatbegin;
            validPair = true;
        }
        if (string("removeall") == value) {
            actionkind = SimItemHandlingActionKind::removeall;
            validPair = true;
        }
        if (string("removeid") == value) {
            actionkind = SimItemHandlingActionKind::removeid;
            validPair = true;
        }
    }
    if (string("kind") == key) {
        if (string("flat") == value) {
            kind = ItemKinds::flat;
            validPair = true;
        }
        if (string("holeup") == value) {
            kind = ItemKinds::holeup;
            validPair = true;
        }
        if (string("holedown") == value) {
            kind = ItemKinds::holedown;
            validPair = true;
        }
        if (string("metalup") == value) {
            kind = ItemKinds::metalup;
            validPair = true;
        }
        if (string("metaldown") == value) {
            kind = ItemKinds::metaldown;
            validPair = true;
        }
        if (string("code0") == value) {
            kind = ItemKinds::codeA;
            validPair = true;
        }

        if (string("code1") == value) {
            kind = ItemKinds::codeB;
            validPair = true;
        }
        if (string("code2") == value) {
            kind = ItemKinds::codeC;
            validPair = true;
        }
        if (string("code3") == value) {
            kind = ItemKinds::codeD;
            validPair = true;
        }
        if (string("code4") == value) {
            kind = ItemKinds::codeE;
            validPair = true;
        }
        if (string("code5") == value) {
            kind = ItemKinds::codeF;
            validPair = true;
        }
        if (string("code6") == value) {
            kind = ItemKinds::codeG;
            validPair = true;
        }
        if (string("code7") == value) {
            kind = ItemKinds::codeH;
            validPair = true;
        }
        if (string("lego1") == value) {
            kind = ItemKinds::lego1;
            validPair = true;
        }
        if (string("lego2") == value) {
            kind = ItemKinds::lego2;
            validPair = true;
        }
        if (string("lego3") == value) {
            kind = ItemKinds::lego3;
            validPair = true;
        }
    }
    if (string("f") == key) {
        if (string("false") == value) {
            flip = false;
            validPair = true;
        }
        if (string("true") == value) {
            flip = true;
            validPair = true;
        }
    }
    if (string("sticky") == key) {
        if (string("false") == value) {
            sticky = false;
            validPair = true;
        }
        if (string("true") == value) {
            sticky = true;
            validPair = true;
        }
    }

    //cout << "valid:" << (validPair?"T":"F") << endl;
    return validPair;
};

bool SimItemHandlingAction::evalPair(const char* key, double value) {
    //cout << "nummeric>" << key << ":" << value << endl;
    bool validPair = false;
    if (string("atTime") == key) {
        atTime = (int) value;
        validPair = true;
    }
    if (string("x") == key) {
        x = value;
        validPair = true;
    }
    if (string("y") == key) {
        y = value;
        validPair = true;
    }
    if (string("id") == key) {
        ID = (unsigned int) value;
        validPair = true;
    }
    return validPair;
};

void SimItemHandlingAction::setToDefault() {
    executed = false;
    atTime = 0;
    actionkind = SimItemHandlingActionKind::nop;
    ID = 0;
    kind = ItemKinds::flat;
    x = 0.0;
    y = 60.0;
    flip = false;
    sticky = false;
};

std::string SimItemHandlingAction::toJSONString() {
    std::ostringstream result;
    result << "{\"type\":\"itemaction\", ";
    result << "\"atTime\": " << atTime << ", ";
    result << "\"action\": ";
    if(actionkind == SimItemHandlingActionKind::removeid){
    	// This action has different set of values
    	result << "\"removeid\", ";
    	result << "\"id\": " << ID << "";
    } else {
		switch (actionkind) {
			case SimItemHandlingActionKind::nop:
				result << "\"nop\", ";
				break;
			case SimItemHandlingActionKind::add:
				result << "\"add\", ";
				break;
			case SimItemHandlingActionKind::removeall:
				result << "\"removeall\", ";
				break;
			case SimItemHandlingActionKind::removeatbegin:
				result << "\"removeatbegin\", ";
				break;
			case SimItemHandlingActionKind::removeatend:
				result << "\"removeatend\", ";
				break;
			case SimItemHandlingActionKind::removeallslide:
				result << "\"removeallslide\", ";
				break;
			case SimItemHandlingActionKind::removeid:
				// Intentionally left blank. Not needed, just to complete set of cases.
				break;
		}
		result << "\"kind\": ";
		switch (kind) {
			case ItemKinds::flat:
				result << "\"flat\", ";
				break;
			case ItemKinds::holedown:
				result << "\"holedown\", ";
				break;
			case ItemKinds::holeup:
				result << "\"holeup\", ";
				break;
			case ItemKinds::metaldown:
				result << "\"metaldown\", ";
				break;
			case ItemKinds::metalup:
				result << "\"metalup\", ";
				break;
			case ItemKinds::codeA:
				result << "\"code0\", ";
				break;
			case ItemKinds::codeB:
				result << "\"code1\", ";
				break;
			case ItemKinds::codeC:
				result << "\"code2\", ";
				break;
			case ItemKinds::codeD:
				result << "\"code3\", ";
				break;
			case ItemKinds::codeE:
				result << "\"code4\", ";
				break;
			case ItemKinds::codeF:
				result << "\"code5\", ";
				break;
			case ItemKinds::codeG:
				result << "\"code6\", ";
				break;
			case ItemKinds::codeH:
				result << "\"code7\", ";
				break;
			case ItemKinds::lego1:
				result << "\"lego1\", ";
				break;
			case ItemKinds::lego2:
				result << "\"lego2\", ";
				break;
			case ItemKinds::lego3:
				result << "\"lego3\", ";
				break;
		}
		result << "\"x\": " << x << ", ";
		result << "\"y\": " << y << ", ";
		result << "\"f\": " << (flip ? "true" : "false") << ", ";
		result << "\"sticky\": " << (sticky ? "true" : "false");
    }
    result << "}";

    return result.str();
}
