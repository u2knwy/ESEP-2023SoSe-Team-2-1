/* 
 * File:   item.cpp
 * @author Lehmann
 * @date 3. April 2020
 */

#include <cmath>
#include <sstream>
#include "simitem.h"

#include <iostream>
using namespace std;

short int SimItem::heightValues[13][41] = {
    { 0x0a3f, 0x0a1f, 0x0a19, 0x0a14,
        0x0a17, 0x0a11, 0x0a18, 0x0a1a, 0x0a15, 0x0a14, 0x0a12, 0x0a19, 0x0a19,
        0x0a11, 0x0a15, 0x0a1f, 0x0a14, 0x0a1c, 0x0a18, 0x0a16, 0x0a19, 0x0a17,
        0x0a19, 0x0a18, 0x0a16, 0x0a17, 0x0a18, 0x0a15, 0x0a17, 0x0a15, 0x0a18,
        0x0a1a, 0x0a1b, 0x0a14, 0x0a1a, 0x0a19, 0x0a14, 0x0a1a, 0x0a18, 0x0a1a,
        0x0a27}, // flat
    { 0x0b9d, 0x0949, 0x0937, 0x092b, 0x0936, 0x0930, 0x092d, 0x092c,
        0x092f, 0x092a, 0x0931, 0x0932, 0x092a, 0x092f, 0x0cc7, 0x0cde,
        0x0cda, 0x0ce3, 0x0cea, 0x0ce4, 0x0cbf, 0x0cc4, 0x0ccf, 0x0cd2,
        0x0cd8, 0x0cd5, 0x0cda, 0x0cd2, 0x0915, 0x0924, 0x0928, 0x092f,
        0x0927, 0x0932, 0x0924, 0x092e, 0x092d, 0x092f, 0x092b, 0x092c,
        0x0943}, // hole up
    { 0x0b9d, 0x0949, 0x0937, 0x092b, 0x0936, 0x0930, 0x092d, 0x092c,
        0x092f, 0x092a, 0x0931, 0x0932, 0x0937, 0x092b, 0x0936, 0x0930,
        0x092d, 0x092c, 0x092f, 0x092a, 0x0931, 0x0932, 0x0937, 0x092b,
        0x0936, 0x0930, 0x092d, 0x092c, 0x092f, 0x092a, 0x0931, 0x0932,
        0x0927, 0x0932, 0x0924, 0x092e, 0x092d, 0x092f, 0x092b, 0x092c,
        0x0943}, // hole down
    { 0x0995, 0x0967, 0x0930, 0x093c, 0x092c, 0x0937, 0x0939, 0x093b,
        0x092f, 0x093e, 0x090c, 0x0977, 0x0938, 0x0cbc, 0x0d10, 0x0d0d,
        0x0d14, 0x0d15, 0x0d10, 0x0d05, 0x0cd5, 0x0cea, 0x0cdf, 0x0ceb,
        0x0cea, 0x0cfa, 0x0cec, 0x0d55, 0x092a, 0x092f, 0x0977, 0x0916,
        0x092f, 0x092f, 0x093b, 0x091e, 0x092e, 0x0920, 0x0920, 0x0924,
        0x0959}, // metal up
    { 0x0b9d, 0x0949, 0x0937, 0x092b, 0x0936, 0x0930, 0x092d, 0x092c,
        0x092f, 0x092a, 0x0931, 0x0932, 0x0937, 0x092b, 0x0936, 0x0930,
        0x092d, 0x092c, 0x092f, 0x092a, 0x0931, 0x0932, 0x0937, 0x092b,
        0x0936, 0x0930, 0x092d, 0x092c, 0x092f, 0x092a, 0x0931, 0x0932,
        0x0927, 0x0932, 0x0924, 0x092e, 0x092d, 0x092f, 0x092b, 0x092c,
        0x0943}, // metal down
    { 0x096f, 0x0968, 0x095c, 0x09a4, 0x0a13, 0x0a09, 0x0992, 0x0963,
        0x095c, 0x09a4, 0x0a13, 0x0a09, 0x0992, 0x0963, 0x0963, 0x096f,
        0x0a08, 0x0a0f, 0x0a12, 0x0a18, 0x0a0b, 0x0a03, 0x0a12, 0x0a01,
        0x0a0e, 0x095c, 0x0957, 0x0968, 0x0977, 0x0a10, 0x0a14, 0x09d8,
        0x095d, 0x0968, 0x0977, 0x0a10, 0x0a14, 0x09d8, 0x095d, 0x095f,
        0x096f}, // coded000
    { 0x096f, 0x0968, 0x095c, 0x09a4, 0x0a13, 0x0a09, 0x0992, 0x0963,
        0x095c, 0x09a4, 0x0a13, 0x0a09, 0x0992, 0x0963, 0x0963, 0x096f,
        0x0abc, 0x0ac3, 0x0ac6, 0x0acc, 0x0abf, 0x0ab7, 0x0ac6, 0x0ab5,
        0x0ac2, 0x095c, 0x0957, 0x0968, 0x0977, 0x0a10, 0x0a14, 0x09d8,
        0x095d, 0x0968, 0x0977, 0x0a10, 0x0a14, 0x09d8, 0x095d, 0x095f,
        0x096f}, // coded001
    { 0x096f, 0x0968, 0x095c, 0x09a4, 0x0a13, 0x0a09, 0x0992, 0x0963,
        0x095d, 0x0983, 0x0abf, 0x0ab2, 0x0998, 0x095d, 0x0963, 0x096f,
        0x0a08, 0x0a0f, 0x0a12, 0x0a18, 0x0a0b, 0x0a03, 0x0a12, 0x0a01,
        0x0a0e, 0x095c, 0x0957, 0x095f, 0x097c, 0x0ab9, 0x0aa8, 0x0981,
        0x0950, 0x0968, 0x0977, 0x0a10, 0x0a14, 0x09d8, 0x095d, 0x095f,
        0x096f}, // coded010
    { 0x096f, 0x0968, 0x095c, 0x09a4, 0x0a13, 0x0a09, 0x0992, 0x0963,
        0x095d, 0x0983, 0x0abf, 0x0ab2, 0x0998, 0x095d, 0x0963, 0x096f,
        0x0abc, 0x0ac3, 0x0ac6, 0x0acc, 0x0abf, 0x0ab7, 0x0ac6, 0x0ab5,
        0x0ac2, 0x095c, 0x0957, 0x095f, 0x097c, 0x0ab9, 0x0aa8, 0x0981,
        0x0950, 0x0968, 0x0977, 0x0a10, 0x0a14, 0x09d8, 0x095d, 0x095f,
        0x096f}, // coded011
    { 0x096f, 0x0968, 0x095d, 0x0983, 0x0abf, 0x0ab2, 0x0998, 0x095d,
        0x095c, 0x09a4, 0x0a13, 0x0a09, 0x0992, 0x0963, 0x0963, 0x096f,
        0x0a08, 0x0a0f, 0x0a12, 0x0a18, 0x0a0b, 0x0a03, 0x0a12, 0x0a01,
        0x0a0e, 0x095c, 0x0957, 0x0968, 0x0977, 0x0a10, 0x0a14, 0x09d8,
        0x095d, 0x095f, 0x097c, 0x0ab9, 0x0aa8, 0x0981, 0x0950, 0x095f,
        0x096f}, // coded100
    { 0x096f, 0x0968, 0x095d, 0x0983, 0x0abf, 0x0ab2, 0x0998, 0x095d,
        0x095c, 0x09a4, 0x0a13, 0x0a09, 0x0992, 0x0963, 0x0963, 0x096f,
        0x0abc, 0x0ac3, 0x0ac6, 0x0acc, 0x0abf, 0x0ab7, 0x0ac6, 0x0ab5,
        0x0ac2, 0x095c, 0x0957, 0x0968, 0x0977, 0x0a10, 0x0a14, 0x09d8,
        0x095d, 0x095f, 0x097c, 0x0ab9, 0x0aa8, 0x0981, 0x0950, 0x095f,
        0x096f}, // coded101
    { 0x096f, 0x0968, 0x095d, 0x0983, 0x0abf, 0x0ab2, 0x0998, 0x095d,
        0x095d, 0x0983, 0x0abf, 0x0ab2, 0x0998, 0x095d, 0x0963, 0x096f,
        0x0a08, 0x0a0f, 0x0a12, 0x0a18, 0x0a0b, 0x0a03, 0x0a12, 0x0a01,
        0x0a0e, 0x095c, 0x0957, 0x095f, 0x097c, 0x0ab9, 0x0aa8, 0x0981,
        0x0950, 0x095f, 0x097c, 0x0ab9, 0x0aa8, 0x0981, 0x0950, 0x095f,
        0x096f}, // coded110
    { 0x096f, 0x0968, 0x095d, 0x0983, 0x0abf, 0x0ab2, 0x0998, 0x095d,
        0x095d, 0x0983, 0x0abf, 0x0ab2, 0x0998, 0x095d, 0x0963, 0x096f,
        0x0abc, 0x0ac3, 0x0ac6, 0x0acc, 0x0abf, 0x0ab7, 0x0ac6, 0x0ab5,
        0x0ac2, 0x095c, 0x0957, 0x095f, 0x097c, 0x0ab9, 0x0aa8, 0x0981,
        0x0950, 0x095f, 0x097c, 0x0ab9, 0x0aa8, 0x0981, 0x0950, 0x095f,
        0x096f} // coded111
};

