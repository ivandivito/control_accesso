#ifndef CONTROL_BEACONMANAGER_H
#define CONTROL_BEACONMANAGER_H

#include "sapi.h"
#include "Eeprom.h"
#include "Beacon.h"
#include "CmdManager.h"

#define BLE_OPERATOR_START "$S\n"
#define BLE_OPERATOR_END "$E\n"

#define PC_BEACON_PRESENCE_STATE_PRESENT "present"
#define PC_BEACON_PRESENCE_STATE_MISSING "missing"
#define PC_BEACON_PRESENCE_STATE_LOST "lost"

#define BEACON_DATABASE_UPDATE_PERIOD_MS 100

#define BLE_BUFFER_SIZE 30

typedef void (*beacon_scan_status_cb_t)(bool_t started);

typedef enum {CB_LED_RGB,CB_LED_1,CB_LED_2,CB_LED_3,BeaconCbIndexMax} BeaconCbIndex_t;
typedef enum {PRESENCE_MODE,BeaconCbModeMax} BeaconCbMode_t;

void beaconSetScanInitCb(beacon_scan_status_cb_t cb);
void beaconScanInit(bool_t init);
void beaconSaveBeaconStatus(bool_t started);
void beaconLoadBeaconStatus();

void beaconManagerTask(void *a);
void beaconManagerPeriodicTask(void *a);
void initBeaconManagerTask(uint32_t priority);

bool_t sendBeaconState(uint16_t majorAddr,uint16_t minorAddr);
bool_t configBeaconCallback(uint16_t majorAddr, uint16_t minorAddr, BeaconCbIndex_t beaconCbIndex,
                            BeaconCbMode_t beaconCbMode);

//definicion de callbacks
void beaconPresentCbLedRGB(BeaconState_t *beaconState);
void beaconPresentCbLed(BeaconState_t *beaconState, gpioMap_t gpioMap);
void beaconPresentCbLed1(BeaconState_t *beaconState);
void beaconPresentCbLed2(BeaconState_t *beaconState);
void beaconPresentCbLed3(BeaconState_t *beaconState);

#endif //CONTROL_BEACONMANAGER_H
