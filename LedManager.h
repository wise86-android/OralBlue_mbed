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

#ifndef ORALBLUE_LEDMANAGER_H
#define ORALBLUE_LEDMANAGER_H

#include <cstdint>
#include <array>

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

#endif //ORALBLUE_LEDMANAGER_H
