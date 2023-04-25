
/* 
 * File:   simjsonbase.cpp
 * @author Lehmann
 * @date 16. Mai 2020
 */

#include "simjsonbase.h"
#include <cstdlib>
#include <iostream>

bool SimJSONBase::parseJSON(std::string input) {
    constexpr int MAX = 50;
    setToDefault(); // clear this oject

    bool parseResult = false;
    pstate = PJS::waitopen;
    int counter = 0;
    char buffer_key[MAX + 1];
    char buffer_value[MAX + 1];

    for (unsigned int i = 0; i < input.size() && pstate != PJS::error && pstate != PJS::end; i++) {
        char character = input[i];
        switch (pstate) {
            case PJS::waitopen:
                if (character == '{') {
                    pstate = PJS::waitkey;
                }
                break;
            case PJS::waitkey:
                if ((character == 0x27) || (character == 0x22)) {
                    pstate = PJS::key;
                    counter = 0;
                }
                break;
            case PJS::key:
                if ((character == 0x27) || (character == 0x22)) {
                    buffer_key[counter++] = '\0';
                    pstate = PJS::waitsep;
                } else {
                    if (counter < MAX) {
                        buffer_key[counter++] = character;
                    } else {
                        pstate = PJS::error;
                    }
                }
                break;
            case PJS::waitsep:
                if (character == ':') {
                    pstate = PJS::waitvalue;
                    counter = 0;
                }
                break;
            case PJS::waitvalue:
                if (isdigit(character) || character == '+' || character == '-') {
                    pstate = PJS::value;
                    //cout <<character << " numeric" << endl;
                    buffer_value[counter++] = character;
                }
                if ((character == 0x27) || (character == 0x22)) {
                    pstate = PJS::text;
                    //cout <<character << " text" << endl;
                }
                if ((character == 'f') || (character == 't')) {
                    pstate = PJS::boolean;
                    buffer_value[counter++] = character;
                    //std::cout << character << " text" << std::endl;
                } else {
                    if (character == ',' || isalpha(character)) {
                        pstate = PJS::error;
                    }
                }
                break;
            case PJS::text:
                if ((character == 0x27) || (character == 0x22)) {
                    buffer_value[counter] = '\0';
                    pstate = PJS::waitcomma;
                    counter = 0;
                    //evaluate pair
                    if (!evalPair(buffer_key, buffer_value)) {
                        pstate = PJS::error;
                    }
                } else {
                    if (counter < MAX) {
                        buffer_value[counter++] = character;
                    } else {
                        pstate = PJS::error;
                    }
                }
                break;
            case PJS::boolean:
                //std::cout << character << " bool" << std::endl;

                if ((character == ' ') || (character == ',') || (character == '}')) {
                    buffer_value[counter] = '\0';
                    pstate = PJS::waitcomma;
                    counter = 0;
                    if (character == ' ') {
                        pstate = PJS::waitcomma;
                    }
                    if (character == ',') {
                        pstate = PJS::waitkey;
                    }
                    if (character == '}') {
                        pstate = PJS::end;
                    }
                    
                    //evaluate pair
                    if (!evalPair(buffer_key, buffer_value)) {
                        pstate = PJS::error;
                    } else {
                        if (character == '}') {
                            pstate = PJS::end;
                        }
                    }
                } else {
                    if (counter < MAX) {
                        buffer_value[counter++] = character;
                    } else {
                        pstate = PJS::error;
                    }
                }
                break;
            case PJS::waitcomma:
                if (character == ',') {
                    pstate = PJS::waitkey;
                }
                if (character == '}') {
                    pstate = PJS::end;
                }
                break;
            case PJS::value:
                if (character == ',' || character == '}') {
                    buffer_value[counter] = '\0';
                    if (character == '}') {
                        pstate = PJS::end;
                    }
                    if (character == ',') {
                        pstate = PJS::waitkey;
                    }
                    counter = 0;
                    //cout << buffer_value << " ";
                    double value = std::atof(buffer_value);
                    //cout << value << endl;
                    //evaluate pair
                    if (!evalPair(buffer_key, value)) {
                        pstate = PJS::error;
                    }
                } else {
                    if (counter < MAX) {
                        buffer_value[counter++] = character;
                    } else {
                        pstate = PJS::error;
                    }

                }
                break;
            default:
                // should never happen, so it is an error.
                pstate = PJS::error;
        }
    }
    if (pstate == PJS::end) {
        parseResult = true;
    } else {
        // something went wrong, so end it.
        setToDefault();
    }

    return parseResult;
}

bool SimJSONBase::evalPair(const char* key, const char* value) {
    //cout << "text>" << key << ":" << value << endl;
    bool validPair = false;

    // this class is not serialisable and has no key/value pair
    pstate = PJS::error;

    return validPair;
};

bool SimJSONBase::evalPair(const char* key, double value) {
    //cout << "nummeric>" << key << ":" << value << endl;
    bool validPair = false;

    // this class is not serialisable and has no key/value pair
    pstate = PJS::error;

    return validPair;
};

void SimJSONBase::setToDefault() {
};

std::string SimJSONBase::toJSONString() {
    std::string result = "{}";

    return result;
}
