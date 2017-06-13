
/*==================[inclusions]=============================================*/


#include "sapi.h"
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
#include "BeaconManager.h"
#include "CmdManager.h"

#define MAIN_DEBUG
#define BEACON_MANAGER_DEBUG
#define CMD_MANAGER_DEBUG

int main(void) {

    //inicio hardware
	initHardware();

    //inicio base de datos
    //initDatabase(BEACON_DEFAULT,BEACON_DEFAULT,BEACON_DEFAULT);

    //inicio tareas
    //initUartTask(tskIDLE_PRIORITY+1);
    //initBeaconManagerTask(tskIDLE_PRIORITY+2);
    //initCmdManagerTask(tskIDLE_PRIORITY+3);

    //inicio scheduler
	//vTaskStartScheduler();

    //si por alguna razon se termina la ejecucion limpio la base de datos
    //deleteDatabase();

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

#ifdef MAIN_DEBUG
    debug_uart("init hard \r\n");
#endif

}
