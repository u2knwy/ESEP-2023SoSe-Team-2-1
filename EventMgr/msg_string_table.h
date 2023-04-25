//
// Created by Dominik on 15.04.2023.
//

#ifndef GUMBOCODE_MSG_STRING_TABLE_H
#define GUMBOCODE_MSG_STRING_TABLE_H

#define ESTRING(x) #x,
static const char* EventCodeString[] = {
#include "msg_estring_def.h"
};
#undef ESTRING

#endif //GUMBOCODE_MSG_STRING_TABLE_H
