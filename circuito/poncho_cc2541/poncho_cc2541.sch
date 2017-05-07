EESchema Schematic File Version 2
LIBS:Misc_Poncho_Grande
LIBS:Poncho_Esqueleto
LIBS:hm-10
LIBS:Zilog
LIBS:zetex
LIBS:xilinx
LIBS:Xicor
LIBS:Worldsemi
LIBS:wiznet
LIBS:video
LIBS:valves
LIBS:ttl_ieee
LIBS:triac_thyristor
LIBS:transistors
LIBS:transf
LIBS:texas
LIBS:switches
LIBS:supertex
LIBS:stm32
LIBS:stm8
LIBS:siliconi
LIBS:silabs
LIBS:sensors
LIBS:rfcom
LIBS:relays
LIBS:regul
LIBS:references
LIBS:Power_Management
LIBS:powerint
LIBS:power
LIBS:philips
LIBS:Oscillators
LIBS:opto
LIBS:onsemi
LIBS:nxp_armmcu
LIBS:nxp
LIBS:nordicsemi
LIBS:msp430
LIBS:motors
LIBS:motorola
LIBS:motor_drivers
LIBS:modules
LIBS:microcontrollers
LIBS:microchip_pic32mcu
LIBS:microchip_pic24mcu
LIBS:microchip_pic18mcu
LIBS:microchip_pic16mcu
LIBS:microchip_pic12mcu
LIBS:microchip_pic10mcu
LIBS:microchip_dspic33dsc
LIBS:microchip
LIBS:memory
LIBS:mechanical
LIBS:maxim
LIBS:logo
LIBS:linear
LIBS:leds
LIBS:Lattice
LIBS:ir
LIBS:interface
LIBS:intel
LIBS:hc11
LIBS:graphic
LIBS:gennum
LIBS:ftdi
LIBS:ESD_Protection
LIBS:elec-unifil
LIBS:dsp
LIBS:display
LIBS:diode
LIBS:digital-audio
LIBS:device
LIBS:dc-dc
LIBS:cypress
LIBS:contrib
LIBS:conn
LIBS:cmos_ieee
LIBS:cmos4000
LIBS:brooktre
LIBS:bosch
LIBS:bbd
LIBS:battery_management
LIBS:audio
LIBS:atmel
LIBS:analog_switches
LIBS:analog_devices
LIBS:Altera
LIBS:allegro
LIBS:adc-dac
LIBS:actel
LIBS:ac-dc
LIBS:74xx
LIBS:74xgxx
LIBS:poncho_cc2541-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Poncho Chico - Modelo - Ejemplo - Template"
Date "2015-10-02"
Rev "1.0"
Comp "Proyecto CIAA - COMPUTADORA INDUSTRIAL ABIERTA ARGENTINA"
Comment1 "https://github.com/ciaa/Ponchos/tree/master/modelos/doc"
Comment2 "Autores y Licencia del template (Diego Brengi - UNLaM)"
Comment3 "Autor del poncho (COMPLETAR NOMBRE Y APELLIDO). Ver directorio \"doc\""
Comment4 "CÓDIGO PONCHO:"
$EndDescr
$Comp
L OSHWA #G2
U 1 1 560A0A28
P 10800 6050
F 0 "#G2" H 10770 6450 60  0001 C CNN
F 1 "OSHWA" H 10800 6353 60  0001 C CNN
F 2 "Poncho_Esqueleto:Logo_OSHWA" H 10800 6050 60  0001 C CNN
F 3 "" H 10800 6050 60  0000 C CNN
	1    10800 6050
	1    0    0    -1  
$EndComp
$Comp
L Logo_Poncho #G1
U 1 1 560CFFC0
P 10150 6000
F 0 "#G1" H 10100 5600 60  0001 C CNN
F 1 "Logo_Poncho" H 10350 5600 60  0001 C CNN
F 2 "Poncho_Esqueleto:Logo_Poncho" H 10150 6000 60  0001 C CNN
F 3 "" H 10150 6000 60  0001 C CNN
	1    10150 6000
	1    0    0    -1  
