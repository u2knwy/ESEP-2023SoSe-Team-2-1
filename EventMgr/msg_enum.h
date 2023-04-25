//
// Created by Dominik on 15.04.2023.
//

#ifndef GUMBOCODE_MSG_ENUM_H
#define GUMBOCODE_MSG_ENUM_H

#define ESTRING(x) x,
typedef enum {
#include "msg_estring_def.h"
} Event;
#undef ESTRING

#endif //GUMBOCODE_MSG_ENUM_H
