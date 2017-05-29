//
// Created by ivan-ubuntu on 28/05/17.
//

#ifndef NRF51822_IBEACONADVPACKAGEDATA_H
#define NRF51822_IBEACONADVPACKAGEDATA_H

#include "core_cmInstr.h"
#include "mbed.h"
#include "ble/BLE.h"
#include "cmsis/arm_math.h"

namespace BleIBeacon {

    static const size_t IBEACON_ADDR_LEN = 16;
    typedef uint8_t BeaconUUID_t[IBEACON_ADDR_LEN];

    static const uint8_t IBEACON_FIXED_PREFIX[6] = {0x1a, 0xff, 0x4c, 0x00, 0x02, 0x15};
    static const uint8_t IBEACON_FIXED_LENGHT = 27;

    typedef struct IBeaconPayload_t {
        uint8_t fixedPrefix[6];
        BleIBeacon::BeaconUUID_t beaconUUID;
        uint8_t majorNumberH;
        uint8_t majorNumberL;
        uint8_t minorNumberH;
        uint8_t minorNumberL;
        uint8_t rssiRef;
    } IBeaconPayload;

    bool isAdvertisingDatafromIBeacon(const uint8_t *data, uint8_t lenght) {
        return (lenght == IBEACON_FIXED_LENGHT &&
                !memcmp(data, IBEACON_FIXED_PREFIX, sizeof(IBEACON_FIXED_PREFIX)));
    }
}

#endif //NRF51822_IBEACONADVPACKAGEDATA_H