$EndComp
Text Notes 14250 1350 0    79   Italic 16
Este diseño puede copiarse como un \nTemplate de KICAD. \n1)En el administrador de Proyectos:  \n   Proyecto Nuevo -> Nuevo Proyecto desde Template\n2)Ingresar el nombre del poncho a crear.\n3)Indicar el directorio de este modelo.
Text Notes 14300 300  0    118  ~ 24
http://www.proyecto-ciaa.com.ar/
$Comp
L Conn_PonchoMP_2x_20x2 XA1
U 6 1 560E54EF
P 1700 4450
F 0 "XA1" H 1600 4750 60  0000 C CNN
F 1 "Conn_PonchoMP_2x_20x2" H 1300 4650 60  0000 C CNN
F 2 "HM-10:Conn_Poncho_Chico" H 1550 4800 60  0001 C CNN
F 3 "" H 1550 4800 60  0000 C CNN
	6    1700 4450
	1    0    0    -1  
$EndComp
$Comp
L Conn_PonchoMP_2x_20x2 XA1
U 11 1 560E5596
P 1550 2900
F 0 "XA1" H 1450 3400 60  0000 C CNN
F 1 "Conn_PonchoMP_2x_20x2" H 1200 3300 60  0000 C CNN
F 2 "HM-10:Conn_Poncho_Chico" H 1400 3250 60  0001 C CNN
F 3 "" H 1400 3250 60  0000 C CNN
	11   1550 2900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 560EB677
P 6100 1950
F 0 "#PWR01" H 6100 1950 30  0001 C CNN
F 1 "GND" H 6100 1880 30  0001 C CNN
F 2 "" H 6100 1950 60  0000 C CNN
F 3 "" H 6100 1950 60  0000 C CNN
	1    6100 1950
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR02
U 1 1 560EB699
P 6100 1250
F 0 "#PWR02" H 6100 1210 30  0001 C CNN
F 1 "+3.3V" H 6100 1360 30  0000 C CNN
F 2 "" H 6100 1250 60  0000 C CNN
F 3 "" H 6100 1250 60  0000 C CNN
	1    6100 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 1300 6350 1300
Wire Wire Line
	6100 1300 6100 1250
$Comp
L PWR_FLAG #FLG03
U 1 1 560EDDA1
P 5900 1300
F 0 "#FLG03" H 5900 1395 30  0001 C CNN
F 1 "PWR_FLAG" H 5900 1480 30  0000 C CNN
F 2 "" H 5900 1300 60  0000 C CNN
F 3 "" H 5900 1300 60  0000 C CNN
	1    5900 1300
	1    0    0    -1  
$EndComp
Connection ~ 6100 1300
$Comp
L Conn_PonchoMP_2x_20x2 XA1
U 2 1 560EE110
P 2700 1600
F 0 "XA1" H 2550 2150 60  0000 C CNN
F 1 "Conn_PonchoMP_2x_20x2" H 3000 2050 60  0000 C CNN
F 2 "HM-10:Conn_Poncho_Chico" H 2550 1950 60  0001 C CNN
F 3 "" H 2550 1950 60  0000 C CNN
	2    2700 1600
	1    0    0    -1  
$EndComp
NoConn ~ 6350 1800
NoConn ~ 7250 1800
NoConn ~ 7250 1700
NoConn ~ 7250 1600
NoConn ~ 7250 1500
NoConn ~ 3150 1300
NoConn ~ 3150 1400
NoConn ~ 3150 1500
NoConn ~ 3150 1600
$Comp
L GND #PWR04
U 1 1 560EE345
P 7450 1500
F 0 "#PWR04" H 7450 1500 30  0001 C CNN
F 1 "GND" H 7450 1430 30  0001 C CNN
F 2 "" H 7450 1500 60  0000 C CNN
F 3 "" H 7450 1500 60  0000 C CNN
	1    7450 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 1400 7450 1400
Wire Wire Line
	7450 1300 7450 1500
Wire Wire Line
	7250 1300 7450 1300
Connection ~ 7450 1400
Wire Wire Line
	6350 1500 6100 1500
Wire Wire Line
	6100 1500 6100 1950
Wire Wire Line
	6350 1600 6100 1600
Connection ~ 6100 1600
Wire Wire Line
	6100 1700 6350 1700
