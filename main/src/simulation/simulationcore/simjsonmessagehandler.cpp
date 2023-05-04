/* 
 * File:   simjsonmessagehandler.cpp
 * @author Lehmann
 * @date 15. Mai 2020
 */

#include "simjsonmessagehandler.h"
#include "simitemhandlingaction.h"
#include "simctrlaction.h"
#include "simhciaction.h"
#include "simconfaction.h"

bool SimJSONMessageHandler::dispatchMessage(std::string message) {
    bool result = false;
    msgKey = "";
    msgValue = "";
    if (parseJSON(message)) {  // only true if type is identified
        if (string("itemaction") == msgValue) {
            if (itemhandler != nullptr) {
                // parse message for item action
                SimItemHandlingAction action;
                if (action.parseJSON(message)) {
                    itemhandler->addAction(action);
                    result = true;
                }
            }
        }
        if (string("simctrl") == msgValue) {
            if (simctrlhandler != nullptr) {
                // parse message for item action
                SimCtrlAction action;
                if (action.parseJSON(message)) {
                    simctrlhandler->addAction(action);
                    result = true;
                }
            }
        }
        if (string("hciaction") == msgValue) {
            if (hcihandler != nullptr) {
                // parse message for item action
                SimHCIAction action(0);
                if (action.parseJSON(message)) {
                    hcihandler->addAction(action);
                    result = true;
                }
            }
        }
        if (string("simconfaction") == msgValue) {
            if (confhandler != nullptr) {
                // parse message for item action
                SimConfAction action;
                if (action.parseJSON(message)) {
                    confhandler->addAction(action);
                    result = true;
                }
            }
        }
    }
    return result;
}

bool SimJSONMessageHandler::evalPair(const char* key, const char* value) {
    //cout << "text>" << key << ":" << value << endl;
    bool validPair = false;

    if (string("type") == key) {
        // enough, found message type header.
        msgKey = string(key);
        msgValue = string(value);
        validPair = true;
        pstate = PJS::end;
    }

    return validPair;
}
