//
// Created by wise on 3/30/19.
//

#ifndef TESTBLE2_ORALBLUEMANAGER_H
#define TESTBLE2_ORALBLUEMANAGER_H

#include <cmath>
#include <array>
#include <cstdint>
#include "OralBlue.h"
#include "platform/mbed_error.h"
#include "platform/mbed_debug.h"
#include "gap/AdvertisingDataParser.h"
#include "gap/Gap.h"
#include "ble/BLE.h"
#include <mbed.h>
#include <events/mbed_events.h>


template <class LED_T,std::size_t N>
struct OralBlueManager : public Gap::EventHandler,private NonCopyable<OralBlueManager<LED_T,N>> {

    explicit OralBlueManager(BLE &ble, LedManager::ProgressManager<LED_T, N> &manager) :
            _ble(ble),
            _gap(ble.gap()),
            _ledManager(manager){
        _ble.onEventsToProcess(BLE::OnEventsToProcessCallback_t(this,&OralBlueManager::schedule_ble_events));
        _ble.init(this,&OralBlueManager::on_init_complete);
        _gap.setEventHandler(this);
    }

    void start(){
        event_queue.dispatch_forever();
    }

    /** This is called when BLE interface is initialised and starts the first mode */
    void on_init_complete(BLE::InitializationCompleteCallbackContext *event){
        debug("init ok");
        scan();
    }

    /** Schedule processing of events from the BLE middleware in the event queue. */
    void schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context) {
        event_queue.call(Callback<void()>(&context->ble, &BLE::processEvents));
    }


    void onAdvertisingReport(const ble::AdvertisingReportEvent &event) override {
        const auto& payload = event.getPayload();
        auto info = OralBlue::AdvertiseInfo::fromAdvertiseData(payload.data(),payload.size());
        if(!info.has_value())
            return;
        auto sectorSeconds = info->brushingTime.count() % 30;
        _ledManager.setProgress(uint8_t(ceil(sectorSeconds * 10.0f / 3.0f)));
        debug("time: %lld\n",info->brushingTime.count());
    }

    void scan()
    {

        debug("set param");
        ble_error_t status = _gap.setScanParameters(ble::ScanParameters());
        if (status) {
            error("Error caused by Gap::setScanParameters");
        }

        debug("start");
        /* start scanning and attach a callback that will handle advertisements
         * and scan requests responses */
        status = _gap.startScan(ble::scan_duration_t::forever(),
                                ble::duplicates_filter_t::DISABLE,
                                ble::scan_period_t(0));
        if (status) {
            error("Error caused by Gap::startScan");
        }

        debug("Scan started");
    }

private:

    BLE &_ble;
    Gap &_gap;
    LedManager::ProgressManager<LED_T, N> _ledManager;
    events::EventQueue event_queue;
};

#endif //TESTBLE2_ORALBLUEMANAGER_H