Connection ~ 6100 1700
Connection ~ 6100 1800
NoConn ~ 3150 1700
NoConn ~ 3150 1800
Text Notes 14300 2450 0    60   ~ 0
Luego de abrir el nuevo proyecto:\n1)Editar datos del rótulo.\n2)Editar los archivos del subdirectorio "doc".\n2.1) Agregarse en el archivo de licencia.\n2.2) Colocar info mínima del Poncho en el LEEME.txt\n2.3) Colocar info mínima en el CAMBIOS.doc
$Comp
L HM-10 U1
U 1 1 590F8CBD
P 5600 3950
F 0 "U1" H 5000 4950 60  0000 C CNN
F 1 "HM-10" H 6050 4950 60  0000 C CNN
F 2 "HM-10:HM-10" H 5600 3950 60  0001 C CNN
F 3 "" H 5600 3950 60  0001 C CNN
	1    5600 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 4500 2000 4500
Wire Wire Line
	2000 4400 2150 4400
Wire Wire Line
	4700 3100 4500 3100
Wire Wire Line
	4500 3200 4700 3200
Wire Wire Line
	4700 3300 4500 3300
Wire Wire Line
	4500 3400 4700 3400
Text Label 4500 3100 2    60   ~ 0
UART_TX
Text Label 4500 3200 2    60   ~ 0
UART_RX
Text Label 4500 3300 2    60   ~ 0
UART_CTS
Text Label 4500 3400 2    60   ~ 0
UART_RTS
Text Label 2150 4400 0    60   ~ 0
UART_TX
Text Label 2150 4500 0    60   ~ 0
UART_RX
$Comp
L CONN_01X02 J1
U 1 1 590F9F8F
P 1800 3800
F 0 "J1" H 1800 3950 50  0000 C CNN
F 1 "CONN_01X02" V 1900 3800 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.00mm" H 1800 3800 50  0001 C CNN
F 3 "" H 1800 3800 50  0001 C CNN
	1    1800 3800
	-1   0    0    1   
$EndComp
Wire Wire Line
	2000 3850 2150 3850
Wire Wire Line
	2150 3750 2000 3750
Text Label 2150 3750 0    60   ~ 0
UART_CTS
Text Label 2150 3850 0    60   ~ 0
UART_RTS
Wire Wire Line
	4700 4300 3850 4300
Wire Wire Line
	3850 4300 3850 4550
Wire Wire Line
	4700 4400 3850 4400
Connection ~ 3850 4400
Wire Wire Line
	6500 4300 7250 4300
Wire Wire Line
	7250 4300 7250 4550
Wire Wire Line
	6500 4400 7250 4400
Connection ~ 7250 4400
$Comp
L GND #PWR05
U 1 1 590FA7DB
P 3850 4550
F 0 "#PWR05" H 3850 4550 30  0001 C CNN
F 1 "GND" H 3850 4480 30  0001 C CNN
F 2 "" H 3850 4550 60  0000 C CNN
F 3 "" H 3850 4550 60  0000 C CNN
	1    3850 4550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 590FA7FE
P 7250 4550
F 0 "#PWR06" H 7250 4550 30  0001 C CNN
F 1 "GND" H 7250 4480 30  0001 C CNN
F 2 "" H 7250 4550 60  0000 C CNN
F 3 "" H 7250 4550 60  0000 C CNN
	1    7250 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 4100 4500 4100
Text Label 4500 4100 2    60   ~ 0
Reset
$Comp
L CONN_01X05 J3
U 1 1 590FB2F2
P 1800 6750
F 0 "J3" H 1800 7050 50  0000 C CNN
F 1 "CONN_01X05" V 1900 6750 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x05_Pitch2.00mm" H 1800 6750 50  0001 C CNN
F 3 "" H 1800 6750 50  0001 C CNN
	1    1800 6750
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR07
U 1 1 590FB726
P 2150 7000
F 0 "#PWR07" H 2150 7000 30  0001 C CNN
F 1 "GND" H 2150 6930 30  0001 C CNN
F 2 "" H 2150 7000 60  0000 C CNN
F 3 "" H 2150 7000 60  0000 C CNN
	1    2150 7000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 6950 2150 6950
Wire Wire Line
	2150 6950 2150 7000
