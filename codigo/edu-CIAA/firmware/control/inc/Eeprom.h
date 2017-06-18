//
// Created by ivan on 18/06/17.
//

#ifndef CONTROL_EEPROM_H
#define CONTROL_EEPROM_H

#include "stdint.h"
#include "chip.h"
#include "stddef.h"
#include "sapi.h"

#define EEPROM_ADDRESS_CHECK 0x0000
#define EEPROM_ADDRESS_BEACON_STATUS 0x0001
#define EEPROM_ADDRESS_BEACON_DATABASE 0x0003

#define EEPROM_CHECK_TOKEN 0xAB

void Board_EEPROM_init(void);
bool_t Board_EEPROM_writeByte(uint32_t addr,uint8_t value);
bool_t Board_EEPROM_writeData(uint32_t* addr,uint8_t* data, size_t dataSize);
uint8_t Board_EEPROM_readByte(uint32_t addr);
bool_t Board_EEPROM_readData(uint32_t* addr,uint8_t* data, size_t dataSize);


#endif //CONTROL_EEPROM_H
