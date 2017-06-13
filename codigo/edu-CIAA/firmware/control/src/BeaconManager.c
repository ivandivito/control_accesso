

#include "BeaconManager.h"

bool_t beaconScanEnable = false;

char ble_buffer[BLE_BUFFER_SIZE];
int ble_buffer_index = 0;

//esta tarea procesa la informacion probiniente del modulo ble
void beaconManagerTask(void *a){

    char dataByte;

    while(1) {

        //espero a recibir un paquete de datos entero
        while(1){

            //levanto proximo caracter
            xQueueReceive(inputUart232Queue, &dataByte, portMAX_DELAY);

            //espero el primer caracter y si no es el de inicio lo ignoro
            if(ble_buffer_index == 0 && dataByte != BEACON_UPDATE_DATA_INIT_CHAR){
                continue;
            }

            //agrego la data y aumento el indice
            ble_buffer[ble_buffer_index] = dataByte;
            ble_buffer_index++;

            //reviso si es el ultimo caracter
            if(ble_buffer_index >= BEACON_UPDATE_DATA_LENGTH){
                //si es el correcto lo proceso y sino solo reinicio el buffer
                ble_buffer_index = 0;
                if (dataByte == BEACON_UPDATE_DATA_LAST_CHAR){
                    break;
                }
            }

        }

        //levanto el tiempo actual
        portTickType currentTime = xTaskGetTickCount();

#ifdef BEACON_MANAGER_DEBUG
        debug_uart("ble data\r\n");
#endif

        //envio el stream de datos a la biblioteca
        onStringUpdate(ble_buffer,currentTime);
    }

}

//esta tarea actualiza el estado de la base de datos
void beaconManagerPeriodicTask(void *a){

    while(1){

        // levanto tiempo actual
        portTickType currentTime = xTaskGetTickCount();
        // actualizo la base de datos
        onTimeUpdate(currentTime);
        //retraso la proxima ejecucion
        vTaskDelay(BEACON_DATABASE_UPDATE_PERIOD_MS / portTICK_RATE_MS);
    }
}

void initBeaconManagerTask(uint32_t priority){
    xTaskCreate(beaconManagerTask, (const char *)"beaconManagerTask", configMINIMAL_STACK_SIZE, 0, priority, 0);
    xTaskCreate(beaconManagerPeriodicTask, (const char *)"beaconManagerPeriodicTask", configMINIMAL_STACK_SIZE, 0, priority, 0);
}

void beaconScanInit(bool_t init){
    if(init){
        ble_uart(BLE_OPERATOR_START);
    } else {
        ble_uart(BLE_OPERATOR_END);
    }
    beaconScanEnable = init;
}

//envio al controlador informacion sobre un estado
bool_t sendBeaconState(uint16_t majorAddr,uint16_t minorAddr){

    //busco es estado de ese beacon
    BeaconState_t * beaconState = getBeaconState(majorAddr,minorAddr);

    if(beaconState != NULL){
        char* stateString;
        switch (beaconState->presenceState){
            case BEACON_STATE_PRESENT:
                stateString = PC_BEACON_PRESENCE_STATE_PRESENT;
                break;
            case BEACON_STATE_MISSING:
                stateString = PC_BEACON_PRESENCE_STATE_MISSING;
                break;
            case BEACON_STATE_LOST:
                stateString = PC_BEACON_PRESENCE_STATE_LOST;
                break;
            default:
                stateString = PC_BEACON_PRESENCE_STATE_MISSING;
                break;
        }
        pc_uart("beacon addr: %d %d, presence: %s, last distance: %d\r\n",beaconState->majorNumber,beaconState->minorNumber,stateString,beaconState->lastDistance);
    } else {
        pc_uart("beacon addr: %d %d, presence: %s, last distance: %d\r\n",majorAddr,minorAddr,PC_BEACON_PRESENCE_STATE_MISSING,0);
    }

    return TRUE;
}

//configuro un callback adecuado para el beacon pedido
bool_t configBeaconCallback(uint16_t majorAddr, uint16_t minorAddr, BeaconCbIndex_t beaconCbIndex,
                            BeaconCbMode_t beaconCbMode){

    //asigno el cb correspondiente
    switch(beaconCbMode){
        case PRESENCE_MODE:
            switch (beaconCbIndex){
                case CB_LED_1:
                    addBeaconTracking(majorAddr, minorAddr, beaconPresentCbLed1);
                    break;
                case CB_LED_2:
                    addBeaconTracking(majorAddr, minorAddr, beaconPresentCbLed2);
                    break;
                case CB_LED_3:
                    addBeaconTracking(majorAddr, minorAddr, beaconPresentCbLed3);
                    break;
                case CB_LED_RGB:
                    addBeaconTracking(majorAddr, minorAddr, beaconPresentCbLed3);
                    break;
                default:
                    return FALSE;
            }
            break;
        default:
            return FALSE;
    }

    return TRUE;

}

void beaconPresentCbLed1(BeaconState_t *beaconState){

    if(beaconState==NULL){
        return;
    }

    if(beaconState->presenceState == BEACON_STATE_PRESENT){
        gpioWrite(LED1, ON);
    } else {
        gpioWrite(LED1, FALSE);
    }
}

void beaconPresentCbLed2(BeaconState_t *beaconState){

    if(beaconState==NULL){
        return;
    }

    if(beaconState->presenceState == BEACON_STATE_PRESENT){
        gpioWrite(LED2, ON);
    } else {
        gpioWrite(LED2, FALSE);
    }
}

void beaconPresentCbLed3(BeaconState_t *beaconState){

    if(beaconState==NULL){
        return;
    }

    if(beaconState->presenceState == BEACON_STATE_PRESENT){
        gpioWrite(LED3, ON);
    } else {
        gpioWrite(LED3, FALSE);
    }
}