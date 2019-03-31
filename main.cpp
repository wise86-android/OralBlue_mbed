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

#include "mbed.h"
#include "ble/BLE.h"

#include "src/PinConfiguration.h"
#include "src/LedManager.h"
#include "src/OralBlue.h"
#include "src/OralBlueManager.h"

class  PWMLed : public LedManager::Led<PWMLed>{

public:
    explicit PWMLed(PinName pin):pwmOut(pin){}

    void setDimmingImpl(uint8_t value){
        pwmOut=value/255.0f;
    }

private:
    PwmOut pwmOut;

};

int main(){

    std::array leds{ PWMLed(Configuration::RED_LED),
                     PWMLed(Configuration::YELLOW_LED),
                     PWMLed(Configuration::GREEN_LED)};
    LedManager::ProgressManager ledManager{leds};

    BLE &ble = BLE::Instance();
    OralBlueManager oralBlueManager(ble, ledManager);
    oralBlueManager.start();
    return 0;
}
