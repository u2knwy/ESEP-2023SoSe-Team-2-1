//
// Created by domxs on 17.04.2023.
//

#ifndef CONSTRUCT_EVENTTYPES_STRINGLIST_H
#define CONSTRUCT_EVENTTYPES_STRINGLIST_H

#define ESTRING(x) #x,

static const char *EventString[] = {
#include "eventtypes_estrings.h"
};

#undef ESTRING
#endif   // CONSTRUCT_EVENTTYPES_STRINGLIST_H
