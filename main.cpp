
#include <events/mbed_events.h>
#include <mbed.h>
#include "ble/BLE.h"
#include "gap/Gap.h"
#include "gap/AdvertisingDataParser.h"

#include "platform/mbed_debug.h"
#include "platform/mbed_error.h"

#include "OralBlue.h"

events::EventQueue event_queue;

/* Duration of each mode in milliseconds */
static const size_t MODE_DURATION_MS      = 6000;

/* Time between each mode in milliseconds */
static const size_t TIME_BETWEEN_MODES_MS = 2000;

/* how long to wait before disconnecting in milliseconds */
static const size_t CONNECTION_DURATION   = 3000;

/* how many advertising sets we want to crate at once */
static const uint8_t ADV_SET_NUMBER       = 2;

static const uint16_t MAX_ADVERTISING_PAYLOAD_SIZE = 1000;

#include <cstdint>
#include <array>
#include <cmath>

namespace LedManager{

    template <typename LedImplementation>
    class Led {
    public:
        void setDimming(uint8_t dimmingValue){
            static_cast<LedImplementation*>(this)->setDimmingImpl(dimmingValue);
        }
    };

    template <class LED_T,std::size_t N>
    struct ProgressManager{

        explicit ProgressManager(std::array<LED_T,N> &leds):mLeds(leds){}

        void setProgress(uint8_t progress){
            progress = std::min(uint8_t{100},progress);
            auto remainingProgress = progress*N;
            for(size_t i= 0; i<N ; i++){
                if(remainingProgress>100){
                    mLeds[i].setDimming(100);
                    remainingProgress-=100;
                }else{
                    mLeds[i].setDimming(remainingProgress);
                    remainingProgress = 0;
                }
            }
        }

        ~ProgressManager(){
            for(auto &led : mLeds){
                led.setDimming(0);
            }
        }


    private:
        std::array<LED_T,N>&  mLeds;
    };

}

class  PWMLed : public LedManager::Led<PWMLed>{

public:
    explicit PWMLed(PinName pin):pwmOut(pin){}

    void setDimmingImpl(uint8_t value){
        pwmOut=value/255.0f;
    }

private:
    PwmOut pwmOut;

};

struct BLEScanner : public ble::Gap::EventHandler,private mbed::NonCopyable<BLEScanner> {

    explicit BLEScanner(BLE &ble, LedManager::ProgressManager<PWMLed, 3> &manager) :
        _ble(ble),
        _gap(ble.gap()),
        _ledManager(manager){
        _ble.init(this,&BLEScanner::on_init_complete);
        _gap.setEventHandler(this);
    }

    /** This is called when BLE interface is initialised and starts the first mode */
    void on_init_complete(BLE::InitializationCompleteCallbackContext *event){
        debug("init ok");
        scan();
    }

    void onAdvertisingReport(const ble::AdvertisingReportEvent &event) override {
        const auto& payload = event.getPayload();
        auto info = OralBlue::AdvertiseInfo::fromAdvertiseData(payload.data(),payload.size());
        if(!info.has_value())
            return;
        auto sectorSeconds = info->brushingTime.count() % 30;
        _ledManager.setProgress(uint8_t(std::ceil(sectorSeconds*10.0f/3.0f)));
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
    LedManager::ProgressManager<PWMLed, 3> _ledManager;
};

/** Schedule processing of events from the BLE middleware in the event queue. */
void schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context) {
    event_queue.call(Callback<void()>(&context->ble, &BLE::processEvents));
}


constexpr PinName RED_LED = PB_8;
constexpr PinName YELLOW_LED = PB_9;
constexpr PinName GREEN_LED = PA_10;

int main()
{

    std::array leds{ PWMLed(RED_LED),PWMLed(YELLOW_LED),PWMLed(GREEN_LED)};
    LedManager::ProgressManager ledManager{leds};

    BLE &ble = BLE::Instance();
    ble.onEventsToProcess(schedule_ble_events);
    BLEScanner scanning(ble, ledManager);
    event_queue.dispatch_forever();
    return 0;
}
