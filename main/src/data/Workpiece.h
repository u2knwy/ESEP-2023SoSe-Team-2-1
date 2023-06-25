/*
 * Workpiece.h
 *
 *  Created on: 9.06.2023
 *      Author: Sulaiman
 */

#pragma once

#include "workpiecetype_enum.h"
#include "workpiecetype_stringlist.h"

#define WP_TYPE_TO_STRING(wp) std::string(WorkpieceTypeString[wp])

struct Workpiece {
    /**
     * Workpiece ID
     */
    int id{-1};

    /**
     * The average measured height at FBM1
     */
    float avgHeightFBM1{0.0};

    /**
     * The average measured height at FBM2
     */
    float avgHeightFBM2{0.0};

    /**
     * Maximum measured height at FBM2
     */
    float maxHeightFBM2{0.0};

    /**
     * Whether the Workpiece contains a metal.
     */
    bool metal{false};

    /**
     * if the workpiece was flipped between FBM1 and FBM2.
     */
    bool flipped{false};

    /**
     * Set the result if a workpiece should sorted out or not
     */
    bool sortOut{false};

    /**
     * Type of the workpiece in FBM-Master
     *
     * @return Workpiece Type (TYPE_A, TYPE_B, TYPE_C or UNKNOWN (default))
     */
    WorkpieceType M_type = WorkpieceType::WS_UNKNOWN;

    /**
     * Type of the workpiece in FBM-Slave
     *
     * @return Workpiece Type (TYPE_A, TYPE_B, TYPE_C or UNKNOWN (default))
     */
    WorkpieceType S_type{WorkpieceType::WS_UNKNOWN};
};