short int SimItem::heightValuesLego[3][65] = {
    {   0xbad, 0xbab, 0xbb8, 0xb53, 0xb47, 0xb47, 0xb49, 0xbab,
        0xbad, 0xbab, 0xbaa, 0xb3b, 0xb35, 0xb42, 0xb40, 0xbb7,
		0xdb7, 0xd7d, 0xd75, 0xd79, 0xd73, 0xd7d, 0xddd, 0xddd,
		0xdac, 0xd82, 0xd73, 0xd82, 0xd78, 0xdaa, 0xdd9, 0xde7,
		0xbb9, 0xb9d, 0xb87, 0xb42, 0xb40, 0xb4d, 0xb4a, 0xbb2,
		0xbb5, 0xb89, 0xb75, 0xb5d, 0xb48, 0xb48, 0xb55, 0xbaf,
		0xbba, 0xbaf, 0x989, 0x99b, 0x92a, 0x92f, 0x935, 0x925,
		0x990, 0x994, 0x98e, 0x98f, 0x913, 0x91e, 0x928, 0x92a,
		0x996  },
	{   0xbad, 0xbab, 0xbb8, 0xb53, 0xb47, 0xb47, 0xb49, 0xbab,
		0xbad, 0xbab, 0xbaa, 0xb3b, 0xb35, 0xb42, 0xb40, 0xbb7,
		0xdb7, 0xd7d, 0xd75, 0xd79, 0xd73, 0xd7d, 0xddd, 0xddd,
		0xdac, 0xd82, 0xd73, 0xd82, 0xd78, 0xdaa, 0xdd9, 0xde7,
		0x92a, 0x996, 0x989, 0x99b, 0x92a, 0x92f, 0x935, 0x925,
		0x990, 0x994, 0x98e, 0x98f, 0x913, 0x91e, 0x928, 0x92a,
		0x996, 0xbad, 0xbab, 0xbb8, 0xb53, 0xb47, 0xb47, 0xb49,
		0xbab, 0xbad, 0xbab, 0xbaa, 0xb3b, 0xb35, 0xb42, 0xb40,
		0xbb7 },
	{   0xbad, 0xbab, 0xbb8, 0xb53, 0xb47, 0xb47, 0xb49, 0xbab,
	    0xbad, 0xbab, 0xbaa, 0xb3b, 0xb35, 0xb42, 0xb40, 0xbb7,
		0xdb7, 0xd7d, 0xd75, 0xd79, 0xd73, 0xd7d, 0xddd, 0xddd,
		0xdac, 0xd82, 0xd73, 0xd82, 0xd78, 0xdaa, 0xdd9, 0xde7,
		0xbb9, 0xb9d, 0xb87, 0xb42, 0xb40, 0xb4d, 0xb4a, 0xbb2,
		0xbb5, 0xb89, 0xb75, 0xb5d, 0xb48, 0xb48, 0xb55, 0xbaf,
		0xbba, 0xbaf, 0xbab, 0xbb8, 0xb53, 0xb47, 0xb47, 0xb49,
		0xbab, 0xbad, 0xbab, 0xbaa, 0xb3b, 0xb35, 0xb42, 0xb40,
		0xbb7 }
};


