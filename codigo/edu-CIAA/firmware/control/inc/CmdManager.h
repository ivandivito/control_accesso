//
// Created by ivan on 12/06/17.
//

#ifndef CONTROL_CMDMANAGER_H
#define CONTROL_CMDMANAGER_H

#include "ctype.h"
#include "stddef.h"
#include "sapi.h"
#include "Beacon.h"
#include "BeaconManager.h"

#define SERIAL_OPERATOR_CMD '$'
#define SERIAL_OPERATOR_SPACE " "
#define SERIAL_OPERATOR_START 'S'
#define SERIAL_OPERATOR_END 'E'
#define SERIAL_OPERATOR_READ 'R'

#define PC_START_MSG "start ble scan\r\n"
#define PC_END_MSG "end ble scan\r\n"
#define PC_FORMAT_ERROR_MSG "format error\r\n"

#define PC_READ_STATUS_CMD_OFFSET 2
#define PC_ASSIGN_CALLBACK_CMD_OFFSET 1

#define CMD_BUFFER_SIZE 30

void cmdManagerTask(void * a);
void initCmdManagerTask(uint32_t priority);

size_t getCmdLength();
bool_t parseReadStatusCmd(char* data);
bool_t parseAssignCbCmd(char* data);

#endif //CONTROL_CMDMANAGER_H
