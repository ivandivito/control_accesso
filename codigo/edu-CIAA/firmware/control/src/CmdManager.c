
#include "CmdManager.h"

char cmd_buffer[CMD_BUFFER_SIZE];
int cmd_buffer_index = 0;

//esta tarea procesa los comandos provinientes por USB
void cmdManagerTask(void * a){

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
#ifdef CMD_MANAGER_DEBUG
                    debug_uart("SERIAL_OPERATOR_START\r\n",cmd_buffer);
#endif
                    beaconScanInit(true);
                    pc_uart(PC_START_MSG);
                    break;
                    //comando de fin de operacion del modulo ble
                case SERIAL_OPERATOR_END:

#ifdef CMD_MANAGER_DEBUG
                    debug_uart("SERIAL_OPERATOR_END\r\n",cmd_buffer);
#endif
                    beaconScanInit(false);
                    pc_uart(PC_END_MSG);
                    break;
                    //comando de pedido de estado de un beacon
                case SERIAL_OPERATOR_READ:
#ifdef CMD_MANAGER_DEBUG
                    debug_uart("SERIAL_OPERATOR_READ\r\n",cmd_buffer);
#endif
                    if(!parseReadStatusCmd(cmd_buffer+PC_READ_STATUS_CMD_OFFSET)){
                        pc_uart(PC_FORMAT_ERROR_MSG);
                    }

                    break;
                case SERIAL_OPERATOR_DELETE_STATE:
#ifdef CMD_MANAGER_DEBUG
                    debug_uart("SERIAL_OPERATOR_DELETE_STATE\r\n",cmd_buffer);
#endif
                    Board_EEPROM_writeByte(EEPROM_ADDRESS_CHECK,0x00);
                    deleteDatabase();
                    initDatabase(BEACON_DEFAULT,BEACON_DEFAULT,BEACON_DEFAULT,BEACON_DEFAULT);
                    break;
                default:
                    break;
            }

        } else {

            if(!parseAssignCbCmd(cmd_buffer+PC_ASSIGN_CALLBACK_CMD_OFFSET)){
                pc_uart(PC_FORMAT_ERROR_MSG);
            }
        }

    }
}

//inicio las tareas de procesamiento de comandos
void initCmdManagerTask(uint32_t priority){
    xTaskCreate(cmdManagerTask, (const char *)"cmdManagerTask", configMINIMAL_STACK_SIZE, 0, priority, 0);
}

//proceso comando que pide informe sobre el estado de un beacon
bool_t parseReadStatusCmd(char* data){

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

    //llamo a la funcion que informa el estado del beacon
    return sendBeaconState(majorAddr,minorAddr);
}

//busco el largo del comando
size_t getCmdLength(){

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


//proceso un comando que setea un cb ante la modificaciondel estado de un beacon
bool_t parseAssignCbCmd(char* data){

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
    uint16_t beaconCbIndex = (uint16_t) strtol (token, &endptr, 10 );
    if(beaconCbIndex == 0 && token == endptr){
        return FALSE;
    }
    //compruebo parametro valido
    if(beaconCbIndex >= BeaconCbIndexMax){
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
    //compruebo parametro valido
    if(mode >= BeaconCbModeMax){
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

    //llamo a la funcion que configura el callback
    return configBeaconCallback(majorAddr, minorAddr, (BeaconCbIndex_t) beaconCbIndex, (BeaconCbMode_t) mode);
}
