//
// Created by domxs on 17.04.2023.
//

#ifndef CONSTRUCT_EVENTTYPES_ENUM_H
#define CONSTRUCT_EVENTTYPES_ENUM_H

#define ESTRING(x) x,

enum EventType {
#include "eventtypes_estrings.h"
};

#undef ESTRING


#endif //CONSTRUCT_EVENTTYPES_ENUM_H
