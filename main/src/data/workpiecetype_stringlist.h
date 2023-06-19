//
// Created by domxs on 17.04.2023.
//

#ifndef CONSTRUCT_WORKPIECETYPES_STRINGLIST_H
#define CONSTRUCT_WORKPIECETYPES_STRINGLIST_H

#define ESTRING(x) #x,

static const char *WorkpieceTypeString[] = {
#include "workpiecetype_estrings.h"
};

#undef ESTRING
#endif   // CONSTRUCT_WORKPIECETYPES_STRINGLIST_H