Wire Wire Line
	2150 6550 2000 6550
Wire Wire Line
	2150 6100 2150 6550
Wire Wire Line
	4700 3700 4500 3700
Wire Wire Line
	4700 3800 4500 3800
Text Label 4500 3700 2    60   ~ 0
DC
Text Label 4500 3800 2    60   ~ 0
DD
Text Label 2150 6850 0    60   ~ 0
Reset
Text Label 2150 6750 0    60   ~ 0
DD
Text Label 2150 6650 0    60   ~ 0
DC
Wire Wire Line
	2000 6650 2150 6650
Wire Wire Line
	2150 6750 2000 6750
Wire Wire Line
	2000 6850 2150 6850
Wire Wire Line
	3400 4200 4700 4200
Connection ~ 3600 4200
$Comp
L GND #PWR08
U 1 1 590FC297
P 3600 4550
F 0 "#PWR08" H 3600 4550 30  0001 C CNN
F 1 "GND" H 3600 4480 30  0001 C CNN
F 2 "" H 3600 4550 60  0000 C CNN
F 3 "" H 3600 4550 60  0000 C CNN
	1    3600 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 4550 3600 4500
$Comp
L CONN_01X03 J2
U 1 1 590FC691
P 1800 6000
F 0 "J2" H 1800 6200 50  0000 C CNN
F 1 "CONN_01X03" V 1900 6000 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.00mm" H 1800 6000 50  0001 C CNN
F 3 "" H 1800 6000 50  0001 C CNN
	1    1800 6000
	-1   0    0    1   
$EndComp
Wire Wire Line
	2000 6100 2150 6100
$Comp
L +3.3V #PWR09
U 1 1 590FCED7
P 2150 5850
F 0 "#PWR09" H 2150 5810 30  0001 C CNN
F 1 "+3.3V" H 2150 5960 30  0000 C CNN
F 2 "" H 2150 5850 60  0000 C CNN
F 3 "" H 2150 5850 60  0000 C CNN
	1    2150 5850
	1    0    0    -1  
$EndComp
Text Label 3400 4200 2    60   ~ 0
VCC
Text Label 2150 6000 0    60   ~ 0
VCC
Wire Wire Line
	2000 6000 2150 6000
$Comp
L CONN_01X05 J4
U 1 1 590FD41A
P 9300 2700
F 0 "J4" H 9300 3150 50  0000 C CNN
F 1 "CONN_01X05" V 9400 2700 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x05_Pitch2.00mm" H 9300 2700 50  0001 C CNN
F 3 "" H 9300 2700 50  0001 C CNN
	1    9300 2700
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 590FDECC
P 8800 4900
F 0 "R1" V 8880 4900 50  0000 C CNN
F 1 "1k" V 8800 4900 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 8730 4900 50  0001 C CNN
F 3 "" H 8800 4900 50  0001 C CNN
	1    8800 4900
	0    -1   -1   0   
$EndComp
$Comp
L R R2
U 1 1 590FDF60
P 9050 4650
F 0 "R2" V 9130 4650 50  0000 C CNN
F 1 "1k" V 9050 4650 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 8980 4650 50  0001 C CNN
F 3 "" H 9050 4650 50  0001 C CNN
	1    9050 4650
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 590FDFA0
P 9050 5150
F 0 "C2" H 9075 5250 50  0000 L CNN
F 1 "100n" H 9075 5050 50  0000 L CNN
F 2 "Capacitors_THT:C_Disc_D5.0mm_W2.5mm_P5.00mm" H 9088 5000 50  0001 C CNN
F 3 "" H 9050 5150 50  0001 C CNN
	1    9050 5150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR010
U 1 1 590FDFE3
P 9050 5400
F 0 "#PWR010" H 9050 5400 30  0001 C CNN
F 1 "GND" H 9050 5330 30  0001 C CNN
F 2 "" H 9050 5400 60  0000 C CNN
F 3 "" H 9050 5400 60  0000 C CNN
	1    9050 5400
	1    0    0    -1  
