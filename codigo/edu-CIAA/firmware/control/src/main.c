
/*==================[inclusions]=============================================*/

#include "main.h"
#include "print.h"
#include "Beacon.h"

#include "ctype.h"

#include "sapi_adc.h"
#include "sapi_board.h"
#include "sapi_gpio.h"
#include "sapi_rtc.h"
#include "sapi_uart.h"
#include "board.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define SERIAL_OPERATOR_CMD '$'
#define SERIAL_OPERATOR_ASSIGN "="
#define SERIAL_OPERATOR_START 'S'
#define SERIAL_OPERATOR_END 'E'
#define SERIAL_OPERATOR_READ 'R'

#define BLE_OPERATOR_START "$S\n"
#define BLE_OPERATOR_END "$E\n"

#define BEACON_DATABASE_UPDATE_PERIOD_MS 1000

#define CMD_BUFFER_SIZE 30
#define BLE_BUFFER_SIZE 30

char cmd_buffer[CMD_BUFFER_SIZE];
int cmd_buffer_index = 0;

char ble_buffer[BLE_BUFFER_SIZE];
int ble_buffer_index = 0;

xSemaphoreHandle pc_cmd_semaphore;
xSemaphoreHandle ble_data_semaphore;

void uart_usb_callback(const uartMap_t uart, uint8_t data){
    //uartWriteByte(UART_232,data);

    portBASE_TYPE higherPriorityTaskWoken = pdFALSE;

    if(data == '\r' || data == '\n'){
        cmd_buffer[cmd_buffer_index] = '\0';
        cmd_buffer_index = 0;
        //TODO revisar si aca hay que encontrar la forma de evitar qeu un nuevo dato escriba en el buffer hsata que se termine de procesar el dato actual ¿usar colas con las ordenes?
        xSemaphoreGiveFromISR(pc_cmd_semaphore, &higherPriorityTaskWoken);
    } else {
        cmd_buffer[cmd_buffer_index] = data;
        cmd_buffer_index++;
        if(cmd_buffer_index >= CMD_BUFFER_SIZE){
            cmd_buffer_index = 0;
        }
    }

    portEND_SWITCHING_ISR(higherPriorityTaskWoken);
}

void uart_232_callback(const uartMap_t uart, uint8_t data){
    //uartWriteByte(UART_USB,data);

    portBASE_TYPE higherPriorityTaskWoken = pdFALSE;

    //si espero el primer caracter y no es el de inicio lo ignoro
    if(ble_buffer_index == 0 && data != BEACON_UPDATE_DATA_INIT_CHAR){
        return;
    }

    //agrego la data y aumento el indice
    ble_buffer[ble_buffer_index] = data;
    ble_buffer_index++;

    //reviso si es el ultimo caracter
    if(ble_buffer_index >= BEACON_UPDATE_DATA_LENGTH-1){
        //si es el correcto entrego el semaforo y si solo reinicio el buffer
        ble_buffer_index = 0;
        if (data == BEACON_UPDATE_DATA_LAST_CHAR){
            xSemaphoreGiveFromISR(ble_data_semaphore, &higherPriorityTaskWoken);
        }
    }

    portEND_SWITCHING_ISR(higherPriorityTaskWoken);
}

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

	//Debug UART
    uartConfigCb(UART_USB, uart_usb_callback);
	uartConfig(UART_USB, 115200);

	//BLE UART
    uartConfigCb(UART_232, uart_232_callback);
	uartConfig(UART_232, 115200);

}