unsigned int SimItem::IDCounter(0);

SimItem::SimItem(ItemKinds kind) : SimItem(kind, 0, 60){
};

SimItem::SimItem(ItemKinds kind, double x, double y):
heightIndex(-1), x(x), y(y), kind(kind), state(ItemState::onBelt), ID(0), roi(RoI::none), flip(false), sticky(false) {
    switch (kind) {
        case ItemKinds::flat:
            heightIndex = 0;
            break;
        case ItemKinds::holeup:
            heightIndex = 1;
            break;
        case ItemKinds::holedown:
            heightIndex = 2;
            break;
        case ItemKinds::metalup:
            heightIndex = 3;
            break;
        case ItemKinds::metaldown:
            heightIndex = 4;
            break;
        case ItemKinds::codeA:
            heightIndex = 5;
            break;
        case ItemKinds::codeB:
            heightIndex = 6;
            break;
        case ItemKinds::codeC:
            heightIndex = 7;
            break;
        case ItemKinds::codeD:
            heightIndex = 8;
            break;
        case ItemKinds::codeE:
            heightIndex = 9;
            break;
        case ItemKinds::codeF:
            heightIndex = 10;
            break;
        case ItemKinds::codeG:
            heightIndex = 11;
            break;
        case ItemKinds::codeH:
            heightIndex = 12;
            break;
        case ItemKinds::lego1:
            heightIndex = 0;
            break;
        case ItemKinds::lego2:
            heightIndex = 1;
            break;
        case ItemKinds::lego3:
            heightIndex = 2;
            break;
        default:
            heightIndex = -1;
    }
#ifndef GTEST
    ID = IDCounter++;
#endif
}

