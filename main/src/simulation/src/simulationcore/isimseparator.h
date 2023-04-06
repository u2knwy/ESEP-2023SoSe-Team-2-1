
/* 
 * File:   isimseparator.h
 * @author Lehmann
 * @date 21. Mai 2020
 */

#ifndef ISIMSEPARATOR_H
#define ISIMSEPARATOR_H

class ISimSeparator {
public:
    virtual ~ISimSeparator() {
    };
    /**
     * Translate register settings to opening state of switch or pusher
     * @return state (true if open, so item can pass)
     */
    virtual bool isModePassing() = 0;
    virtual double getPosition() = 0;
    virtual bool isPusher() = 0;
};

#endif /* ISIMSEPARATOR_H */

