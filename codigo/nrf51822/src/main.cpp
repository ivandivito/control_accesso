
//#define MAIN_DEBUG

#include "mbed.h"
#include "mbed_events.h"
#include "ble/BLE.h"
#include "IBeaconAdvPackageData.h"
#include "Beacon.h"

EventQueue eventQueue(/* event count */ 16 * EVENTS_EVENT_SIZE);

DigitalOut led(P0_29, 1);
Serial pc(P0_9,P0_11);

void periodicCallback(void)
{
    led = !led; /* blink poncho led */
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

    if(!BleIBeacon::isAdvertisingDatafromIBeacon(params->advertisingData,params->advertisingDataLen)){
        return;
    }

    BleIBeacon::IBeaconPayload_t *IBeaconPayload = (BleIBeacon::IBeaconPayload_t *) params->advertisingData;
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

    Beacon beacon(majorNumber,minorNumber,256-IBeaconPayload->rssiRef);
    double dist = beacon.calcDistance(-(params->rssi));

#ifdef MAIN_DEBUG
    pc.printf("ref: %lu rssi: %d distance: %f \r\n", beacon.getDistanceRef(), -(params->rssi), dist);
#endif


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

    ble.gap().setScanParams(1800 /* scan interval */, 1500 /* scan window */);
    ble.gap().startScan(advertisementCallback);
}

void scheduleBleEventsProcessing(BLE::OnEventsToProcessCallbackContext* context) {
    BLE &ble = BLE::Instance();
    eventQueue.call(Callback<void()>(&ble, &BLE::processEvents));
}

int main()
{
    pc.baud(115200);
    pc.printf("init\r\n");

    eventQueue.call_every(1000, periodicCallback);

    BLE &ble = BLE::Instance();
    ble.onEventsToProcess(scheduleBleEventsProcessing);
    ble.init(bleInitComplete);

    eventQueue.dispatch_forever();

    return 0;
}