double SimItem::getHeight(double relativeX) {
    double height = 0.0;
    switch (kind) {
        case ItemKinds::flat:
            if (fabs(relativeX) <= 20.0) {
                height = 21.0;
            }
            break;
        case ItemKinds::holedown:
            if (fabs(relativeX) <= 20.0) {
                height = 25.0;
            }
            break;
        case ItemKinds::metaldown:
            if (fabs(relativeX) <= 20.0) {
                height = 25.0;
            }
            break;
        case ItemKinds::metalup:
            if (fabs(relativeX) <= 20.0) {
                height = 25.0;
            }
            if (fabs(relativeX) <= 7.5) {
                height = 10.0;
            }
            break;
        case ItemKinds::holeup:
            if (fabs(relativeX) <= 20.0) {
                height = 25.0;
            }
            if (fabs(relativeX) <= 7.5) {
                height = 10.0;
            }
            break;
        case ItemKinds::codeA:
            height = calcRingHeight(0x00, relativeX);
            break;
        case ItemKinds::codeB:
            height = calcRingHeight(0x01, relativeX);
            break;
        case ItemKinds::codeC:
            height = calcRingHeight(0x02, relativeX);
            break;
        case ItemKinds::codeD:
            height = calcRingHeight(0x03, relativeX);
            break;
        case ItemKinds::codeE:
            height = calcRingHeight(0x04, relativeX);
            break;
        case ItemKinds::codeF:
            height = calcRingHeight(0x05, relativeX);
            break;
        case ItemKinds::codeG:
            height = calcRingHeight(0x06, relativeX);
            break;
        case ItemKinds::codeH:
            height = calcRingHeight(0x07, relativeX);
            break;
        case ItemKinds::lego1:
            height = calcLegoHeight(0x01, relativeX);
            break;
        case ItemKinds::lego2:
            height = calcLegoHeight(0x02, relativeX);
            break;
        case ItemKinds::lego3:
            height = calcLegoHeight(0x03, relativeX);
            break;
        default:
            ;
    }

    return height;
}

double SimItem::calcRingHeight(unsigned char code, double relativeX) {
    double centreDistance = fabs(relativeX);
    double height = 0; // default belt
    if (centreDistance <= 20) {
        height = 25; // default full height
        if (centreDistance >= 0 && centreDistance < 5) { // ring 3
            if ((code & 0x01) != 0) { // one
                height = 20;
            } else {
                height = 23;
            }
        }
        if (centreDistance >= 8 && centreDistance < 11) { // ring 3
            if ((code & 0x02) != 0) { // one
                height = 20;
            } else {
                height = 23;
            }
        }
        if (centreDistance >= 14 && centreDistance < 17) { // ring 3
            if ((code & 0x04) != 0) { // one
                height = 20;
            } else {
                height = 23;
            }
        }
    }
    return height;
}

