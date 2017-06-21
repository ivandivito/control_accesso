
/*==================[inclusions]=============================================*/


#include "main.h"

//#define MAIN_DEVELOPMENT
//#define MAIN_DEBUG
//#define BEACON_MANAGER_DEBUG
//#define CMD_MANAGER_DEBUG

int main(void) {

    //inicio hardware
	initHardware();

    //inicio base de datos
    initBeaconDatabase();

    //inicio tareas
    initUartTask(tskIDLE_PRIORITY+1);
    initBeaconManagerTask(tskIDLE_PRIORITY+2);
    initCmdManagerTask(tskIDLE_PRIORITY+3);

    //retraso el inicio para que inicialize el modulo ble
    delayInaccurate(1000);
    //cargo el estado de la eeprom
    loadStateFromEEPROM();
    //inicio scheduler
	vTaskStartScheduler();

    //si por alguna razon se termina la ejecucion limpio la base de datos
    deleteDatabase();

	while (1) {}
}


void initHardware(void) {

    //configuro placa
    boardConfig();

    //inicializo puerto
    gpioConfig( 0, GPIO_ENABLE );

    //Teclas edu-ciaa
    gpioConfig(TEC1, GPIO_INPUT);
    gpioConfig(TEC2, GPIO_INPUT);
    gpioConfig(TEC3, GPIO_INPUT);
    gpioConfig(TEC4, GPIO_INPUT);

    //Leds edu-ciaa
    gpioConfig(LEDR, GPIO_OUTPUT);
    gpioConfig(LEDG, GPIO_OUTPUT);
    gpioConfig(LEDB, GPIO_OUTPUT);
    gpioConfig(LED1, GPIO_OUTPUT);
    gpioConfig(LED2, GPIO_OUTPUT);
    gpioConfig(LED3, GPIO_OUTPUT);

    //Conexion con nrf51822

    gpioConfig(GPIO1, GPIO_INPUT_PULLDOWN);
    gpioConfig(GPIO3, GPIO_INPUT_PULLDOWN);
    gpioConfig(GPIO5, GPIO_INPUT_PULLDOWN);
    gpioConfig(GPIO7, GPIO_INPUT_PULLDOWN);
    gpioConfig(GPIO8, GPIO_INPUT_PULLDOWN);

    initUart();

    Board_EEPROM_init();

#ifdef MAIN_DEBUG
    debug_uart("init hard \r\n");
#endif

}

void loadStateFromEEPROM(){

    //leo la direccion de checkeo
    uint8_t checkData = Board_EEPROM_readByte(EEPROM_ADDRESS_CHECK);
    //si la misma es correcta cargo la info y sino solo marco la eeprom como valida
    if(checkData == EEPROM_CHECK_TOKEN){
        beaconLoadBeaconStatus();
    } else {
        Board_EEPROM_writeByte(EEPROM_ADDRESS_CHECK,EEPROM_CHECK_TOKEN);
    }

}

void initBeaconDatabase(){

#ifndef MAIN_DEVELOPMENT
    //inicializo callbacks
    beaconSetLoadBeaconCb(loadDatabaseFromEEPROM);
    beaconSetSaveBeaconCb(saveDatabaseToEEPROM);

    //leo la direccion de checkeo
    uint8_t checkData = Board_EEPROM_readByte(EEPROM_ADDRESS_CHECK);
    //si la misma es correcta inicio la carga de la base de datos y si no la inicializo por defecto
    if(checkData == EEPROM_CHECK_TOKEN){
        size_t databaseSize = 0;
        uint32_t addr = EEPROM_ADDRESS_BEACON_DATABASE;

        //leo el tamaño de la base de datos
        if(!Board_EEPROM_readData(&addr, (uint8_t *) &databaseSize, sizeof(size_t))){
            return;
        }
        initDatabase(BEACON_DEFAULT, BEACON_DEFAULT, databaseSize, BEACON_DEFAULT);
    } else {
        initDatabase(BEACON_DEFAULT, BEACON_DEFAULT, BEACON_DEFAULT, BEACON_DEFAULT);
    }
#else
    beaconSetLoadBeaconCb(loadDatabaseFromEEPROM);
    beaconSetSaveBeaconCb(saveDatabaseToEEPROM);

    initDatabase(BEACON_DEFAULT, BEACON_DEFAULT, BEACON_DEFAULT, BEACON_DEFAULT);
#endif
}


void loadDatabaseFromEEPROM(BeaconState_t *beaconDatabase, size_t size, size_t* elementCount){

#ifndef MAIN_DEVELOPMENT
    uint8_t checkData = Board_EEPROM_readByte(EEPROM_ADDRESS_CHECK);
    //si la misma es correcta cargo la info y sino salgo
    if(checkData != EEPROM_CHECK_TOKEN){
        return;
    }

    size_t databaseSize = 0;
    size_t databaseIndex = 0;
    uint32_t addr = EEPROM_ADDRESS_BEACON_DATABASE;

    //leo el tamaño de la base de datos
    if(!Board_EEPROM_readData(&addr, (uint8_t *) &databaseSize, sizeof(size_t))){
        return;
    }
    //reviso si tengo espacio para la base de datos
    if(databaseSize > size){
        return;
    }
    *elementCount = databaseSize;
    //leo cada una de las entradas de la base de datos
    for (;databaseIndex<databaseSize;databaseIndex++){
        if(!Board_EEPROM_readData(&addr, (uint8_t *) &(beaconDatabase[databaseIndex]), sizeof(BeaconState_t))){
            return;
        }
    }
#endif

}

void saveDatabaseToEEPROM(BeaconState_t *beaconDatabase, size_t size){

    size_t databaseIndex = 0;
    uint32_t addr = EEPROM_ADDRESS_BEACON_DATABASE;

    //guardo el tamaño de la base de datos
    if(!Board_EEPROM_writeData(&addr, (uint8_t *) &size, sizeof(size_t))){
        return;
    }
    //guardo cada una de las entradas de la base de datos
    for (;databaseIndex<size;databaseIndex++){
        if(!Board_EEPROM_writeData(&addr, (uint8_t *) &(beaconDatabase[databaseIndex]), sizeof(BeaconState_t))){
            return;
        }
    }

}