/* 
 * File:   item.h
 * @author Lehmann
 * @date 3. April 2020
 */

#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <iostream>

enum class ItemKinds {
    flat, holeup, holedown, metalup, metaldown, codeA, codeB, codeC, codeD, codeE, codeF, codeG, codeH, lego1, lego2, lego3
};

enum class ItemState {
    onBelt, onSlide, droppedLeft, droppedRight, removed
};

/* Three non-overlapping regions are defined on the belt. If a workpiece enters 
 * one of these regions, a message is displayed, either on the console or via 
 * network. Since the regions do not overlap, a workpiece can be in one region 
 * at most and the region can be stored with the workpiece.
 */
enum class RoI {
    none, entry, exit, discarded
};

class SimItem {
private:
    static short int heightValues[13][41];
    static short int heightValuesLego[3][65];
    static unsigned int IDCounter;
    unsigned char heightIndex;
public:
    double x;
    double y;
    ItemKinds kind;
    ItemState state;
    unsigned int ID;
    RoI roi;
    bool flip;
    bool sticky;

    SimItem(ItemKinds kind);
    SimItem(ItemKinds kind, double x, double y);
    
    bool isMagnetic() {
        return kind == ItemKinds::metalup;
    };
    
    void setFlipping(){flip = true;};
    void evalFlip();
    
    void setSticky(){sticky = true;};
    
    double getHeight(double relativeX);
    short int getADCHeight(double relativeX);

    std::string toJSONString();
    void typeToShortTypeName(std::ostream& result);

private:
    double calcRingHeight(unsigned char code, double relativeX);
    double calcLegoHeight(unsigned char code, double relativeX);
};

#endif /* ITEM_H */

