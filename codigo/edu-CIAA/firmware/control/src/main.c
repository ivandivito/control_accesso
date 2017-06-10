
/*==================[inclusions]=============================================*/

#include "sapi_adc.h"
#include "sapi_board.h"
#include "sapi_gpio.h"
#include "sapi_rtc.h"
#include "sapi_uart.h"
#include "board.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "portable.h"
#include "semphr.h"

#include "ctype.h"

#include "heap.h"
#include "main.h"
#include "uart.h"
#include "print.h"
#include "Beacon.h"

//#define MAIN_DEBUG

#define SERIAL_OPERATOR_CMD '$'
#define SERIAL_OPERATOR_SPACE " "
#define SERIAL_OPERATOR_START 'S'
#define SERIAL_OPERATOR_END 'E'
#define SERIAL_OPERATOR_READ 'R'

#define BLE_OPERATOR_START "$S\n"
#define BLE_OPERATOR_END "$E\n"

#define PC_START_MSG "start ble scan\r\n"
#define PC_END_MSG "end ble scan\r\n"
#define PC_FORMAT_ERROR_MSG "format error\r\n"

#define PC_BEACON_CB_PRESENCE_MODE 0

#define PC_BEACON_PRESENCE_STATE_PRESENT "present"
#define PC_BEACON_PRESENCE_STATE_MISSING "missing"
#define PC_BEACON_PRESENCE_STATE_LOST "lost"


#define BEACON_DATABASE_UPDATE_PERIOD_MS 100

#define CMD_BUFFER_SIZE 30
#define BLE_BUFFER_SIZE 30

char cmd_buffer[CMD_BUFFER_SIZE];
int cmd_buffer_index = 0;

char ble_buffer[BLE_BUFFER_SIZE];
int ble_buffer_index = 0;

uint16_t majorTest = 0;
uint16_t minorTest = 0;

static void initHardware(void) {

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

#ifdef MAIN_DEBUG
    debug_uart("init hard \r\n");
#endif

}

