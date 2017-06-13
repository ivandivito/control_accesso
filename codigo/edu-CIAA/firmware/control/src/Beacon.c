//
// Created by ivan-ubuntu on 02/06/17.
//

#include "Beacon.h"

static BeaconState_t *beaconDatabase = NULL;
static BeaconUpdate_t beaconUpdateTemp;
static size_t beaconDatabaseElementCount = 0;
static size_t beaconDatabaseSize = 0;
static size_t beaconDatabaseReallocSize = 0;
static uint32_t beaconDatabaseBeaconTimeout = 0;

// inicializo las variables de la base de datos asi como reservo memoria para la misma
void initDatabase(uint32_t beaconTimeout, size_t initialSize, size_t reallocSize){
    if(beaconTimeout == 0){
        beaconTimeout = BEACON_TIMEOUT_DEFAULT;
    }
    if(initialSize == 0){
        initialSize = BEACON_DATABASE_INITIAL_SIZE_DEFAULT;
    }
    if(reallocSize == 0){
        reallocSize = BEACON_DATABASE_REALLOC_SIZE_DEFAULT;
    }

    if(beaconDatabase == NULL){
        beaconDatabase = MALLOC(initialSize * sizeof(BeaconState_t));
        beaconDatabaseSize = initialSize;
        beaconDatabaseElementCount = 0;
        beaconDatabaseReallocSize = reallocSize;
        beaconDatabaseBeaconTimeout = beaconTimeout;
    }
}

// libero la memoria alocada y reinicio el estado de las variable
void deleteDatabase(){
    if(beaconDatabase != NULL){
        FREE(beaconDatabase);
        beaconDatabase = NULL;
        beaconDatabaseElementCount = 0;
        beaconDatabaseSize = 0;
        beaconDatabaseReallocSize = 0;
    }
}

// helper para agregar una entrada
BeaconState_t * createBeaconEntry(uint16_t majorNumber, uint16_t minorNumber){

    //si no hay espacio agrando la base de datos
    if(beaconDatabaseElementCount >= beaconDatabaseSize){
        beaconDatabaseSize += beaconDatabaseReallocSize;
        REALLOC(beaconDatabase,beaconDatabaseSize * sizeof(BeaconState_t));
    }

    //seteo identificacion
    BeaconState_t * result = &(beaconDatabase[beaconDatabaseElementCount]);
    result->majorNumber = majorNumber;
    result->minorNumber = minorNumber;
    result->lastUpdateTime = 0;
    result->lastDistance = 0;
    result->presenceState = BEACON_STATE_MISSING;
    result->beacon_tracking_cb = NULL;

    //actualizo cantidad de elementos
    beaconDatabaseElementCount++;

    return result;
}

//agrego una actualizacion proviniente del modulo bluetooth
void addUpdate(BeaconUpdate_t *data, uint32_t timestep){
    if (data == NULL){
        return;
    }

    BeaconState_t *beacon = NULL;

    //reviso si este beacon ya existe en la base de datos
    size_t i;
    for (i = 0; i < beaconDatabaseElementCount; ++i) {
        if(beaconDatabase[i].majorNumber == data->majorNumber &&
                beaconDatabase[i].minorNumber == data->minorNumber){
            beacon = &(beaconDatabase[i]);
            //si existe un callback lo llamo antes de salir
            break;
        }
    }

    //si no existe agrego una nueva entrada a la base de datos
    if(beacon == NULL){
        beacon = createBeaconEntry(data->majorNumber,data->minorNumber);
    }

    //actualizo los valores de la base de datos
    beacon->lastDistance = data->lastDistance;
    beacon->presenceState = BEACON_STATE_PRESENT;
    beacon->lastUpdateTime = timestep;

    //si hay callback lo llamo
    if(beacon->beacon_tracking_cb != NULL){
        (beacon->beacon_tracking_cb)(beacon);
    }
}

//busco el beacon en la base de datos
BeaconState_t *getBeaconState(uint16_t majorNumber, uint16_t minorNumber){

    size_t i;
    for (i = 0; i < beaconDatabaseElementCount; ++i) {
        if(beaconDatabase[i].majorNumber == majorNumber &&
           beaconDatabase[i].minorNumber == minorNumber){
            return &(beaconDatabase[i]);
        }
    }
    //si no se encontro devuelvo NULL
    return NULL;

}

//agrego un callback ante una actualizacion
void addBeaconTracking(uint16_t majorNumber, uint16_t minorNumber, beacon_tracking_cb_t beacon_tracking_cb){

    //busco si se encuenta en la base de datos
    BeaconState_t * beacon = getBeaconState(majorNumber,minorNumber);
    //si no existe lo creo para que avise ante la primera aparicion
    if(beacon==NULL){
        beacon = createBeaconEntry(majorNumber,minorNumber);
    }
    //agrego callback
    beacon->beacon_tracking_cb = beacon_tracking_cb;
}

//funcion para actualizar el estado de los beacon segun
void onTimeUpdate(uint32_t timestep){

    //actualizo el estado de todos los beacons
	size_t i;
    for (i = 0; i < beaconDatabaseElementCount; ++i) {
        if(beaconDatabase[i].presenceState == BEACON_STATE_PRESENT &&
                (timestep - beaconDatabase[i].lastUpdateTime) > beaconDatabaseBeaconTimeout){
            // si paso el timeout cambio el estado
            beaconDatabase[i].presenceState = BEACON_STATE_LOST;
            // si existe un callback llamo al mismo
            if(beaconDatabase[i].beacon_tracking_cb != NULL){
                (beaconDatabase[i].beacon_tracking_cb)(&(beaconDatabase[i]));
            }
        }
    }

}

void onStringUpdate(char* data,uint32_t timestep){

    if(data[0] != BEACON_UPDATE_DATA_INIT_CHAR){
        return;
    }
    if(data[BEACON_UPDATE_DATA_LENGTH-1] != BEACON_UPDATE_DATA_LAST_CHAR){
        return;
    }

    memcpy(&beaconUpdateTemp,data+1, sizeof(BeaconUpdate_t));

    addUpdate(&beaconUpdateTemp,timestep);

}