double SimItem::calcLegoHeight(unsigned char code, double relativeX) {
    double height = 0; // default belt
	if (relativeX >= -32 and relativeX <= -16 ) {
		height = 3.2+9.6+1.8;
	}
	if (relativeX > -16 and relativeX <= 0 ) {
		height = 3.2+1.8;
	}
    if(code == 1){
		if (relativeX >= 0 and relativeX <= 18 ) {
			height = 3.2+9.6+1.8;
		}
		if (relativeX >= 18 and relativeX <= 33 ) {
			height = 3.2+9.6+9.6+1.8;
		}
    }
    if(code == 2){
 		if (relativeX >= 0 and relativeX <= 18 ) {
 			height = 3.2+9.6+9.6+1.8;
 		}
 		if (relativeX >= 18 and relativeX <= 33 ) {
 			height = 3.2+9.6+1.8;
 		}
     }
    if(code == 3){
 		if (relativeX >= 0 and relativeX <= 18 ) {
 			height = 3.2+9.6+1.8;
 		}
 		if (relativeX >= 18 and relativeX <= 33 ) {
 			height = 3.2+9.6+1.8;
 		}
     }
    return height;
}

short int SimItem::getADCHeight(double relativeX) {
	/* Method returns the height of an item based on real measurements or maximum
	 * which means item is not in range.
	 */
    short int height = 0xFFFF;     // set to maximum as the belt is of lower value
    if(kind == ItemKinds::lego1 || kind == ItemKinds::lego2 || kind == ItemKinds::lego3){
        if ((relativeX >= -32.0) && (relativeX <= 32.0)) {
            unsigned char index = (unsigned char) (relativeX + 32.0);
            if (heightIndex >= 0 && heightIndex <=2) {
                height = heightValuesLego[heightIndex][index];
            }
            //cout << (int)heightIndex << ", " << (int)index << ", " << height << endl;
        }
    }
    else {
        if ((relativeX >= -20.0) && (relativeX <= 20.0)) {
            unsigned char index = (unsigned char) (relativeX + 20.0);
            if (heightIndex >= 0 && heightIndex <13) {
                height = heightValues[heightIndex][index];
            }
            //cout << (int)heightIndex << ", " << (int)index << ", " << height << endl;
        }
    }
    return height;
}

void SimItem::typeToShortTypeName(std::ostream& result) {
    switch (kind) {
        case ItemKinds::flat:
            result << "\"f\"";
            break;
        case ItemKinds::holedown:
            result << "\"HD\"";
            break;
        case ItemKinds::metaldown:
            result << "\"MD\"";
            break;
        case ItemKinds::metalup:
            result << "\"MU\"";
            break;
        case ItemKinds::holeup:
            result << "\"HU\"";
            break;
        case ItemKinds::codeA:
            result << "\"cA\"";
            break;
        case ItemKinds::codeB:
            result << "\"cB\"";
            break;
        case ItemKinds::codeC:
            result << "\"cC\"";
            break;
        case ItemKinds::codeD:
            result << "\"cD\"";
            break;
        case ItemKinds::codeE:
            result << "\"cE\"";
            break;
        case ItemKinds::codeF:
            result << "\"cF\"";
            break;
        case ItemKinds::codeG:
            result << "\"cG\"";
            break;
        case ItemKinds::codeH:
            result << "\"cH\"";
            break;
        case ItemKinds::lego1:
            result << "\"l1\"";
            break;
        case ItemKinds::lego2:
            result << "\"l2\"";
            break;
        case ItemKinds::lego3:
            result << "\"l3\"";
            break;
        default:
            ;
    }
}

std::string SimItem::toJSONString() {
    std::stringstream result;
    result << "{";

    result << "\"ID\": ";
    result << ID;

    result << ", \"T\": ";
    
    typeToShortTypeName(result);
    
    result << ", \"x\": ";
    result << x;
    result << ", \"y\": ";
    result << y;
    result << "}";

    return result.str();
}

void SimItem::evalFlip() {
    if (flip) {
        switch (kind) {
            case ItemKinds::holedown:
                kind = ItemKinds::holeup;
                break;
            case ItemKinds::metaldown:
                kind = ItemKinds::metalup;
                break;
            case ItemKinds::metalup:
                kind = ItemKinds::metaldown;
                break;
            case ItemKinds::holeup:
                kind = ItemKinds::holedown;
                break;
            default:
                ;
        }
    }
}
