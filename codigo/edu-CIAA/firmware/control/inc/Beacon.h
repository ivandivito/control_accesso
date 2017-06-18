//
// Created by ivan-ubuntu on 02/06/17.
//

#ifndef CONTROL_BEACON_H
#define CONTROL_BEACON_H

#include "stdlib.h"
#include "stdint.h"
#include "stddef.h"
#include "string.h"

#include "Heap.h"
#include "portable.h"

#define MALLOC(size) pvPortMalloc((size))
#define REALLOC(mem,newSize) pvPortRealloc((mem),(newSize))
#define FREE(ptr) vPortFree(ptr)

#define BEACON_UPDATE_DATA_INIT_CHAR '$'
#define BEACON_UPDATE_DATA_LAST_CHAR '\n'
#define BEACON_UPDATE_DATA_LENGTH 7

#define BEACON_DEFAULT 0
#define BEACON_TIMEOUT_DEFAULT 1000
#define BEACON_SAVE_TIMEOUT_DEFAULT 10000
#define BEACON_DATABASE_INITIAL_SIZE_DEFAULT 10
#define BEACON_DATABASE_REALLOC_SIZE_DEFAULT 10

typedef struct BeaconState BeaconState_t;

typedef void (*beacon_tracking_cb_t)(BeaconState_t *beaconState);
typedef void (*beacon_save_database_cb_t)(BeaconState_t *beaconDatabase, size_t size);
typedef void (*beacon_load_database_cb_t)(BeaconState_t *beaconDatabase, size_t size, size_t* elementCount);

typedef struct BeaconUpdate {
    uint16_t majorNumber;
    uint16_t minorNumber;
    uint8_t lastDistance;
} BeaconUpdate_t;

typedef enum{
    BEACON_STATE_PRESENT,
    BEACON_STATE_MISSING,
    BEACON_STATE_LOST
} BeaconPresenceState_t;

struct BeaconState {
    uint16_t majorNumber;
    uint16_t minorNumber;
    uint8_t lastDistance;
    BeaconPresenceState_t presenceState;
    uint32_t lastUpdateTime;
    beacon_tracking_cb_t beacon_tracking_cb;
};

void beaconSetSaveBeaconCb(beacon_save_database_cb_t cb);
void beaconSetLoadBeaconCb(beacon_load_database_cb_t cb);

void beaconLoadBeaconStatus();

void initDatabase(uint32_t beaconDatabaseBeaconTimeout, uint32_t saveTimeout, size_t initialSize, size_t reallocSize);

void deleteDatabase();

BeaconState_t *createBeaconEntry(uint16_t majorNumber, uint16_t minorNumber);

void addUpdate(BeaconUpdate_t *data, uint32_t timestep);

BeaconState_t *getBeaconState(uint16_t majorNumber, uint16_t minorNumber);

void addBeaconTracking(uint16_t majorNumber, uint16_t minorNumber, beacon_tracking_cb_t beacon_tracking_cb);

void onTimeUpdate(uint32_t timestep);

void onStringUpdate(char* data,uint32_t timestep);

#endif //CONTROL_BEACON_H