$EndComp
$Comp
L SW_Push SW1
U 1 1 590FE07C
P 9350 4900
F 0 "SW1" H 9400 5000 50  0000 L CNN
F 1 "SW_Push" H 9350 4840 50  0000 C CNN
F 2 "Buttons_Switches_THT:SW_PUSH_6mm" H 9350 5100 50  0001 C CNN
F 3 "" H 9350 5100 50  0001 C CNN
	1    9350 4900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR011
U 1 1 590FE0FA
P 9650 5000
F 0 "#PWR011" H 9650 5000 30  0001 C CNN
F 1 "GND" H 9650 4930 30  0001 C CNN
F 2 "" H 9650 5000 60  0000 C CNN
F 3 "" H 9650 5000 60  0000 C CNN
	1    9650 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	8950 4900 9150 4900
Wire Wire Line
	9050 4800 9050 5000
Connection ~ 9050 4900
Wire Wire Line
	9050 5300 9050 5400
Wire Wire Line
	9550 4900 9650 4900
Wire Wire Line
	9650 4900 9650 5000
Text Label 9050 4400 2    60   ~ 0
VCC
Wire Wire Line
	9050 4500 9050 4400
$Comp
L R R3
U 1 1 590FE960
P 9100 3600
F 0 "R3" V 9180 3600 50  0000 C CNN
F 1 "470" V 9100 3600 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 9030 3600 50  0001 C CNN
F 3 "" H 9100 3600 50  0001 C CNN
	1    9100 3600
	0    1    1    0   
$EndComp
$Comp
L LED D1
U 1 1 590FEB20
P 9350 3850
F 0 "D1" H 9350 3950 50  0000 C CNN
F 1 "LED" H 9350 3750 50  0000 C CNN
F 2 "LEDs:LED_D5.0mm" H 9350 3850 50  0001 C CNN
F 3 "" H 9350 3850 50  0001 C CNN
	1    9350 3850
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR012
U 1 1 590FEC28
P 9350 4100
F 0 "#PWR012" H 9350 4100 30  0001 C CNN
F 1 "GND" H 9350 4030 30  0001 C CNN
F 2 "" H 9350 4100 60  0000 C CNN
F 3 "" H 9350 4100 60  0000 C CNN
	1    9350 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 4200 6700 4200
Wire Wire Line
	6500 4100 6700 4100
Text Label 6700 4100 0    60   ~ 0
ST_LED
Text Label 6700 4200 0    60   ~ 0
BTN
Wire Wire Line
	8650 4900 8550 4900
Text Label 8550 4900 2    60   ~ 0
BTN
Text Label 8800 3600 2    60   ~ 0
ST_LED
Wire Wire Line
	8800 3600 8950 3600
Wire Wire Line
	9250 3600 9350 3600
Wire Wire Line
	9350 3600 9350 3700
Wire Wire Line
	9350 4000 9350 4100
Wire Wire Line
	6500 3100 6700 3100
Wire Wire Line
	6700 3200 6500 3200
Wire Wire Line
	6500 3300 6700 3300
Wire Wire Line
	6700 3400 6500 3400
Wire Wire Line
	6500 3500 6700 3500
Text Label 6700 3100 0    60   ~ 0
P0
Text Label 6700 3200 0    60   ~ 0
P1
Text Label 6700 3300 0    60   ~ 0
P2
Text Label 6700 3400 0    60   ~ 0
P3
Text Label 6700 3500 0    60   ~ 0
P4
Wire Wire Line
	9100 2500 8900 2500
Wire Wire Line
	8900 2600 9100 2600
Wire Wire Line
	9100 2700 8900 2700
Wire Wire Line
	8900 2800 9100 2800
Wire Wire Line
	9100 2900 8900 2900
Text Label 8900 2500 2    60   ~ 0
P0
Text Label 8900 2600 2    60   ~ 0
P1
Text Label 8900 2700 2    60   ~ 0
P2
Text Label 8900 2800 2    60   ~ 0
P3
Text Label 8900 2900 2    60   ~ 0
P4
Wire Wire Line
	2200 2700 2000 2700
Wire Wire Line
	2200 2900 2000 2900
Wire Wire Line
	2200 3100 2000 3100
Wire Wire Line
	2200 3300 2000 3300
