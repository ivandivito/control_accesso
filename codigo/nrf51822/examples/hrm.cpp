/*
DigitalOut led1(P0_29);

led1 = 0;
wait(1);
pc.baud(115200);
pc.printf("init\n");
*/

#include "app_util.h"
#include "mbed.h"
#include "ble/BLE.h"
#include "ble/services/HeartRateService.h"
#include "ble/services/BatteryService.h"
#include "ble/services/DeviceInformationService.h"

#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(379, UNIT_1_25_MS)              /**< Minimum connection interval (379 ms) */
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(399, UNIT_1_25_MS)              /**< Maximum connection interval (399 ms). */
#define SLAVE_LATENCY                   4                                             /**< Slave latency. */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(6000, UNIT_10_MS)               /**< Connection supervisory timeout (6 seconds). */

DigitalOut led1(P0_29);
Serial pc(P0_9, P0_11);

const static char     DEVICE_NAME[]        = "HRM1";
static const uint16_t uuid16_list[]        = {GattService::UUID_HEART_RATE_SERVICE,
                                              GattService::UUID_DEVICE_INFORMATION_SERVICE};
static volatile bool  triggerSensorPolling = false;

uint8_t hrmCounter = 100; // init HRM to 100bps

HeartRateService         *hrService;
DeviceInformationService *deviceInfo;

void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *params)
{
    BLE::Instance(BLE::DEFAULT_INSTANCE).gap().startAdvertising(); // restart advertising
}


void periodicCallback(void)
{
    led1 = !led1;

    triggerSensorPolling = true;
}

void onConnectionCallback(const Gap::ConnectionCallbackParams_t *p_conn_param)
{
    Gap::ConnectionParams_t gap_conn_params;
    gap_conn_params.minConnectionInterval = MIN_CONN_INTERVAL;
    gap_conn_params.maxConnectionInterval = MAX_CONN_INTERVAL;
    gap_conn_params.slaveLatency = SLAVE_LATENCY;
    gap_conn_params.connectionSupervisionTimeout = CONN_SUP_TIMEOUT;
    BLE::Instance(BLE::DEFAULT_INSTANCE).updateConnectionParams(p_conn_param->handle, &gap_conn_params);
}


void bleInitComplete(BLE::InitializationCompleteCallbackContext *params)
{
    BLE &ble          = params->ble;
    ble_error_t error = params->error;

    if (error != BLE_ERROR_NONE) {
        return;
    }

    ble.gap().onDisconnection(disconnectionCallback);
    ble.gap().onConnection(onConnectionCallback);

    /* Setup primary service. */
    hrService = new HeartRateService(ble, hrmCounter, HeartRateService::LOCATION_FINGER);

    /* Setup auxiliary service. */
    deviceInfo = new DeviceInformationService(ble, "ARM", "Model1", "SN1", "hw-rev1", "fw-rev1", "soft-rev1");

    /* Setup advertising. */
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::GENERIC_HEART_RATE_SENSOR);
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
    ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    ble.gap().setAdvertisingInterval(1000); /* 1000ms */
    ble.gap().startAdvertising();
}

int main(void)
{
    led1 = 0;
    pc.baud(115200);

    Ticker ticker;
    ticker.attach(periodicCallback, 1); // blink LED every second

    BLE& ble = BLE::Instance(BLE::DEFAULT_INSTANCE);
    ble.init(bleInitComplete);

    /* SpinWait for initialization to complete. This is necessary because the
     * BLE object is used in the main loop below. */
    while (ble.hasInitialized()  == false) { /* spin loop */ }

    // infinite loop
    while (1) {
        // check for trigger from periodicCallback()
        if (triggerSensorPolling && ble.getGapState().connected) {
            triggerSensorPolling = false;

            // Do blocking calls or whatever is necessary for sensor polling.
            // In our case, we simply update the HRM measurement.
            hrmCounter++;
            if (hrmCounter == 175) { //  100 <= HRM bps <=175
                hrmCounter = 100;
            }

            hrService->updateHeartRate(hrmCounter);
        } else {
            ble.waitForEvent(); // low power wait for event
        }
    }
}