#ifndef CONTROL_BEACONMANAGER_H
#define CONTROL_BEACONMANAGER_H

#include "sapi.h"
#include "Beacon.h"
#include "CmdManager.h"

#define BLE_OPERATOR_START "$S\n"
#define BLE_OPERATOR_END "$E\n"

#define PC_BEACON_PRESENCE_STATE_PRESENT "present"
#define PC_BEACON_PRESENCE_STATE_MISSING "missing"
#define PC_BEACON_PRESENCE_STATE_LOST "lost"

#define BEACON_DATABASE_UPDATE_PERIOD_MS 100

#define BLE_BUFFER_SIZE 30

typedef enum {CB_LED_1,CB_LED_2,CB_LED_3,CB_LED_RGB,BeaconCbIndexMax} BeaconCbIndex_t;
typedef enum {PRESENCE_MODE,BeaconCbModeMax} BeaconCbMode_t;

void beaconScanInit(bool_t init);

static void beaconManagerTask(void *a);
static void beaconManagerPeriodicTask(void *a);
void initBeaconManagerTask(uint32_t priority);

bool_t sendBeaconState(uint16_t majorAddr,uint16_t minorAddr);
bool_t configBeaconCallback(uint16_t majorAddr, uint16_t minorAddr, BeaconCbIndex_t beaconCbIndex,
                            BeaconCbMode_t beaconCbMode);

//definicion de callbacks
void beaconPresentCbLed1(BeaconState_t *beaconState);
void beaconPresentCbLed2(BeaconState_t *beaconState);
void beaconPresentCbLed3(BeaconState_t *beaconState);

#endif //CONTROL_BEACONMANAGER_H
