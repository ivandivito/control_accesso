//
// Created by ivan-ubuntu on 28/05/17.
//

#ifndef NRF51822_BEACON_H
#define NRF51822_BEACON_H


#include "cmsis/arm_math.h"

class Beacon {

private:
    uint16_t groupId;
    uint16_t unitId;
    int32_t distanceRef;
    double lastDistance;

public:
    Beacon(uint16_t groupId, uint16_t unitId, int32_t distanceRef) : groupId(groupId),
                                                                      unitId(unitId),
                                                                      distanceRef(distanceRef)
    {

    }

    uint16_t getGroupId() const {
        return groupId;
    }

    uint16_t getUnitId() const {
        return unitId;
    }

    int32_t getDistanceRef() const {
        return distanceRef;
    }

    double getLastDistance() const {
        return lastDistance;
    }

    double calcDistance(int8_t rssi){
        if (rssi == 0) {
            lastDistance = -1.0;
        } else {
            double ratio = ((double)rssi)/((double)distanceRef);
            if (ratio < 1.0) {
                lastDistance = pow(ratio,10);
            }
            else {
                lastDistance =  (0.89976)*pow(ratio,7.7095) + 0.111;
            }
        }
        return lastDistance;
    }



};


#endif //NRF51822_BEACON_H
