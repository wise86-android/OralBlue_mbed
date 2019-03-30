#include <mbed.h>
#include "ble/BLE.h"

#include "LedManager.h"
#include "OralBlue.h"
#include "OralBlueManager.h"


class  PWMLed : public LedManager::Led<PWMLed>{

public:
    explicit PWMLed(PinName pin):pwmOut(pin){}

    void setDimmingImpl(uint8_t value){
        pwmOut=value/255.0f;
    }

private:
    PwmOut pwmOut;

};

constexpr PinName RED_LED = PB_8;
constexpr PinName YELLOW_LED = PB_9;
constexpr PinName GREEN_LED = PA_10;

int main()
{

    std::array leds{ PWMLed(RED_LED),PWMLed(YELLOW_LED),PWMLed(GREEN_LED)};
    LedManager::ProgressManager ledManager{leds};

    BLE &ble = BLE::Instance();
    OralBlueManager scanning(ble, ledManager);
    scanning.start();
    return 0;
}
