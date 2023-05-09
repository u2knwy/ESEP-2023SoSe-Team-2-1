/* 
 * File:   simbase.h
 * Variant: QNX
 * @author Lehmann
 * @date 3. April 2020
 */

#ifndef SIMBASE_H
#define SIMBASE_H

// common definitions
#define SPEED_X_NORMAL   75  /* [mm/s] */
#define SPEED_Y_NORMAL   10  /* [mm/s] */

class SimulationBase {
public:
    static constexpr unsigned int timeslice = 20;       // Simulate this time in each cycle
    static constexpr unsigned int realTimeSlice = 20;   // Do step only if this time has expired since last call
    static constexpr unsigned int simVersionCode = 22;
    static constexpr unsigned int displayEachNCycle = 10;
};



#endif /* SIMBASE_H */

