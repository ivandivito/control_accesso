//
// Created by ivan-ubuntu on 28/05/17.
//

#ifndef NRF51822_BEACON_H
#define NRF51822_BEACON_H

#include "cmsis/arm_math.h"
#include "string"

class Beacon {

private:
    uint16_t groupId;
    uint16_t unitId;
    uint8_t distanceRef;
    uint8_t lastDistance;

public:
    Beacon(uint16_t groupId, uint16_t unitId, uint8_t distanceRef) : groupId(groupId),
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

    uint8_t getLastDistance() const {
        return lastDistance;
    }

    uint8_t calcDistance(int8_t rssi){
        if (rssi <= 0) {
            lastDistance = 0xFF;
        } else {
            double ratio = ((double)rssi)/((double)distanceRef);
            if (ratio < 1.0) {
                lastDistance = (uint8_t) pow(ratio, 10);
            }
            else {
                lastDistance = (uint8_t) ((0.89976) * pow(ratio, 7.7095) + 0.111);
            }
        }
        return lastDistance;
    }

    void sentUpdate(Serial &pc){
/*
        pc.printf("%u ",(groupId & 0xFF00) >> 8);
        pc.printf("%u ",groupId & 0x00FF);
        pc.printf("%u ",(unitId & 0xFF00) >> 8);
        pc.printf("%u ",unitId & 0x00FF);*/

        pc.putc('$');
        pc.putc(groupId & 0x00FF);
        pc.putc((groupId & 0xFF00) >> 8);
        pc.putc(unitId & 0x00FF);
        pc.putc((unitId & 0xFF00) >> 8);
        pc.putc(lastDistance);
        pc.putc('\n');
    }


};


#endif //NRF51822_BEACON_H
