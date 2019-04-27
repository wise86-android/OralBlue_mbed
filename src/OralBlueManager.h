//Copyright (c) 2019, Giovanni Visentini
//SPDX-License-Identifier: Apache-2.0
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//
//You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
//either express or implied.
//
//See the License for the specific language governing permissions and limitations under the License.


#ifndef ORALBLUE_ORALBLUEMANAGER_H
#define ORALBLUE_ORALBLUEMANAGER_H

#include <cmath>
#include <array>
#include <cstdint>

#include "platform/mbed_error.h"
#include "platform/mbed_debug.h"
#include "gap/AdvertisingDataParser.h"
#include "gap/Gap.h"
#include "ble/BLE.h"
#include "mbed.h"
#include "events/mbed_events.h"

#include "LCDManagerTextLCD.h"
#include "OralBlue.h"

template <class LED_T,std::size_t N>
struct OralBlueManager : public Gap::EventHandler,private NonCopyable<OralBlueManager<LED_T,N>> {

    explicit OralBlueManager(BLE &ble, LedManager::ProgressManager<LED_T, N> &manager,LCDManagerTextLCD &lcdManager) :
            _ble(ble),
            _gap(ble.gap()),
            _ledManager(manager),
            _display(lcdManager){
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
        auto progressPercentage = uint8_t(ceil(sectorSeconds * 10.0f / 3.0f)) ;
        _ledManager.setProgress(progressPercentage);
        _display.setProgress(info->sector,progressPercentage);
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
    LCDManagerTextLCD& _display;
    events::EventQueue event_queue;
};

#endif //ORALBLUE_ORALBLUEMANAGER_H
