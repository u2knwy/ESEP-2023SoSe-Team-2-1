/*
 * checks.h
 *
 *  Created on: 25.06.2023
 *      Author: Maik
 */
#pragma once

#include <cmath>
#include <limits>

static std::string formatFloat(float value, int decimalPlaces) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(decimalPlaces) << value;
    return stream.str();
}

static bool compareFloats(float a, float b, float tolerance = std::numeric_limits<float>::epsilon()) {
    return std::fabs(a - b) <= tolerance;
}

