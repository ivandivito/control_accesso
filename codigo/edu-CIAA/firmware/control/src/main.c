
/*==================[inclusions]=============================================*/

#include "../../control/inc/main.h"

#include "../../control/inc/FreeRTOSConfig.h"
#include "../../control/inc/print.h"
#include "../../control/inc/sapi_adc.h"
#include "../../control/inc/sapi_board.h"
#include "../../control/inc/sapi_gpio.h"
#include "../../control/inc/sapi_rtc.h"
#include "../../control/inc/sapi_uart.h"
#include "board.h"

#include "FreeRTOS.h"
#include "task.h"


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

	//Debug UART
	uartConfig(UART_USB, 115200);

	//BLE UART
	uartConfig(UART_232, 115200);

}

static void ledTask(void * a) {

	while (1) {

		int direction = adcRead(CH1);
		debug_uart("Direction Sensor: %d \r\n", direction);

		bool_t rain = gpioRead(GPIO1);
		debug_uart("Rain Sensor: %d \r\n", rain);

		bool_t speed = gpioRead(GPIO3);
		debug_uart("Wind Speed Sensor: %d \r\n", speed);

		gpioWrite(LED1, ON);
		vTaskDelay(500 / portTICK_RATE_MS);

		gpioWrite(LED1, OFF);
		vTaskDelay(500 / portTICK_RATE_MS);

	}

}

static void rtcTask(void * a) {

	rtc_t rtc;
/*
	rtc.year = 2017;
	rtc.month = 5;
	rtc.mday = 25;
	rtc.wday = 4;
	rtc.hour = 17;
	rtc.min = 51;
	rtc.sec= 0;

	val = rtcConfig( &rtc );
*/

	while (1) {

		rtcRead(&rtc);

		debug_uart("%d - %d - %d - %d - %d - %d - %d\r\n", rtc.year, rtc.month, rtc.mday, rtc.wday, rtc.hour, rtc.min, rtc.sec);

		vTaskDelay(1000 / portTICK_RATE_MS);

	}
}


int main(void) {

	initHardware();

	xTaskCreate(ledTask, (const char *)"led", configMINIMAL_STACK_SIZE*2, 0, tskIDLE_PRIORITY+1, 0);
	xTaskCreate(rtcTask, (const char *)"rtc", configMINIMAL_STACK_SIZE*2, 0, tskIDLE_PRIORITY+1, 0);

	vTaskStartScheduler();

	while (1) {}
}

