//
// Created by domxs on 17.04.2023.
//

#ifndef CONSTRUCT_WORKPIECETYPES_ENUM_H
#define CONSTRUCT_WORKPIECETYPES_ENUM_H

#include <string>

#define ESTRING(x) x,

enum WorkpieceType {
#include "workpiecetype_estrings.h"
};

#undef ESTRING

#endif   // CONSTRUCT_WORKPIECETYPES_ENUM_H