//busco el largo del comando
static size_t getCmdLength(){

    size_t cmd_length = 0;
    while (cmd_buffer[cmd_length] != '\0'){
        cmd_length++;
        //si recorri el buffer y no encotnre el caracter de finalizacion no proceso el comando
        if(cmd_length == CMD_BUFFER_SIZE){
            return 0;
        }
    }
    return cmd_length;
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

static bool_t parseReadStatusCmd(char* data){

    static char auxBuff[CMD_BUFFER_SIZE];

    char *token;
    char *endptr = NULL;

    //copio el comando para poder partirlo;
    strcpy(auxBuff,data);

    //obtengo el numero mayor, si no existe o es invalido cancelo
    token = strtok(auxBuff, SERIAL_OPERATOR_SPACE);
    if(token == NULL){
        return FALSE;
    }
    uint16_t majorAddr = (uint16_t) strtol (token, &endptr, 10 );
    if(majorAddr == 0 && token == endptr){
        return FALSE;
    }

    //obtengo el numero menor, si no existe o es invalido cancelo
    token = strtok(NULL, SERIAL_OPERATOR_SPACE);
    if(token == NULL){
        return FALSE;
    }
    uint16_t minorAddr = (uint16_t) strtol (token, &endptr, 10 );
    if(minorAddr == 0 && token == endptr){
        return FALSE;
    }

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

}

static bool_t parseAssignCbCmd(char* data){

    static char auxBuff[CMD_BUFFER_SIZE];
    char *token;
    char *endptr = NULL;

    //copio el comando para procesarlo
    strcpy(auxBuff,data);

    //obtengo el indice
    token = strtok(auxBuff, SERIAL_OPERATOR_SPACE);
    if(token == NULL){
        return FALSE;
    }
    uint16_t ledIndex = (uint16_t) strtol (token, &endptr, 10 );
    if(ledIndex == 0 && token == endptr){
        return FALSE;
    }

    //busco si hay asignacion de modo
    token = strtok(NULL, SERIAL_OPERATOR_SPACE);
    if(token == NULL){
        //todo devolver datos actuales
        return TRUE;
    }
    uint16_t mode = (uint16_t) strtol (token, &endptr, 10 );
    if(mode == 0 && token == endptr){
        return FALSE;
    }

    //obtengo el numero mayor, si no existe o es invalido cancelo
    token = strtok(NULL, SERIAL_OPERATOR_SPACE);
    if(token == NULL){
        return FALSE;
    }
    uint16_t majorAddr = (uint16_t) strtol (token, &endptr, 10 );
    if(majorAddr == 0 && token == endptr){
        return FALSE;
    }

    //obtengo el numero menor, si no existe o es invalido cancelo
    token = strtok(NULL, SERIAL_OPERATOR_SPACE);
    if(token == NULL){
        return FALSE;
    }
    uint16_t minorAddr = (uint16_t) strtol (token, &endptr, 10 );
    if(minorAddr == 0 && token == endptr){
        return FALSE;
    }

    //asigno el cb correspondiente
    switch(mode){
        case PC_BEACON_CB_PRESENCE_MODE:
            switch (ledIndex){
                case 1:
                    addBeaconTracking(majorAddr, minorAddr, beaconPresentCbLed1);
                    break;
                case 2:
                    addBeaconTracking(majorAddr, minorAddr, beaconPresentCbLed2);
                    break;
                case 3:
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

//esta tarea procesa los comandos provinientes por USB
static void cmdManagerTask(void * a){

    char cmdChar;

    while(1) {
        //espero a recibir un comando entero
        while(1){

            //levanto proximo caracter
            xQueueReceive(inputUartUsbQueue, &cmdChar, portMAX_DELAY);

            if(cmdChar == '\r' || cmdChar == '\n'){
                cmd_buffer[cmd_buffer_index] = '\0';
                cmd_buffer_index = 0;

                //si recibo un comando entero corto el while para procesarlo
                break;
            } else {
                //guardo y reinicio el buffer en caso de overflow
                cmd_buffer[cmd_buffer_index] = cmdChar;
                cmd_buffer_index++;
                if(cmd_buffer_index >= CMD_BUFFER_SIZE){
                    cmd_buffer_index = 0;
                }
            }

        }

        #ifdef MAIN_DEBUG
        debug_uart("cmd: %s \r\n",cmd_buffer);
        #endif

        //busco el largo del comando
        size_t cmd_length = getCmdLength();

        //si la linea de comando no empieza con el caracter establecido o es mas corta que el largo minimo corto
        if(cmd_length < 2 || cmd_buffer[0] != SERIAL_OPERATOR_CMD){
            continue;
        }

        //reviso que comando es
        if(!isdigit(cmd_buffer[1])){

            switch (cmd_buffer[1]){
                //comando de inicio de operacion del modulo ble
                case SERIAL_OPERATOR_START:
#ifdef MAIN_DEBUG
                    debug_uart("SERIAL_OPERATOR_START\r\n",cmd_buffer);
#endif
                    ble_uart(BLE_OPERATOR_START);
                    pc_uart(PC_START_MSG);
                    break;
                //comando de fin de operacion del modulo ble
                case SERIAL_OPERATOR_END:

#ifdef MAIN_DEBUG
                    debug_uart("SERIAL_OPERATOR_END\r\n",cmd_buffer);
#endif
                    ble_uart(BLE_OPERATOR_END);
                    pc_uart(PC_END_MSG);
                    break;
                //comando de pedido de estado de un beacon
                case SERIAL_OPERATOR_READ:
#ifdef MAIN_DEBUG
                    debug_uart("SERIAL_OPERATOR_READ\r\n",cmd_buffer);
#endif
                    if(!parseReadStatusCmd(cmd_buffer+2)){
                        pc_uart(PC_FORMAT_ERROR_MSG);
                    }

                    break;
            }

        } else {

            if(!parseAssignCbCmd(cmd_buffer+1)){
                pc_uart(PC_FORMAT_ERROR_MSG);
            }
        }

    }
}

//esta tarea procesa la informacion probiniente del modulo ble
static void bleDataManagerTask(void * a){

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

#ifdef MAIN_DEBUG
        debug_uart("ble data\r\n");
#endif

        //envio el stream de datos a la biblioteca
        onStringUpdate(ble_buffer,currentTime);
    }

}

//esta tarea actualiza el estado de la base de datos
static void bleDataPeriodicTask(void * a){

    while(1){

        // levanto tiempo actual
        portTickType currentTime = xTaskGetTickCount();
        // actualizo la base de datos
        onTimeUpdate(currentTime);
        //retraso la proxima ejecucion
        vTaskDelay(BEACON_DATABASE_UPDATE_PERIOD_MS / portTICK_RATE_MS);
    }
}

int main(void) {

    //inicio hardware
	initHardware();

    //inicio base de datos
    initDatabase(BEACON_DEFAULT,BEACON_DEFAULT,BEACON_DEFAULT);

    //inicio tareas
    initUartTask(tskIDLE_PRIORITY+1);
    xTaskCreate(bleDataManagerTask, (const char *)"bleDataManagerTask", configMINIMAL_STACK_SIZE, 0, tskIDLE_PRIORITY+3, 0);
    xTaskCreate(bleDataPeriodicTask, (const char *)"bleDataPeriodicTask", configMINIMAL_STACK_SIZE, 0, tskIDLE_PRIORITY+3, 0);
    xTaskCreate(cmdManagerTask, (const char *)"cmdManagerTask", configMINIMAL_STACK_SIZE, 0, tskIDLE_PRIORITY+2, 0);

    //inicio scheduler
	vTaskStartScheduler();

    //si por alguna razon se termina la ejecucion limpio la base de datos
    deleteDatabase();

	while (1) {}
}

