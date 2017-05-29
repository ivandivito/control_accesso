
//#define MAIN_DEBUG

#include "mbed.h"
#include "mbed_events.h"
#include "ble/BLE.h"
#include "IBeaconAdvPackageData.h"
#include "Beacon.h"

#define SERIAL_OPERATOR_CMD '$'
#define SERIAL_OPERATOR_END 'E'
#define SERIAL_OPERATOR_START 'S'
#define SERIAL_OPERATOR_ASSIGN "="

#define CMD_BUFFER_SIZE 30

using namespace BleIBeacon;

EventQueue eventQueue(/* cantidad de eventos */ 16 * EVENTS_EVENT_SIZE);

DigitalOut led(P0_29, 1);
Serial pc(P0_9,P0_11);

bool sendUpdateEnable = false;
char cmd_buffer[CMD_BUFFER_SIZE];
int cmd_buffer_index = 0;

void periodicCallback(void)
{
    if(sendUpdateEnable){
        led = !led; /* parpadeo led poncho si estoy enviando data*/
    } else {
        led = 0;
    }
}

void advertisementCallback(const Gap::AdvertisementCallbackParams_t *params)
{

#ifdef MAIN_DEBUG
    pc.printf("Adv peerAddr: [%02x %02x %02x %02x %02x %02x] rssi %d, ScanResp: %u, AdvType: %u, AdvLen: %u\r\n",
           params->peerAddr[5], params->peerAddr[4], params->peerAddr[3], params->peerAddr[2], params->peerAddr[1], params->peerAddr[0],
           params->rssi, params->isScanResponse, params->type, params->advertisingDataLen);
    for (unsigned index = 0; index < params->advertisingDataLen; index++) {
        pc.printf("%02x ", params->advertisingData[index]);
    }
    pc.printf("\r\n");
#endif

    if(sendUpdateEnable){
        if(!isAdvertisingDatafromIBeacon(params->advertisingData,params->advertisingDataLen)){
            return;
        }

        IBeaconPayload_t *IBeaconPayload = (IBeaconPayload_t *) params->advertisingData;
        uint16_t majorNumber = IBeaconPayload->majorNumberL | IBeaconPayload->majorNumberH << 8;
        uint16_t minorNumber = IBeaconPayload->minorNumberL | IBeaconPayload->minorNumberH << 8;

        #ifdef MAIN_DEBUG
        pc.puts("beacon addr ");
        pc.printf("largo: %d :",sizeof(params->peerAddr));
        for (size_t i=0; i< sizeof(params->peerAddr); i++) {
            printf("%02x ", params->peerAddr[i]);
        }
        pc.printf("rssi %d ", params->rssi);
        pc.puts("UUID ");
        pc.printf("largo: %d :",sizeof(IBeaconPayload->beaconUUID));
        for (size_t i=0; i< sizeof(IBeaconPayload->beaconUUID); i++) {
            printf("%02x ", IBeaconPayload->beaconUUID[i]);
        }
        pc.printf("major: %lu ", (unsigned long) majorNumber);
        pc.printf("minor: %lu ", (unsigned long) minorNumber);
        pc.printf("rssi ref: %d", IBeaconPayload->rssiRef);
        pc.puts("\r\n");
        #endif

        Beacon beacon(majorNumber,minorNumber,(uint8_t)(256-IBeaconPayload->rssiRef));
        beacon.calcDistance(-(params->rssi));

        #ifdef MAIN_DEBUG
        pc.printf("ref: %lu rssi: %d distance: %u \r\n", beacon.getDistanceRef(), -(params->rssi), beacon.getLastDistance());
        #endif

        beacon.sentUpdate(pc);
    }
}

void bleInitComplete(BLE::InitializationCompleteCallbackContext *params)
{
    BLE&        ble   = params->ble;
    ble_error_t error = params->error;

    if (error != BLE_ERROR_NONE) {
        return;
    }

    if (ble.getInstanceID() != BLE::DEFAULT_INSTANCE) {
        return;
    }

    ble.gap().setScanParams(1800 /* intervalo de scan */, 1500 /* ventana de scan */);
    ble.gap().startScan(advertisementCallback);
}

void scheduleBleEventsProcessing(BLE::OnEventsToProcessCallbackContext* context) {
    BLE &ble = BLE::Instance();
    eventQueue.call(Callback<void()>(&ble, &BLE::processEvents));
}

void processCmd() {
    if(cmd_buffer_index<=1 || cmd_buffer[0] != SERIAL_OPERATOR_CMD){
        return;
    }

    if(isdigit(cmd_buffer[1])){
/*
        char buff[CMD_BUFFER_SIZE];
        char buff_aux[10];
        //busco si existe asignacion
        int assignIndex = strcspn(cmd_buffer,SERIAL_OPERATOR_ASSIGN);
        //busco el indice a modificar/leer
        strncpy(buff,cmd_buffer + 1, (size_t) (assignIndex - 1));
        buff[assignIndex-1] = '\0';
        int index = atoi(buff);
        if(assignIndex < cmd_buffer_index){

            //si hay asignacion guardo
            strncpy(buff_aux,cmd_buffer + assignIndex + 1, (size_t) (cmd_buffer_index - assignIndex));
            buff_aux[cmd_buffer_index - assignIndex] = '\0';
            int data = atoi(buff_aux);

            switch (index){
                case SERIAL_INDEX_:
                    break;

            }

        }
        //halla o no asignacion imprimo el valor
        switch (index){
            case SERIAL_INDEX_:
                break;
        }
*/
    } else {

        switch (cmd_buffer[1]){
            case SERIAL_OPERATOR_START:
                sendUpdateEnable = true;
                break;
            case SERIAL_OPERATOR_END:
                sendUpdateEnable = false;
                break;
        }

    }

}

void rxCallback() {
    char c = (char) pc.getc();
    if(c == 0x0D || c == 0x0A){
        cmd_buffer[cmd_buffer_index] = '\0';
        processCmd();
        cmd_buffer_index = 0;
    } else {
        cmd_buffer[cmd_buffer_index] = c;
        cmd_buffer_index++;
        if(cmd_buffer_index >= CMD_BUFFER_SIZE){
            cmd_buffer_index = 0;
        }
    }
}

int main()
{
    pc.baud(115200);
    #ifdef MAIN_DEBUG
    pc.printf("init\r\n");
    #endif
    pc.attach((Callback<void()>)&rxCallback, Serial::RxIrq);

    eventQueue.call_every(1000, periodicCallback);

    BLE &ble = BLE::Instance();
    ble.onEventsToProcess(scheduleBleEventsProcessing);
    ble.init(bleInitComplete);

    eventQueue.dispatch_forever();

    return 0;
}
