#ifndef _MAIN_H_
#define _MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "sapi.h"
#include "board.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "portable.h"
#include "semphr.h"

#include "ctype.h"

#include "Heap.h"
#include "Uart.h"
#include "Eeprom.h"
#include "Print.h"
#include "Beacon.h"
#include "CmdManager.h"
#include "BeaconManager.h"

int main(void);

void initHardware();
void initBeaconDatabase();

void loadStateFromEEPROM();
void loadDatabaseFromEEPROM(BeaconState_t *beaconDatabase, size_t size, size_t* elementCount);
void saveDatabaseToEEPROM(BeaconState_t *beaconDatabase, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MAIN_H_ */
