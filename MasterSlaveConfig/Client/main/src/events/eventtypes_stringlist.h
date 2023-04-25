//
// Created by domxs on 17.04.2023.
//

#ifndef CONSTRUCT_EVENTTYPES_STRINGLIST_H
#define CONSTRUCT_EVENTTYPES_STRINGLIST_H

#include "eventtypes_estrings.h"

#define ESTRING(x) #x,

static const char *EventString[] = {
#include"eventtypes_stringlist.h"
};

#undef ESTRING
#endif //CONSTRUCT_EVENTTYPES_STRINGLIST_H