//esta tarea procesa los comandos provinientes por USB
static void cmdManagerTask(void * a){

    while(1) {
        //espero que este disponible informacion desde la interfaz con el modulo ble
        xSemaphoreTake(pc_cmd_semaphore, portMAX_DELAY);

        //busco el largo del comando
        size_t cmd_length = 0;
        while (cmd_buffer[cmd_length] != '\0'){
            cmd_length++;
            //si recorri el buffer y no encotnre el caracter de finalizacion no proceso el comando
            if(cmd_length == CMD_BUFFER_SIZE){
                return;
            }
        }

        //si la linea de comando no empieza con el caracter establecido o es mas corta que el largo minimo corto
        if(cmd_buffer[0] != SERIAL_OPERATOR_CMD || cmd_length < 2){
            return;
        }

        //reviso que comando es
        if(!isdigit(cmd_buffer[1])){

            switch (cmd_buffer[1]){
                //comando de inicio de operacion del modulo ble
                case SERIAL_OPERATOR_START:
                    uartWriteString(UART_232,BLE_OPERATOR_START);
                    break;
                //comando de fin de operacion del modulo ble
                case SERIAL_OPERATOR_END:
                    uartWriteString(UART_232,BLE_OPERATOR_END);
                    break;
                //comando de pedido de estado de un beacon
                case SERIAL_OPERATOR_READ:
                    //todo implementar
                    break;
            }

        } else {

            char buff[CMD_BUFFER_SIZE];
            char buff_aux[10];
            //busco si existe asignacion
            size_t assignIndex = strcspn(cmd_buffer,SERIAL_OPERATOR_ASSIGN);
            //busco el indice a modificar/leer
            strncpy(buff,cmd_buffer + 1, (size_t) (assignIndex - 1));
            buff[assignIndex-1] = '\0';
            //convierto el indice a un int
            int index = atoi(buff);
            //si hay asignacion proceso
            if(assignIndex < cmd_length){

                //extraigo los parametros

                //todo implementar
                /*
                strncpy(buff_aux,cmd_buffer + assignIndex + 1, (size_t) (cmd_length - assignIndex));
                buff_aux[cmd_length - assignIndex] = '\0';
                int data = atoi(buff_aux);

                switch (index){
                    case SERIAL_INDEX_:
                        break;

                }
                 */

            }
            //halla o no asignacion imprimo el valor
            //todo implementar
            /*
            switch (index){
                case SERIAL_INDEX_:
                    break;
            }
             */
        }

    }
}

//esta tarea procesa la informacion probiniente del modulo ble
static void bleDataManagerTask(void * a){

    while(1) {
        //espero que este disponible informacion desde la interfaz con el modulo ble
        xSemaphoreTake(ble_data_semaphore, portMAX_DELAY);
        //envio el stream de datos a la biblioteca
        onStringUpdate(ble_buffer);
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

static void ledTask(void * a) {

	while (1) {

		gpioWrite(LED1, ON);
		vTaskDelay(500 / portTICK_RATE_MS);

		gpioWrite(LED1, OFF);
		vTaskDelay(500 / portTICK_RATE_MS);

	}

}

int main(void) {

    //inicio hardware
	initHardware();

    //inicio base de datos
    initDatabase(BEACON_DEFAULT,BEACON_DEFAULT,BEACON_DEFAULT);

    //inicio recursos freertos
    pc_cmd_semaphore = xSemaphoreCreateMutex();
    ble_data_semaphore = xSemaphoreCreateMutex();

    //inicio tareas
    xTaskCreate(bleDataManagerTask, (const char *)"bleDataManagerTask", configMINIMAL_STACK_SIZE, 0, tskIDLE_PRIORITY+2, 0);
    xTaskCreate(bleDataPeriodicTask, (const char *)"bleDataPeriodicTask", configMINIMAL_STACK_SIZE, 0, tskIDLE_PRIORITY+2, 0);
    xTaskCreate(cmdManagerTask, (const char *)"cmdManagerTask", configMINIMAL_STACK_SIZE, 0, tskIDLE_PRIORITY+1, 0);
	xTaskCreate(ledTask, (const char *)"ledTask", configMINIMAL_STACK_SIZE, 0, tskIDLE_PRIORITY+1, 0);

    //inicio scheduler
	vTaskStartScheduler();

    //si por alguna razon se termina la ejecucion limpio la base de datos
    deleteDatabase();

	while (1) {}
}