Text Label 2200 3400 0    60   ~ 0
P0
Text Label 2200 3300 0    60   ~ 0
P1
Text Label 2200 3100 0    60   ~ 0
P2
Text Label 2200 2900 0    60   ~ 0
P3
Text Label 2200 2700 0    60   ~ 0
P4
NoConn ~ 4700 3500
NoConn ~ 4700 3600
NoConn ~ 4700 3900
NoConn ~ 4700 4000
NoConn ~ 4700 4600
NoConn ~ 4700 4700
NoConn ~ 6500 4600
NoConn ~ 6500 4700
NoConn ~ 6500 4000
NoConn ~ 6500 3900
$Comp
L C C1
U 1 1 591015C1
P 3600 4350
F 0 "C1" H 3625 4450 50  0000 L CNN
F 1 "100n" H 3625 4250 50  0000 L CNN
F 2 "Capacitors_THT:C_Disc_D5.0mm_W2.5mm_P5.00mm" H 3638 4200 50  0001 C CNN
F 3 "" H 3600 4350 50  0001 C CNN
	1    3600 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 5900 2150 5900
Wire Wire Line
	2150 5900 2150 5850
NoConn ~ 2250 1400
NoConn ~ 6350 1400
$Comp
L GND #PWR013
U 1 1 591034F4
P 2000 1950
F 0 "#PWR013" H 2000 1950 30  0001 C CNN
F 1 "GND" H 2000 1880 30  0001 C CNN
F 2 "" H 2000 1950 60  0000 C CNN
F 3 "" H 2000 1950 60  0000 C CNN
	1    2000 1950
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG014
U 1 1 591034FA
P 1650 1800
F 0 "#FLG014" H 1650 1895 30  0001 C CNN
F 1 "PWR_FLAG" H 1650 1980 30  0000 C CNN
F 2 "" H 1650 1800 60  0000 C CNN
F 3 "" H 1650 1800 60  0000 C CNN
	1    1650 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 1500 2000 1500
Wire Wire Line
	2000 1500 2000 1950
Wire Wire Line
	2250 1600 2000 1600
Connection ~ 2000 1600
Wire Wire Line
	2000 1700 2250 1700
Connection ~ 2000 1700
Wire Wire Line
	1650 1800 2000 1800
Connection ~ 2000 1800
Wire Wire Line
	4700 4500 4500 4500
Text Label 4500 4500 2    60   ~ 0
I2C_SCL
Wire Wire Line
	6500 4500 6700 4500
Text Label 6700 4500 0    60   ~ 0
I2C_SDA
Text Label 2150 5300 0    60   ~ 0
I2C_SCL
Wire Wire Line
	2000 5300 2150 5300
Wire Wire Line
	2000 5200 2150 5200
Text Label 2150 5200 0    60   ~ 0
I2C_SDA
$Comp
L Conn_PonchoMP_2x_20x2 XA1
U 5 1 590F91B1
P 1600 5250
F 0 "XA1" H 1600 5650 60  0000 C CNN
F 1 "Conn_PonchoMP_2x_20x2" H 1600 5500 60  0000 C CNN
F 2 "HM-10:Conn_Poncho_Chico" H 1450 5600 60  0001 C CNN
F 3 "" H 1450 5600 60  0000 C CNN
	5    1600 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 3400 2200 3400
NoConn ~ 2000 2600
NoConn ~ 2000 2800
NoConn ~ 2000 3000
NoConn ~ 2000 3200
NoConn ~ 6500 3800
NoConn ~ 6500 3700
NoConn ~ 6500 3600
NoConn ~ 6750 4000
$Comp
L Conn_PonchoMP_2x_20x2 XA1
U 1 1 560E549A
P 6800 1550
F 0 "XA1" H 6650 2100 60  0000 C CNN
F 1 "Conn_PonchoMP_2x_20x2" H 7100 1950 60  0000 C CNN
F 2 "HM-10:Conn_Poncho_Chico" H 7450 1100 60  0000 C CNN
F 3 "" H 6650 1900 60  0000 C CNN
	1    6800 1550
	1    0    0    -1  
$EndComp
NoConn ~ 2250 1300
NoConn ~ 8700 5350
NoConn ~ 3100 5500
NoConn ~ 6650 4200
NoConn ~ 3150 1900
NoConn ~ 2250 1800
$EndSCHEMATC
